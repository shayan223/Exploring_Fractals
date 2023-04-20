

#include  "FPToolkit.c"
#include <math.h>

double parametric(double x0, double x1, double t, double power){
    return x0 + pow(t,power)*(x1 - x0);
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

    //starting color
    double sr = 128/255.0;
    double sg = 0/255.0;
    double sb = 128/255.0;

    //ending color
    double er = 64/255.0;
    double eg = 224/255.0;
    double eb = 208/255.0;
    
    double r,g,b;
    double r_pow = 3;
    double g_pow = 2;
    double b_pow = 1;
    double cur_height;

    for (double t = 0.0; t <= 1.0; t+=0.001){
        r = parametric(sr,er,t,r_pow);
        g = parametric(sg,eg,t,g_pow);
        b = parametric(sb,eb,t,b_pow);
        G_rgb(r,g,b);
        cur_height = parametric(0,sheight,t,1);
        G_line(0, cur_height, swidth, cur_height);
    }

    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}