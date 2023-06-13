
#include  "FPToolkit.c"
#include <math.h>


int swidth = 800;
int sheight  = 800;
int RECORDING_LENGTH = 200;


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


int pythag_triangle(double x1, double y1, double x2, double y2, int max_depth, int cur_depth, double triangle_center, double width, double height){

    if(cur_depth <= max_depth){
        //draw right anle movement from the base line
        double x3,y3, x4,y4, delta_ax, delta_ay;

        delta_ax = x2 - x1;
        delta_ay = y2 - y1;

        x3 = x1 - delta_ay;
        y3 = y1 + delta_ax;

        x4 = x2 - delta_ay;
        y4 = y2 + delta_ax;

        //make initial box

        G_line(x1,y1,x2,y2);
        G_line(x2,y2,x4,y4);
        G_line(x4,y4,x3,y3);
        G_line(x3,y3,x1,y1);

        //left point of triangle
        double p0[2];
        //middle point
        double p1[2];
        //right point
        double p2[2];


        //define triangle points to start
        p0[0] = x3;
        p0[1] = y3;

        p2[0] = x4;
        p2[1] = y4;

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

        //children draw between each trangle point (left and right of the triangles outward sides)
        pythag_triangle(p0[0],p0[1],p1[0],p1[1],max_depth,cur_depth+1,triangle_center,width,height);
        pythag_triangle(p1[0],p1[1],p2[0],p2[1],max_depth,cur_depth+1,triangle_center,width,height);
    }

    return 1;
}

double parametric(double x0, double x1, double t, double power){
    return x0 + pow(t,power)*(x1 - x0);
}

void draw_background(double c_start [], double c_end []){
    
    
    double r,g,b;
    double r_pow = 1;
    double g_pow = 1;
    double b_pow = 1;
    double cur_height;

    //Color for grass background
    double grass [] = {0/255.0,154/255.0,23/255.0};
    //generic sun yellow
    double sun [] = {249/255.0,215/255.0,28/255.0};

    for (double t = 0.0; t <= 1.0; t+=0.001){
        r = parametric(c_start[0],c_end[0],t,r_pow);
        g = parametric(c_start[1],c_end[1],t,g_pow);
        b = parametric(c_start[2],c_end[2],t,b_pow);
        G_rgb(r,g,b);
        //draws top half of horizon
        cur_height = sheight/2 + parametric(0,sheight/2,t,1);
        G_line(0, cur_height, swidth, cur_height);

    }
    //Place sun
    G_rgb(sun[0],sun[1],sun[2]);
    G_fill_circle (100, 600, 75) ;


    for (double t = 0.0; t <= 1.0; t+=0.001){
        //draw bottom half of horizon
        G_rgb(grass[0],grass[1],grass[2]);
        cur_height = parametric(0,sheight/2,t,1);
        G_line(0, cur_height, swidth, cur_height);
    }
}

int main()
{

    double lowleftx, lowlefty, width, height ;
    double x[10], y[10] ;
    double numxy ;
    double numab ;

    
    // must do this before you do 'almost' any other graphical tasks 
    
    G_init_graphics (swidth,sheight) ;  // interactive graphics

    
    // clear the screen in a given color
    G_rgb (0.3, 0.3, 0.3) ; // dark gray
    G_clear () ;

    int startingx = 350;
    int startingy = 250;
    double sky [] = {135/255.0,206/255.0,235/255.0};


    //Color blending variables
    //starting color (dark green)
    double sr = 5/255.0;
    double sg = 102/255.0;
    double sb = 8/255.0;

    //ending color (wood brown)
    double er = 139/255.0;
    double eg = 69/255.0;
    double eb = 19/255.0;
    
    double r,g,b;
    double r_pow = 1; //3
    double g_pow = 1; //2
    double b_pow = 1; //1

    double color_scaling = 0;

    int depth = 10;
    double triangle_center = 0.4 ;
    double box_width = 100;
    double box_height = 20;

   
    int key = 0 ;
    double interval = 0.005;
    double angle_min = 0.3;
    double angle_max = 0.7;
    double angle = angle_min;
    int direction = 0;

    char fname[100];//holds on to animation frames
    int frames = 0;
    //Press and hold key to run, q to quit
    while(frames < RECORDING_LENGTH)
    {
        //determine color for this iteration

        draw_background(sky,sky);

        color_scaling = (angle - angle_min) / (angle_max - angle_min);

        r = parametric(sr,er,color_scaling,r_pow);
        g = parametric(sg,eg,color_scaling,g_pow);
        b = parametric(sb,eb,color_scaling,b_pow);
        G_rgb(r,g,b);
        //determine direction of change and adjust the angle, ocilating between min and max.
        if(angle < (angle_max + interval) && direction == 0){
            angle += interval;
            //switch directions if threshold is exceeded after interval step
            if(angle >= angle_max){
                direction = 1;
            }
        }
        else if(angle > (angle_min - interval) && direction == 1){
            angle -= interval;
            if(angle <= angle_min){
                direction = 0;
            }
        }

        pythag_triangle(startingx,startingy,startingx+box_width,startingy,depth,1,angle,box_width,box_height);
        //key =  G_wait_key() ; // pause so user can see results
        //generate background for next frame
        
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


    return 0;
}




