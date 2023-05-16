

#include  "FPToolkit.c"
#include <math.h>

double SQ_SIZE = 600; //initial square size
double ITERS = 1000000;


double parametric(double x0, double x1, double t, double power){
    //default power should be 1
    return x0 + pow(t,power)*(x1 - x0);
}


//transformations which will be applied in the following order:
// 1. scale     2. translate    3. rotate

//cales values of a point
int scale(double p[], double scale_factor){

    /*double new_x,new_y;

    new_x = parametric(p1[0],p2[0],scale_factor,1);
    new_y = parametric(p1[1],p2[1],scale_factor,1);

    p2[0] = new_x;
    p2[1] = new_y;
    */

    p[0] *= scale_factor;
    p[1] *= scale_factor;

    return 1;

}

int translate(double p[],double delta_x,double delta_y){

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


/***********************************

9 total rules for the sierpinsky carpet

    1 | 2 | 3
    4 | 5 | 6
    7 | 8 | 9


parent space is unit square
1. pick a random point in that square
2. apply transformation of rules (randomly)


Origin of each sub square is in the bottom left

*************************************/
int rule1(double p[]){

    //scale down
    scale(p,1.0/3.0);
    //translate to the top left of the parent
    double delta_x = 0.0;
    double delta_y = SQ_SIZE*(2.0/3.0); //move up 2 thirds the way
    translate(p,delta_x,delta_y);


    return 0;
}

int rule2(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate to the top center of the parent
    double delta_x = SQ_SIZE*(1.0/3.0); // move over 1 third the way
    double delta_y = SQ_SIZE*(2.0/3.0); //move up 2 thirds the way
    translate(p,delta_x,delta_y);

    return 0;
}

int rule3(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = SQ_SIZE*(2.0/3.0); // move over 2 thirds the way
    double delta_y = SQ_SIZE*(2.0/3.0); //move up 2 thirds the way
    translate(p,delta_x,delta_y);

    return 0;
}

int rule4(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = 0;
    double delta_y = SQ_SIZE*(1.0/3.0); 
    translate(p,delta_x,delta_y);

    return 0;
}

int rule5(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = SQ_SIZE*(1.0/3.0);
    double delta_y = SQ_SIZE*(1.0/3.0); 
    translate(p,delta_x,delta_y);

    return 0;
}

int rule6(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = SQ_SIZE*(2.0/3.0);
    double delta_y = SQ_SIZE*(1.0/3.0); 
    translate(p,delta_x,delta_y);

    return 0;
}

int rule7(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = 0;
    double delta_y = 0; 
    translate(p,delta_x,delta_y);

    return 0;
}

int rule8(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = SQ_SIZE*(1.0/3.0);
    double delta_y = 0; 
    translate(p,delta_x,delta_y);

    return 0;
}

int rule9(double p[]){
    //scale down
    scale(p,1.0/3.0);
    //translate 
    double delta_x = SQ_SIZE*(2.0/3.0);
    double delta_y = 0; 
    translate(p,delta_x,delta_y);

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
    G_rgb (0,1,0);
    double p [2];
    p[0] = 100; // starting x
    p[1] = 10; // starting y

    //double n_iters = ITERS;
    srand(time(NULL));
    int choice = 0;
    for(int i=0; i< ITERS; ++i){
        choice = rand() % 8;
        if(choice == 0) rule1(p);
        if(choice == 1) rule2(p);
        if(choice == 2) rule3(p);
        if(choice == 3) rule4(p);
        //note, we exclude rule 5
        if(choice == 4) rule6(p);
        if(choice == 5) rule7(p);
        if(choice == 6) rule8(p);
        if(choice == 7) rule9(p);

        G_point (p[0], p[1]);
    }

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}
