


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <ctype.h>
#include "FPToolkit.c"


int SWIDTH = 800;
int SHEIGHT = 800;

int ITERS = 100;
double MIN_TEST_r = -2;
double MAX_TEST_r = 2;
double MIN_TEST_c = -2;
double MAX_TEST_c = 2;

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
    G_init_graphics (SWIDTH,SHEIGHT) ;

    double converge_color [] = {102/255.0, 51/255.0, 153/255.0};
    double diverge_color [] = {34/255.0, 39/255.0, 122/255.0};


    int key ;   
    double center[2], width[2];

    while(key != 113){
        
        

        //one interval for a square screen
        double test_interval_x = (abs(MAX_TEST_r) + abs(MIN_TEST_r))/(double)SWIDTH;
        double test_interval_y = (abs(MAX_TEST_c) + abs(MIN_TEST_c))/(double)SWIDTH;

        printf("Intervals for window: x:%f   y:%f\n",test_interval_x,test_interval_y);
        double cur_real = MIN_TEST_r;
        double cur_complex = MIN_TEST_c;
        int does_converge = 0;

        G_rgb(0.3,0.3,0.3);
        G_clear();

        //x is along the real axis
        for(int x = 0; x < SWIDTH; ++x){
            // y is along the imaginary axis
            for (int y = 0; y < SHEIGHT; ++y){
                //test the current complex number mapped to the current pixel coordinate
                cur_real = MIN_TEST_r+(x*test_interval_x);
                cur_complex = MIN_TEST_c+(y*test_interval_y);
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

        G_wait_click(center);
        G_wait_click(width);

        //convert clicked location to complex plane
        cur_real = MIN_TEST_r + (center[0]*test_interval_x);
        cur_complex = MIN_TEST_c + (center[1]*test_interval_y);

        //compute the new starting min and max values based on the center
        MIN_TEST_r = cur_real - width[0];
        MAX_TEST_r = cur_real + width[0];

        MIN_TEST_c = cur_complex - width[1];
        MAX_TEST_c = cur_complex + width[1];



        //key =  G_wait_key() ; // pause so user can see results
    }


}

