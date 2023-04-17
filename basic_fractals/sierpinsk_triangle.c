


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

int make_triangle(double bottom_left [], double width, double height, double angle)
{
    double bl[2], bottom_right[2], top_point[2], center[2];
    
    bl[0] = bottom_left[0];
    bl[1] = bottom_left[1];
    bottom_right[0] = bl[0] + width;
    bottom_right[1] = bl[1];

    //top of triangle x value is average of the base points (middle)
    top_point[0] = (bl[0] + bottom_right[0]) / 2.0;
    top_point[1] = bl[1] + height;

    //apply rotation if needed
    if(angle > 0)
    {
        //Compute center of triangle (average of all x/y points is the centroid)
        center[0] = (bl[0] + bottom_right[0] + top_point[0]) / 3.0;
        center[1] = (bl[1] + bottom_right[1] + top_point[1]) / 3.0;
        rotate_around_center(center,bl,angle);
        rotate_around_center(center,bottom_right,angle);
        rotate_around_center(center,top_point,angle);
    }

    G_line(bl[0],bl[1],bottom_right[0],bottom_right[1]);
    G_line(bottom_right[0],bottom_right[1],top_point[0],top_point[1]);
    G_line(top_point[0],top_point[1],bl[0],bl[1]);

    return 1;
    
}

int make_eq_triangle(double bottom_left [], double side_len){
    double p1 [2];
    double p2 [2];
    double p3 [2];

    p1[0] = bottom_left[0];
    p1[1] = bottom_left[1];

    p2[0] = p1[0]+side_len;
    p2[1] = p1[1];

    p3[0] = p1[0] + (side_len / 2);
    p3[1] = (.5)*(sqrt(3)*side_len);

    G_line(p1[0],p1[1],p2[0],p2[1]);
    G_line(p2[0],p2[1],p3[0],p3[1]);
    G_line(p3[0],p3[1],p1[0],p1[1]);

    return 1;
}

//recursive function for Sierpinsk Triangle
int SK_triangle(double p[], int total_depth, double size, int artsy){
    double temp_p[2];
    temp_p[0] = p[0];
    temp_p[1] = p[1];
    SK_recurse(temp_p[0],temp_p[1],total_depth,1,size,artsy);
    return 1;
}

int SK_recurse(double px, double py, int total_depth, int cur_depth, double size,int artsy){
    G_rgb(0,1,0);

    if(cur_depth <= total_depth){
        
        //create triangle
        double p[2];
        double top_point[2];
        double top_child_point[2];
        p[0] = px;
        p[1] = py;

        top_point[0] = px + (size / 2.0);
        top_point[1] = py+((.5)*(sqrt(3.0)*size));

        //make_eq_triangle(p,size);
        Triangle_X(px,py,px+size,py,top_point[0],top_point[1]);
        //recurse into 3 sub triangles, using bottom left of each child
        //compute center of two points to start next triangle
        
        //bottom left child
        SK_recurse(px,py,total_depth,cur_depth+1,size/2.0,artsy);
        //bottom right child
        SK_recurse(px+(size/2),py,total_depth,cur_depth+1,size/2.0,artsy);
        //top child
        point_along_line(p,top_point,top_child_point,.5);
        SK_recurse(top_child_point[0],top_child_point[1],total_depth,cur_depth+1,size/2.0,artsy);

        
        //Fill in negative space
        G_rgb(0,0,1);
        if(artsy){
            // use parameterization to create color gradient
            double depth_ratio = (double)cur_depth/(double)total_depth;
            double color_val = (1 - depth_ratio)*0 + depth_ratio*1;
            G_rgb(0,depth_ratio,1-depth_ratio);
        }
        Fill_Triangle_X(px + (size / 2.0), py, 
                top_child_point[0],top_child_point[1],
                top_child_point[0]+(size/2.0), top_child_point[1]);

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

    // ######## Draw angled box at given center ##########
    G_rgb(0,1,0);
    double p[2];

    //G_wait_click(p) ; 
    p[0] = 5;
    p[1] = 5;
    int do_art = 1;   
    SK_triangle(p,6,750,do_art);

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}




