
#include  "FPToolkit.c"
#include <math.h>


int point_along_line(double p[], double q[], double point [], double t){
    double ax, ay, bx, by;
    double y_val, x_val;
    ax = p[0];
    ay = p[1];
    bx = q[0];
    by = q[1];

    x_val = (1 - t)*ax + t*bx;
    y_val = (1 - t)*ay + t*by;

    point[0] = x_val;
    point[1] = y_val;

    return 1;
}

int rotate_around_center(double center [], double p [], double angle){

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

int make_polygon(double p1 [], double p2 [], double width, double height){

    G_rgb(0,1,0);
    double squarex[4],squarey[4], p3[2], p4[2];

    //p1 and p2 are the base points, so the remaining 2 points for the box
    // form a parallel line of the same width, height pixels away.

    //point height pixels away from p1, rotated to be perpendicular
    //p4[0] = p1[0];
    //p4[1] = p1[1] + height;

    //find clockwise rotation angle
    //double angle = 0;


    //points clockwise from bottom left
    squarex[0] = start_x;
    squarex[1] = start_x + width;
    squarex[2] = start_x + width;
    squarex[3] = start_x;

    squarey[0] = start_y;
    squarey[1] = start_y;
    squarey[2] = start_y + height;
    aquarey[3] = start_y + height;
    Polygon_X(squarex,squarey,4)

    return 1;
}

int pythag_triangle(double start_x, double start_y, int max_depth, double triangle_center, double box_width, double box_height){
    G_rgb(0,1,0);
    //make initial box
    make_polygon(start_x,start_y,box_width,box_width, box_height)

    pythag_recursive(start_x,start_y,max_depth,1,triangle_center, box_width, box_height);
    return 1;
}

int pythag_recursive(double start_x, double start_y, int max_depth, int cur_depth, double triangle_center, double box_width, double box_height){
    if(cur_depth <= max_depth){
        //create triangle, with base across the width of the square
    }

    return 1;
}

int main()
{
    int    swidth, sheight ;
    double lowleftx, lowlefty, width, height ;
    double x[10], y[10] ;
    double numxy ;
    double numab ;

    
    // must do this before you do 'almost' any other graphical tasks 
    swidth = 800 ;  sheight = 800 ;
    G_init_graphics (swidth,sheight) ;  // interactive graphics

    
    // clear the screen in a given color
    G_rgb (0.3, 0.3, 0.3) ; // dark gray
    G_clear () ;


    // ############### Draw graph lines ##############
    //set color
    G_rgb (1,1,1);
    double n_lines = 4;
    double intervalH = sheight / n_lines;
    double intervalW = swidth / n_lines;
    // Create graph paper lines
    for (int i = intervalH; i < sheight; i += intervalH)
    {
        G_line(0,i,swidth-1,i);
    }

    for (int i = intervalW; i < swidth; i += intervalW)
    {
        G_line(i,0,i,swidth-1);
    }


    p[0] = 5;
    p[1] = 5;
    int depth = 2;
    double triangle_center = 0.5 ;
    double box_width = 20;
    double box_height = 20;
    pythag_triangle(400,10,depth,triangle_center,box_width,box_height);

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}




