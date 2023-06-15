


#include  "FPToolkit.c"
#include <math.h>


/* Based on the following paper: https://flam3.com/flame_draves.pdf 
    with additional info here: https://en.wikipedia.org/wiki/Fractal_flame */

//double SQ_SIZE = 600; //initial square size
double POLY_W = 500;
double POLY_H = 400;
double THICKNESS = 50;
double BUF_DIST = 50.0;
double ITERS = 6000000;

double PIXEL_VALUES [800][800] = {0};
int HISTOGRAM_FREQ [2400][2400] = {0};
double HISTOGRAM_COLOR [2400][2400][3] = {0.0};

int HISTOGRAM_DIM = 2400;
//used for float rounding and comparison
double ERROR_TOLERANCE = 0.000001
double COLOR_GAMMA = 3.0 //must be greater than 1
//viewport bounds for drawing points
int MIN_X = -2;
int MIN_Y = -2;
int MAX_X = 2;
int MAX_Y = 2;


//probabilities for the respectively numbered variation functions
double P0 = 0.1;
double P1 = 0.2;
double P2 = 0.3;
double P3 = 0.2;
double P4 = 0.2;

int double_compare(double a, double b){
    double compare = fabs(a - b);
    if(compare < ERROR_TOLERANCE && compare > 0){
        return 0; //return 0 if they are equal
    }
    else if(compare > ERROR_TOLERANCE){// a > b
        return 1;
    }
    else{
        return -1; //otherwise a < b
    }
}

double parametric(double x0, double x1, double t, double power){
    //default power should be 1
    return x0 + pow(t,power)*(x1 - x0);
}

//defined here for consistancy with source paper
double r(double p []){ //sqrt(x^2 + y^2)
    return sqrt(pow(p[0],2)+pow(p[1],2));
}

int v0(double p []){//linear
    return 1;
}

int v1(double p []){//sinusoidal
    p[0] = sin[p[0]];
    p[1] = sin[p[1]];

    return 1;
}

int v2(double p []){//spherical
    double frac = (1/pow(r(p),2))
    p[0] = frac*p[0];
    p[1] = frac*p[1];

    return 1;
}


int v3(double p []){//swirl
    double x = p[0];
    double y = p[1];
    double p_temp [] = {x,y};

    p[0] = (x*sin(pow(r(p_temp),2))) - (y*cos(pow(r(p_temp,2)))); //xsin(r^2) - ycos(r^2)
    p[1] = (x*cos(pow(r(p_temp),2))) + (y*sin(pow(r(p_temp,2)))); //xcos(r^2) + ysin(r^2)
}

int v4(double p []){//horshoe
    double frac = 1/r(p);
    double x = p[0];
    double y = p[1];
    p[0] = frac*((x - y)*(x + y));
    p[1] = frac*(2*x*y);

    return 1;
}



void set_color(double cur_color [], double new_color []){
    cur_color[0] = new_color[0];
    cur_color[1] = new_color[1];
    cur_color[2] = new_color[2];
}

int main()
{
    int    swidth, sheight ;
    
    //Because doubles are not discrete, we have to divide into a grid of potential x,y values to 
    //fit everything into discrete locations in our histogram array
    double x_interval = (fabs(MAX_X - MIN_X))/(double)HISTOGRAM_DIM;
    double y_interval = (fabs(MAX_X - MIN_X))/(double)HISTOGRAM_DIM;

    // must do this before you do 'almost' any other graphical tasks 
    swidth = 800 ;  sheight = 800 ;
    G_init_graphics (swidth,sheight) ;  // interactive graphics

    
    // clear the screen in a given color
    G_rgb (0, 0, 0) ; // dark gray
    G_clear () ;
    G_rgb (0,1,0);
    double p [2];
    //start within a unit square between -1 and 1
    p[0] = -.75; // starting x
    p[1] = .5; // starting y

    //double n_iters = ITERS;
    srand(time(NULL));
    double choice = 0;
    int max_frequency = 0;
    int cur_x = 0;
    int cur_y = 0;
    int color = 0;

    double dark_orange [] = {255/255.0, 140/255.0, 0/255.0};
    double light_blue [] = {173/255.0, 216/255.0, 230/255.0};
    double violet [] = {238/255.0, 130/255.0, 238/255.0};
    double light_green [] = {144/255.0, 238/255.0, 144/255.0};
    double crimson [] = {220/255.0, 20/255.0, 60/255.0};

    double cur_color [] = {0/255.0, 0/255.0, 0/255.0};

    for(int i=0; i< ITERS; ++i){
        choice = (double)rand() / RAND_MAX ;
        //note, this is a trick to pick each based on predifined probabilities, they MUST add up to 1
        if(choice < P0) { set_color(cur_color,dark_orange); v0(p); }
        if(choice < P0 + P1) { set_color(cur_color,light_blue); v1(p); }
        if(choice < P0 + P1 + P2) { set_color(cur_color,violet); v2(p); }
        if(choice < P0 + P1 + P2 + P3) { set_color(cur_color,light_green); v3(p); }
        if(choice < P0 + P1 + P2 + P3 + P4) { set_color(cur_color,crimson); v4(p); }

        //TODO: Should I be changing/rounding p here for the next iteration??

        //discretize the point to match up with the histogram
        cur_x = round(((p[0] - MIN_X)/x_interval));//round from float to nearest interval bin
        cur_y = round(((p[1] - MIN_Y)/y_interval));
        //Add to histogram
        HISTOGRAM_FREQ[cur_y][cur_x] += 1;//we switch x and y because c is [row][column]
        //update max frequency
        if(HISTOGRAM_FREQ[cur_y][cur_x] > max_frequency){
            max_frequency = HISTOGRAM_FREQ[cur_y][cur_x];
        }

        //update color for the given coordinate: the average of the newest addition and what it used to be
        HISTOGRAM_COLOR[cur_y][cur_x][0] = (HISTOGRAM_COLOR[cur_y][cur_x][0] + cur_color[0])/2.0;
        HISTOGRAM_COLOR[cur_y][cur_x][1] = (HISTOGRAM_COLOR[cur_y][cur_x][1] + cur_color[1])/2.0;
        HISTOGRAM_COLOR[cur_y][cur_x][2] = (HISTOGRAM_COLOR[cur_y][cur_x][2] + cur_color[2])/2.0;
        
    }

    double alpha = 0.0;
    double gamma = COLOR_GAMMA;
    double avg_freq = 0.0;
    double avg_color [] = {0.0, 0.0, 0.0}; 
    
    //Render the image using the histogram and color buffer
    //to accomplish super-sampling, we use every 3x3 buffered grid as a pixel
    for(int i=0; i < HISTOGRAM_DIM; i+=3){
        for(int j=0; j < HISTOGRAM_DIM; j+=3){
            //average together the 3x3 grid of values
            avg_freq = HISTOGRAM_FREQ[i][j] + HISTOGRAM_FREQ[i][j+1] + HISTOGRAM_FREQ[i][j+2]
            + HISTOGRAM_FREQ[i+1][j] + HISTOGRAM_FREQ[i+1][j+1] + HISTOGRAM_FREQ[i+1][j+2] 
            + HISTOGRAM_FREQ[i+2][j] + HISTOGRAM_FREQ[i+2][j+1] + HISTOGRAM_FREQ[i+2][j+2]
            avg_freq = avg_freq / 9.0;
            
            //same for the color
            for(int channel = 0; channel < 3; ++channel){
                avg_color[channel] = HISTOGRAM_COLOR[i][j][channel] + HISTOGRAM_COLOR[i][j+1][channel] + HISTOGRAM_COLOR[i][j+2][channel]
                + HISTOGRAM_COLOR[i+1][j][channel] + HISTOGRAM_COLOR[i+1][j+1][channel] + HISTOGRAM_COLOR[i+1][j+2][channel]
                + HISTOGRAM_COLOR[i+2][j][channel] + HISTOGRAM_COLOR[i+2][j+1][channel] + HISTOGRAM_COLOR[i+2][j+2][channel]
                avg_color[channel] = avg_color[channel] / 9.0;
            }
            

            //use log values to get a better visual due to vastly varying values
            alpha = log(avg_freq)/log(max_frequency);

            //update color with alpha and gamma values
            avg_color[0] = avg_color[0] * pow(alpha,1/gamma);
            avg_color[1] = avg_color[1] * pow(alpha,1/gamma);
            avg_color[2] = avg_color[2] * pow(alpha,1/gamma);

            //Render computed pixel
            G_rgb(avg_color[0],avg_color[1],avg_color[2]);
            G_point(i+1,j+1);
            
        }
    }

    int key ;   
    key =  G_wait_key() ; // pause so user can see results
    G_save_to_bmp_file("fractal_flame.bmp");

    return 0;
}