


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

int angled_box(double center [], double width, double angle)
{
    double p1[2],p2[2],p3[2],p4[2];

    // create first square point (bottom left)
    p1[0] = center[0] - (width/2);
    p1[1] = center[1] - (width/2);

    // second point, to make the first line
    p2[0] = center[0] + (width/2);
    p2[1] = center[1] - (width/2);

    // third point, top right
    p3[0] = center[0] + (width/2);
    p3[1] = center[1] + (width/2);

    // fourth point, top left
    p4[0] = center[0] - (width/2);
    p4[1] = center[1] + (width/2);
    
    rotate_around_center(center,p1,angle);
    rotate_around_center(center,p2,angle);
    rotate_around_center(center,p3,angle);
    rotate_around_center(center,p4,angle);

    //Draw square
    G_line(p1[0],p1[1],p2[0],p2[1]);
    G_line(p2[0],p2[1],p3[0],p3[1]);
    G_line(p3[0],p3[1],p4[0],p4[1]);
    G_line(p4[0],p4[1],p1[0],p1[1]);

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


    G_wait_click(p) ;
    G_fill_circle(p[0],p[1],2) ;

    //function params: center, width, angle
    angled_box(p,100,45);

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}




