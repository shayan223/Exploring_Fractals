

#include  "FPToolkit.c"
#include <math.h>



double parametric(double x0, double x1, double t, double power){
    //default power should be 1
    return x0 + pow(t,power)*(x1 - x0);
}


//transformations which will be applied in the following order:
// 1. scale     2. translate    3. rotate

//modifies p2 to scale up or down from p1
int scale(double p1[], double p2[], double scale_factor){

    double new_x,new_y;

    new_x = parametric(p1[0],p2[0],scale_factor,1);
    new_y = parametric(p1[1],p2[1],scale_factor,1);

    p2[0] = new_x;
    p2[1] = new_y;

    return 1;

}

int translate(double p[],delta_x,delta_y){

    p[0] += delta_x;
    p[1] += delta_y;

    return 1;
    
}


int rotate(double center [], double p [], double angle){

    /* rotation around a center is:
    
    p(x,y) -> 
        x = (centerx + (x - centerx)cos(angle) - (y - centery)sin(angle))
        y = (centery + (x - centerx)sin(angle) + (y - centery)cos(angle))

    */

    double original_x = p[0];
    double original_y = p[1];
    double rad_angle = angle* (M_PI/180.0);
    p[0] = (center[0] + (original_x - center[0]) * cos(rad_angle) - (original_y - center[1])*sin(rad_angle));
    p[1] = (center[1] + (original_x - center[0]) * sin(rad_angle) + (original_y - center[1])*cos(rad_angle));

    return 1;

}

int rule1(double p[]){

    return 0;
}



int main()
{
    int    swidth, sheight ;
    
    // must do this before you do 'almost' any other graphical tasks 
    swidth = 800 ;  sheight = 800 ;
    G_init_graphics (swidth,sheight) ;  // interactive graphics

    
    // clear the screen in a given color
    G_rgb (0.3, 0.3, 0.3) ; // dark gray
    G_clear () ;

    double p [2];
    p[0] = 100; // starting x
    p[1] = 10; // starting y

    double n;


    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}
