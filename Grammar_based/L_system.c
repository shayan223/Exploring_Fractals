

#include  "FPToolkit.c"
#include <math.h>

double ANGLE_INTERVAL = 90; //quarter turn
double FORWARD_DISTANCE = 50;

double turtle_walk(double p [],double angle, char inst){
    G_rgb(0,0,1);
    //holds on to original p values
    double p_temp[2];
    p_temp[0] = p[0];
    p_temp[1] = p[1];

    if(inst == 'f'){
        //move in the x direction then rotate to the correct position
        p[0] = p_temp[0] + FORWARD_DISTANCE;
        rotate_around_center(p_temp,p,angle);
        G_line(p_temp[0],p_temp[1],p[0],p[1]);
    }
    else if(inst == '+'){
        angle += ANGLE_INTERVAL;
    }
    else if(inst == '-'){
        angle -= ANGLE_INTERVAL;
    }


    return angle;

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
    p[0] = 200; // turtle starting x
    p[1] = 200; // turtle starting y
    double angle = 0; //turtle starting angle (horizontal to the right)

    char instructions[1000] = "f+f+ff-f";
    for(int i = 0; i < strlen(instructions); ++i){
        angle = turtle_walk(p,angle,instructions[i]);
    }


    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}
