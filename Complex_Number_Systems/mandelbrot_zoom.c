


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
double MAX_TEST_r = 0.0;
double MIN_TEST_c = -1;
double MAX_TEST_c = 1;
double ZOOM_RATE = 0.6;
int RECORDING_LENGTH = 100;

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

double parametric(double x0, double x1, double t, double power){
    //default power should be 1
    return x0 + pow(t,power)*(x1 - x0);
}

int check_convergence(complex z){
    if(cabs(z) > 2){//cabs greater than 2, the number diverges
        return 0;
    }
    else{
        return 1; //otherwise, the value is probably converging
    }
}


int test_z(double real, double complex_coef){
    int does_converge = 0;
    int reps = 0;//repitions to convergence
    int i;
    complex z = 0 + 0*I;
    complex c = real + complex_coef*I;
    //z = cpow(z,2) + c;
    for(i = 0; i<ITERS; ++i){
        reps += 1;
        z = cpow(z,2) + c;
        
        if(cabs(z) > 2){//check convergence
            break;
            //return
        }
    }

    return i;
}

int main()
{
    G_init_graphics (SWIDTH,SHEIGHT) ;

    double converge_color [] = {102/255.0, 51/255.0, 153/255.0};
    double diverge_color [] = {34/255.0, 39/255.0, 122/255.0};
    double c1 [] = {1, 0, 0};
    double c2 [] = {0, 1, 0};
    double c3 [] = {0, 0, 1};

    int key ;   
    double center[2], width[2];
    double iter_scaling = 0.0;
    double window_width_x, window_width_y, center_real, center_complex, width_real, width_complex ;
    center_real = -1.5;
    center_complex = 0.0;

    width_real = .5;
    width_complex = .5;

    char fname[100];
    int frames = 0;
    while(frames < RECORDING_LENGTH){
        
        

        //one interval for a square screen
        double test_interval_x = (fabs(MAX_TEST_r - MIN_TEST_r))/(double)SWIDTH;
        double test_interval_y = (fabs(MAX_TEST_c - MIN_TEST_c))/(double)SHEIGHT;

        double cur_real = MIN_TEST_r;
        double cur_complex = MIN_TEST_c;
        int does_converge = 0;

        G_rgb(0.3,0.3,0.3);
        G_clear();

        int reps = 0;
        double color_scale = 0;
        double r,g,b;
        //x is along the real axis
        for(int x = 0; x < SWIDTH; ++x){
            // y is along the imaginary axis
            for (int y = 0; y < SHEIGHT; ++y){
                //test the current complex number mapped to the current pixel coordinate
                cur_real = MIN_TEST_r+(x*test_interval_x);
                cur_complex = MIN_TEST_c+(y*test_interval_y);
                reps = test_z(cur_real,cur_complex);
                
                
                color_scale = (double)reps/(double)ITERS;
                r = parametric(converge_color[0],diverge_color[0],color_scale,3);
                g = parametric(converge_color[1],diverge_color[1],color_scale,3);
                b = parametric(converge_color[2],diverge_color[2],color_scale,3);
                
                G_rgb(r,g,b);
                G_point(x,y);

            }
        }


        
        width_real = width_real * ZOOM_RATE;
        width_complex = width_complex * ZOOM_RATE;
        //scale up iterations per pixel as we zoom in
        iter_scaling = 1 + (1 - ZOOM_RATE);

        //compute the new starting min and max values based on the center
        window_width_x = fabs((center_real - width_real));
        window_width_y = fabs(center_complex - width_complex);
        MIN_TEST_r = center_real - window_width_x;
        MAX_TEST_r = center_real + window_width_x;

        MIN_TEST_c = center_complex - window_width_x;
        MAX_TEST_c = center_complex + window_width_x;

        ITERS = ITERS * iter_scaling ;

        //save frame and iterate frame count
        int n = snprintf(fname, 99,"./frames/img%04d.bmp", frames);

        if(n > -1 && n < 99){
        int fp = fopen(fname, "w");
        }else{
            printf("Error saving file");
        }

        G_save_to_bmp_file(fname);
        printf("Saving Frame: %d/%d \n",frames,RECORDING_LENGTH);
        frames += 1;
    }


}

