


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <ctype.h>
#include "FPToolkit.c"

int ITERS = 1000;
int SWIDTH = 800;
int SHEIGHT = 800;
double MIN_TEST = -2;
double MAX_TEST = 2;

void prcmx (char *control, complex c)
//print complex number
{
  double a,b ;
  a = creal(c) ;
  b = cimag(c) ;

  printf(control,a) ;
  if (b >= 0) {
    printf("+") ;
  } else {
    printf("-") ;
  }
  printf(control,fabs(b)) ;  
  printf("I") ;
}


complex sccmx ()
//scan complex number  
{
  double rp,ip ;
  complex c ;
  scanf("%lf %lf",&rp, &ip) ;
  c = rp + ip*I ;
  return c ;
}

int check_convergence(complex z){
    if(cabs(z) > 2){//cabs greater than 2, the number diverges
        return 0;
    }
    else{
        return 1; //otherwise, the value is probably converging
    }
}

complex z_func(complex z, complex c){
    return cpow(z,2) + c;
}

int test_z(double real, double complex_coef){
    complex z = 0 + 0*I;
    complex c = real + complex_coef*I;

    for(int i = 0; i<ITERS; ++i){
        z = z_func(z,c);
    }

    return check_convergence(z);
}

int main()
{
    double converge_color [] = {102/255.0, 51/255.0, 153/255.0};
    double diverge_color [] = {34/255.0, 39/255.0, 122/255.0};
    //one interval for a square screen
    double test_interval = (abs(MAX_TEST) + abs(MIN_TEST))/SWIDTH;
    double cur_real = MIN_TEST;
    double cur_complex = MIN_TEST;
    int does_converge = 0;

    //x is along the real axis
    for(int x = 0; x < SWIDTH; ++x){
        // y is along the imaginary axis
        for (int y = 0; y < SHEIGHT; ++y){
            //test the current complex number mapped to the current pixel coordinate
            cur_real = MIN_TEST+(x*test_interval);
            cur_complex = MIN_TEST+(y*test_interval);
            does_converge = test_z(cur_real,cur_complex);

            if(does_converge){
                G_rgb(converge_color[0],converge_color[1],converge_color[2]);
                G_point(x,y);
            }
            else{
                G_rgb(diverge_color[0],diverge_color[1],diverge_color[2]);
                G_point(x,y);
            }

        }
    }

    int key ;   
    key =  G_wait_key() ; // pause so user can see results


}

