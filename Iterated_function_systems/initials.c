

#include  "FPToolkit.c"
#include <math.h>

//double SQ_SIZE = 600; //initial square size
double POLY_W = 500;
double POLY_H = 400;
double THICKNESS = 50;
double BUF_DIST = 50.0;
double ITERS = 6000000;


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

//horizontal scaling
int scale_x(double p[], double scale_factor){
    p[0] *= scale_factor;
}

//vertical scaling
int scale_y(double p[], double scale_factor){
    p[1] *= scale_factor;
}

int translate(double p[],double delta_x,double delta_y){

    p[0] += delta_x;
    p[1] += delta_y;

    return 1;

}

double find_scale_factor(double start, double end){
    return (end / start);
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

Construct SJ initials, from bottom of the S
----
|
----
   |
----

and 

------
   |
   |
|__|
*************************************/
int bottom_S(double p[]){

    //create scale factor for mama-shape to desired shape
    double scale_w = find_scale_factor(POLY_W,POLY_W/2.0);
    double scale_h = find_scale_factor(POLY_H,THICKNESS);
    //printf("WIDTH: %f,   HEIGHT: %f",scale_w,scale_h);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);
    //translate to bottom of S
    double delta_x = BUF_DIST;
    double delta_y = BUF_DIST; //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

int middle_S(double p[]){

    //create scale factor for mama-shape to desired shape
    double scale_w = find_scale_factor(POLY_W,POLY_W/2.0);
    double scale_h = find_scale_factor(POLY_H,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);
    //translate to middle of S
    double delta_x = BUF_DIST;
    double delta_y = BUF_DIST + (POLY_H*(.5) - THICKNESS); //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

int top_S(double p[]){

    //create scale factor for mama-shape to desired shape
    double scale_w = find_scale_factor(POLY_W,POLY_W/2.0);
    double scale_h = find_scale_factor(POLY_H,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);
    //translate to top of S
    double delta_x = BUF_DIST; //+ POLY_W/2.0;
    double delta_y = BUF_DIST + (POLY_H - THICKNESS); //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

//bottom right side
int side_S_1(double p[]){



    //create scale factor for mama-shape to desired shape
    //H and W are flipped, because we rotated 90 degrees
    double scale_w = find_scale_factor(POLY_H,(POLY_H/2.0) - (THICKNESS));//fit between the S bottom and center
    double scale_h = find_scale_factor(POLY_W,THICKNESS);


    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);

    //rotate to make vertical rectangle, rotating around the bottom left corner
    double center[2];
    center[0] = BUF_DIST;
    center[1] = BUF_DIST;
    rotate(center,p,90);

    //translate to top of S
    double delta_x = BUF_DIST + ((POLY_W/2.0) - THICKNESS); //shift
    double delta_y = BUF_DIST + THICKNESS; //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

//top left side
int side_S_2(double p[]){



    //create scale factor for mama-shape to desired shape
    //H and W are flipped, because we rotated 90 degrees
    double scale_w = find_scale_factor(POLY_H,(POLY_H/2.0) - THICKNESS);
    double scale_h = find_scale_factor(POLY_W,THICKNESS);
    
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);


    //rotate to make vertical rectangle, rotating around the bottom left corner
    double center[2];
    center[0] = BUF_DIST;
    center[1] = BUF_DIST;
    rotate(center,p,90);

    //translate to top of S
    double delta_x = BUF_DIST;
    double delta_y = BUF_DIST + (POLY_H*(.5)); //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

int top_J(double p[]){

    //create scale factor for mama-shape to desired shape
    double scale_w = find_scale_factor(POLY_W,POLY_W/2.0);
    double scale_h = find_scale_factor(POLY_H,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);
    //translate to top of J
    double delta_x = BUF_DIST + POLY_W*(.5);
    double delta_y = BUF_DIST + (POLY_H - THICKNESS); //start at bottom left
    translate(p,delta_x,delta_y);

    return 0; 
}

int middle_J(double p[]){

    //create scale factor for mama-shape to desired shape
    //H and W are flipped, because we rotated 90 degrees
    double scale_w = find_scale_factor(POLY_H,POLY_H-THICKNESS);
    double scale_h = find_scale_factor(POLY_W,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);

    //rotate to make vertical rectangle, rotating around the bottom left corner
    double center[2];
    center[0] = BUF_DIST;
    center[1] = BUF_DIST;
    rotate(center,p,90);

    //translate to top of S
    double delta_x = BUF_DIST + (POLY_W*(.75));
    double delta_y = BUF_DIST; //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

int bottom_J(double p[]){

    //create scale factor for mama-shape to desired shape
    //H and W are flipped, because we rotated 90 degrees
    double scale_w = find_scale_factor(POLY_W,POLY_W*(.25));
    double scale_h = find_scale_factor(POLY_H,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);

    double delta_x = BUF_DIST + (POLY_W*(.5)) + THICKNESS;
    double delta_y = BUF_DIST; //start at bottom left
    translate(p,delta_x,delta_y);

    return 0;
}

int hook_j(double p[]){

    //create scale factor for mama-shape to desired shape
    //H and W are flipped, because we rotated 90 degrees
    double scale_w = find_scale_factor(POLY_H,POLY_H*(.3));
    double scale_h = find_scale_factor(POLY_W,THICKNESS);
    //scale down
    scale_x(p,scale_w);
    scale_y(p,scale_h);


    //rotate to make vertical rectangle, rotating around the bottom left corner
    double center[2];
    center[0] = BUF_DIST;
    center[1] = BUF_DIST;
    rotate(center,p,90);


    //translate to top of S
    double delta_x = BUF_DIST + (POLY_W*(.5));
    double delta_y = BUF_DIST + THICKNESS; //start at bottom left
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
    p[1] = 100; // starting y

    //double n_iters = ITERS;
    srand(time(NULL));
    int choice = 0;
    for(int i=0; i< ITERS; ++i){
        choice = rand() % 9 ;
        if(choice == 0) {G_rgb (0,1,0); bottom_S(p);}
        if(choice == 1) {G_rgb (0,1,0); middle_S(p);}
        if(choice == 2) {G_rgb (0,1,0); top_S(p);}
        if(choice == 3) {G_rgb (0,1,0); side_S_1(p);}
        if(choice == 4) {G_rgb (0,1,0); side_S_2(p);}
        if(choice == 5) {G_rgb (1,0,0); top_J(p);}
        if(choice == 6) {G_rgb (1,0,0); middle_J(p);}
        if(choice == 7) {G_rgb (1,0,0); bottom_J(p);}
        if(choice == 8) {G_rgb (1,0,0); hook_j(p);}


        G_point (p[0], p[1]);
    }

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}
