#include "FPToolkit.c"

/*

       Y+
       |
       |    * (y,z)
       |  
<------+--------Z+
   D   |
       |
       |


y'    y
- =  ---
D    D+z

H = D*tan(halfangle) ;

with the x-axis perpendicular to this plane.

*/

#define M 700000
double Wsize = 1000  ; // window size ... choose 600 for repl
double X[M],Y[M],Z[M] ;
double Xplot[M],Yplot[M] ;
int N = 0 ;


int translate(double dx, double dy, double dz) 
{
  int i ;

  for (i = 0 ; i < N ; i++) {
    X[i] += dx ;
    Y[i] += dy ;
    Z[i] += dz ;    
  }
}



int rotate_x(double degrees)
// Y[] and Z[] will change but X[] will be unchanged
{
  double radians,c,s,temp ;
  int i ;
  
  radians = degrees*M_PI/180 ;
  c = cos(radians) ;
  s = sin(radians) ;
  for (i = 0 ; i < N ; i++) {
    temp =  c*Y[i] - s*Z[i] ;
    Z[i] =  s*Y[i] + c*Z[i] ;
    Y[i] = temp ;
  }
}




int rotate_y(double degrees)
// X[] and Z[] will change but Y[] will be unchanged
{
  double radians,c,s,temp ;
  int i ;
  
  radians = degrees*M_PI/180 ;
  c = cos(radians) ;
  s = sin(radians) ;
  for (i = 0 ; i < N ; i++) {
    temp =  c*X[i] + s*Z[i] ;
    Z[i] = -s*X[i] + c*Z[i] ;
    X[i] = temp ;
  }
}



int rotate_z(double degrees)
// X[] and Y[] will change but Z[] will be unchanged
{
  double radians,c,s,temp ;
  int i ;
  
  radians = degrees*M_PI/180 ;
  c = cos(radians) ;
  s = sin(radians) ;
  for (i = 0 ; i < N ; i++) {
    temp =  c*X[i] - s*Y[i] ;
    Y[i] =  s*X[i] + c*Y[i] ;
    X[i] = temp ;
  }
}



int project(double observer_distance, double halfangle_degrees)
{

  // student work goes here
  //given x,y,z -> x',y' -> x_p, y_p
  double x_prime, y_prime, scale_factor, screen_scale, H;
  // observation box is 2H by 2H
  // be sure to convert degrees to radians
  H = observer_distance * (tan(halfangle_degrees *(M_PI/180)));
  for(int i=0; i< N; ++i){
    scale_factor = observer_distance / (observer_distance + Z[i]);
    x_prime = X[i]*scale_factor;
    y_prime = Y[i]*scale_factor;

    //translate to origin then scale to screen size
    screen_scale = Wsize / (2*H);
    Xplot[i] = (x_prime + H)*screen_scale;
    Yplot[i] = (y_prime + H)*screen_scale;
  }
  
}


int draw()
{
  int i ;
  for (i = 0 ; i < N ; i=i+2) {
    G_line(Xplot[i],Yplot[i],  Xplot[i+1],Yplot[i+1]) ;
  }

}



int print_object()
{
  int i ;
  for (i = 0 ; i < N ; i=i+2) {
    printf("(%lf, %lf, %lf)   (%lf, %lf, %lf)\n", 
	   X[i],Y[i],Z[i],  X[i+1],Y[i+1],Z[i+1]) ;
  }

  printf("===============================\n") ;
  
  for (i = 0 ; i < N ; i=i+2) {
    printf("(%lf, %lf)   (%lf, %lf)\n", 
	   Xplot[i],Yplot[i],  Xplot[i+1],Yplot[i+1]) ;
  }  
}



int save_line(double xs, double ys, double zs,
	      double xe, double ye, double ze)
{
  // about to add items at slots N and N+1 :
  if (N+1 >= M) {
    printf("full\n") ;
    return 0 ;
  }
  
  X[N] = xs ; Y[N] = ys ; Z[N] = zs ; N++ ;
  X[N] = xe ; Y[N] = ye ; Z[N] = ze ; N++ ;

  return 1 ;
}




int build_pyramid()
{
  int n,k ;
  double a,x[100],z[100],yv ;

  N = 0 ; // global
  
  n = 4 ;
  for (k = 0; k <= n ; k++) {
    a = k * 2*M_PI/n ; 
    x[k] = cos(a) ;
    z[k] = sin(a) ;
  }

  yv = -1 ;
  for (k = 0; k < n ; k++) {
    save_line(0,2,0,  x[k],yv,z[k]) ;
    save_line(x[k],yv,z[k],    x[k+1],yv,z[k+1]) ;
  }  
  
}


int build_cube(){
  int n,k ;
  double a,x[100],z[100],yv ;

  N = 0 ; // global
  //NOTE: Can change to any n sided square-like-shape
  n = 4 ;
  double base_start = -1;
  double side_len = 2;
  double per_side_angle = (360/n)*(M_PI/180.0);
  double radius = side_len/2.0;
  double cur_angle = 0;
  for (k = 0; k <= n ; k++) {
    x[k] = radius*cos(cur_angle) ;
    z[k] = radius*sin(cur_angle) ;
    cur_angle += per_side_angle;
  }

  //yv = base_start+side_len ;
  //to create square, height should be the same as side lengths
  yv = sqrt(pow(x[1] - x[0],2) + pow(z[1] - z[0],2));
  base_start = 0 - yv/2.0;

  printf("Starting y: %f, top of shape: %f, radius: %f \n",base_start,yv,radius);
  printf("Starting y: %f, top of shape: %f, radius: %f",base_start,yv,radius);

  for (k = 0; k < n ; k++) {
    //save_line(0,2,0,  x[k],yv,z[k]) ;
    //save_line(x[k],yv,z[k],    x[k+1],yv,z[k+1]) ;
    //connect a point to its next around the base
    save_line(x[k],base_start,z[k],    x[k+1],base_start,z[k+1]);
    //connect each point with the one above it
    save_line(x[k],base_start,z[k],    x[k],base_start+yv,z[k]);
    //connect each above point to the one next to it
    save_line(x[k],base_start+yv,z[k],    x[k+1],base_start+yv,z[k+1]);
    //connect first and last sides
    if(k == n-1){ 
      save_line(x[k],base_start,z[k],    x[0],base_start,z[0]); //bottom points
      save_line(x[k],base_start+yv,z[k],    x[0],base_start+yv,z[0]); //top points
    }
  }  

  


}


int test_cube(){
  G_init_graphics(Wsize,Wsize) ;
  G_rgb(0,0,0) ;
  G_clear() ;
  G_rgb(0,1,0) ;

  build_cube() ;
  project(3,45) ;
  draw() ;
  print_object() ;
  
  G_wait_key() ;
}

int test_cube_rotate(){
  //  G_choose_repl_display() ; // not too bad as a repl movie
  G_init_graphics(Wsize,Wsize) ;

  build_cube() ;
  
  while (1) {
    G_rgb(0,0,0) ;
    G_clear() ;
    G_rgb(0,1,0) ;
    project(3,45) ;
    draw() ;
    rotate_y(2) ;  
    if (G_wait_key() == 'q') { break ; }
  }
  
}


int test_pyramid()
{
  G_init_graphics(Wsize,Wsize) ;
  G_rgb(0,0,0) ;
  G_clear() ;
  G_rgb(0,1,0) ;

  build_pyramid() ;
  project(3,45) ;
  draw() ;
  print_object() ;
  
  G_wait_key() ;
}




int test_pyramid_rotate()
{
  //  G_choose_repl_display() ; // not too bad as a repl movie
  G_init_graphics(Wsize,Wsize) ;

  build_pyramid() ;
  
  while (1) {
    G_rgb(0,0,0) ;
    G_clear() ;
    G_rgb(0,1,0) ;
    project(3,45) ;
    draw() ;
    rotate_y(2) ;  
    if (G_wait_key() == 'q') { break ; }
  }
  
}




int main()
{
  //test_pyramid() ;
  //test_pyramid_rotate() ;
  //test_cube();
  test_cube_rotate();
}
