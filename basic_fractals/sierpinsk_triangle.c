


#include  "FPToolkit.c"
#include <math.h>



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

//recursive function for Sierpinsk Triangle
int SK_triangle(double p[], int total_depth, double size){
    double temp_p[2];
    temp_p[0] = p[0];
    temp_p[1] = p[1];
    SK_recurse(temp_p[0],temp_p[1],total_depth,0,size);
    return 1;
}

int SK_recurse(double px, double py, int total_depth, int cur_depth, double size){
    if(cur_depth <= total_depth){
        
        //create triangle
        double p[2];
        p[0] = px;
        p[1] = py;

        make_triangle(p,size,size,0);
        //recurse into 3 sub triangles, using bottom left of each child
        //compute center of two points to start next triangle
        
        //bottom left child
        SK_recurse(px,py,total_depth,cur_depth+1,size/2);
        //bottom right child
        SK_recurse(px+(px+size)/2,py,total_depth,cur_depth+1,size/2);
        //top child
        SK_recurse(px+(px+size)/4,py+(py+size)/2,total_depth,cur_depth+1,size/2);
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
    /*
    for(int i = 0; i < 3; i++)
    {
        G_wait_click(p) ;
        make_triangle(p,100,100,0);
    }

    G_wait_click(p) ;    
    make_triangle(p,100,100,45);
    */

    //G_wait_click(p) ; 
    p[0] = 10;
    p[1] = 5;   
    SK_triangle(p,4,750);

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}




