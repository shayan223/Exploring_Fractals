
#include  "FPToolkit.c"
#include <math.h>


int point_along_line(double x0, double y0, double x1, double y1, double t, double point []){

    double y_val, x_val;

    x_val = x0 + t*(x1 - x0);
    y_val = y0 + t*(y1 - y0);

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

int make_polygon(double start_x, double start_y, double width, double height){

    G_rgb(0,1,0);
    double squarex[4];
    double squarey[4];
    double p3[2]; 
    double p4[2];

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
    squarey[3] = start_y + height;
    //Polygon_X(squarex,squarey,4);

    return 1;
}

int pythag_triangle(double start_x, double start_y, int max_depth, double triangle_center, double width, double height){
    G_rgb(0,1,0);
    //left point of triangle
    double p0[2];
    //middle point
    double p1[2];
    //right point
    double p2[2];

    //make initial box
    //make_polygon(start_x,start_y,box_width,box_height);
    G_line(start_x,start_y,start_x+width,start_y);
    G_line(start_x+width,start_y,start_x+width,start_y+height);
    G_line(start_x+width,start_y+height,start_x,start_y + height);
    G_line(start_x,start_y + height,start_x,start_y);


    //define triangle points to start
    p0[0] = start_x;
    p0[1] = start_y + height;

    p2[0] = start_x + width;
    p2[1] = start_y + height;

    //Compute middle point at base of triangle
    double m[2];
    point_along_line(p0[0],p0[1],p2[0],p2[1],triangle_center,m);
    //hold on to length of line segments
    double d, d1, d2;
    d = sqrt(pow((p2[0] - p0[0]),2) + pow(p2[1] - p0[1],2)); //p0 to p2
    d1 = sqrt(pow((m[0] - p0[0]),2) + pow(m[1] - p0[1],2)); // p0 to m between them
    d2 = sqrt(pow((p2[0] - m[0]),2) + pow(p2[1] - m[1],2)); // m to p2

    //find the height of the triangle that maintains a right angle
    double h;
    h = sqrt(d1*d2);

    //compute scale factor for right angle movement
    double scale_factor = (h/d);

    //find the right angle point from m
    double delta_x, delta_y;
    delta_x = p2[0] - p0[0];
    delta_y = p2[1] - p0[1];

    //find the floating right angle point
    double r [2];
    r[0] = m[0] + (-1*delta_y);
    r[1] = m[1] + delta_x;

    //scale it back down to where we need it, storing it in p1
    point_along_line(r[0],r[1],m[0],m[1],scale_factor,p1);

    //draw triangle
    G_triangle(p0[0],p0[1],p1[0],p1[1],p2[0],p2[1]);


    //pythag_recursive(start_x,start_y,max_depth,1,triangle_center, box_width, box_height);
    return 1;
}

int pythag_recursive(double base0[], double base1[], int max_depth, int cur_depth, double triangle_center, double box_width, double box_height){
    if(cur_depth <= max_depth){
        //create triangle, with base across the width of the square

        //right angle movement to get the new p0 and p2
        
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

    int depth = 2;
    double triangle_center = 0.5 ;
    double box_width = 20;
    double box_height = 20;
    pythag_triangle(400,10,depth,triangle_center,box_width,box_height);

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}




