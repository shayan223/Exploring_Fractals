

#include  "FPToolkit.c"
#include <math.h>

// must do this before you do 'almost' any other graphical tasks 
int swidth = 800;
int sheight = 800;

double ANGLE_START = 90;//quarter turn
double ANGLE_INTERVAL = 119;//22.5; 
double FORWARD_DISTANCE = 6;
double PROD_LEN = 10;
double DEPTH = 6;
long long BUFFER_SIZE = 500000;
double MIN_X,MIN_Y,MAX_X,MAX_Y;
enum { STACK_SIZE = 100000 };
double STACK_X [STACK_SIZE]; 
double STACK_Y [STACK_SIZE];
double STACK_A [STACK_SIZE]; //stack for angle
int TOP_X = 0, TOP_Y = 0, TOP_A = 0;


double parametric(double x0, double x1, double t, double power){
    //default power should be 1
    return x0 + pow(t,power)*(x1 - x0);
}

void draw_background(double c_start [], double c_end []){
    
    
    double r,g,b;
    double r_pow = 1;
    double g_pow = 1;
    double b_pow = 1;
    double cur_height;

    double sand [] = {168/255.0,143/255.0,89/255.0};//sand color
    double sun [] = {254/255.0,76/255.0,64/255.0};//sand color

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
    G_fill_circle (100, 400, 75) ;


    for (double t = 0.0; t <= 1.0; t+=0.001){
        //draw bottom half of horizon
        G_rgb(sand[0],sand[1],sand[2]);
        cur_height = parametric(0,sheight/2,t,1);
        G_line(0, cur_height, swidth, cur_height);
    }
}

struct production{
    //Note for axiom: We hold the string "B" NOT the character 'B'
    char axiom [100];
    char rule [100];
    char var;
};

int grammar2(struct production prd[]){

    /*
        Creates the following Grammar:
        Axiom: A
        Rule 1 : A -> F-[[A]+A]+F[+FA]-A
        Rule 2 : F -> FF
    */

    //one production per "rule" in the grammar
    //first production will ONLY have axiom

    strcpy(prd[0].axiom, "A");

    //Rule 1:
    prd[1].var = 'A';
    strcpy(prd[1].rule, "F-[[A]+A]+F[+FA]-A");

    //Rule 2:
    prd[2].var = 'F';
    strcpy(prd[2].rule, "FF");


    //record length of rules in global (axiom plus 1 for each rule)
    PROD_LEN = 3;

    //returns index of last rule (ie. number of rules + 1 for axiom)
    return 2;

}


//Implementation of stack, top points to top free space
void push(double stack [], int *top, double val){

    if(*top < STACK_SIZE - 1){
        stack[*top] = val;
        *top += 1;
    }
    else{
        printf("PUSH FAILED: max stack height exceeded");
    }

}
double pop(double stack [], int *top){
    double val;
    if(*top > 0){
        val = stack[(*top) - 1];
        *top -= 1;
        return val;
    }
    else{
        printf("POP FAILED: nothing in stack");
        return -1;
    }
}

//finds index of a given rule, -1 if terminal
int find_rule(char prod_rule, struct production prd[]){
    for(int i = 1; i < PROD_LEN; ++i){
        //if rule is matched
        //if(strcmp(prd[i].var,prod_rule) == 0){
        if(prd[i].var == prod_rule){
            return i;
        }
    }
    //if none are found, return -1
    return -1;
}


//generates a string of the grammar expanded to the given depth, represents our grammar
//Expands Axiom to a certain depth.
int string_builder(char instructions [], int depth, struct production prd[]){
    int iter = 0;
    char temp[BUFFER_SIZE];
    char cur_rule[100];
    //string starts with axiom at depth 0
    strcpy(instructions,prd[0].axiom);

    //at every depth level, loop through the string and expand it
    while(iter < depth){
        //reset temp at every loop
        strcpy(temp, "\0");
        int rule_index = 0;
        int inst_len = strlen(instructions);

        printf("Instruction length: %i Current Depth: %i \n ", inst_len, iter);

        for (int i = 0; i < inst_len; ++i)
        {
            //lookup and concat expanded terms
            rule_index = find_rule(instructions[i],prd);
            if(rule_index >= 0){
                strcat(temp,prd[rule_index].rule);
            }
            //simply copy terminals directly
            else{
                //DON'T USE THIS: it takes the rest of the string after i
                //strcat(temp,&instructions[i])

                //Use one of these:
                //char temp_inst = instructions[i];
                //strcat(temp,&temp_inst);
                strncat(temp,&instructions[i],1);
            }
        }

        //copy over expanded version of string back to its original array
        strcpy(instructions,temp);

        iter += 1;
    }
    return 0;

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



double turtle_walk(double p [],double angle, char inst){
    G_rgb(0,1,0);
    //holds on to original p values
    double p_temp[2];
    p_temp[0] = p[0];
    p_temp[1] = p[1];

    //defaults non terminal variables to the terminal 'f'
    if(inst == 'f' || inst >= 'A' && inst <='Z'){
        //move in the x direction then rotate to the correct position
        p[0] = p_temp[0] + FORWARD_DISTANCE;
        rotate_around_center(p_temp,p,angle);
        G_line(p_temp[0],p_temp[1],p[0],p[1]);
    }
    else if(inst == '['){//inst to push state onto stack
        push(STACK_X,&TOP_X,p[0]);
        push(STACK_Y,&TOP_Y,p[1]);
        push(STACK_A,&TOP_A,angle);
    }
    else if(inst == ']'){//inst to pop state from stack
        p[0] = pop(STACK_X,&TOP_X);
        p[1] = pop(STACK_Y,&TOP_Y);
        angle = pop(STACK_A,&TOP_A);
    }
    else if(inst == '+'){
        angle += ANGLE_INTERVAL;
    }
    else if(inst == '-'){
        angle -= ANGLE_INTERVAL;
    }
    

    return angle;

}


int autoplacer(double p[],char instructions[],double swidth,double sheight){
    double starting_px, starting_py;
    double angle = ANGLE_START;
    starting_px = p[0];
    starting_py = p[1];
    int inst_len = strlen(instructions);
    //do first run through
    for(int i = 0; i < inst_len; ++i){
        angle = turtle_walk(p,angle,instructions[i]);
        //update min/max location values
        if(p[0] > MAX_X){
            MAX_X = p[0];
        }
        if(p[0] < MIN_X){
            MIN_X = p[0];
        }
        if(p[1] > MAX_Y){
            MAX_Y = p[1];
        }
        if(p[1] < MIN_Y){
            MIN_Y = p[1];
        }
    }
    // clear the screen
    G_rgb (0.3, 0.3, 0.3) ; // dark gray
    G_clear () ;
    //reset angle
    angle = ANGLE_START;
    //update and scale movement length and starting position
    double delta_x, delta_y, scale_factor;
    delta_x = MAX_X - MIN_X;
    delta_y = MAX_Y - MIN_Y;

    if(delta_x > delta_y){
        scale_factor = swidth / delta_x;
    }
    else{
        scale_factor = sheight / delta_y;
    }
    //scale it down slightly smaller than the screen
    scale_factor = scale_factor*0.9;
    FORWARD_DISTANCE = FORWARD_DISTANCE*scale_factor;

    //shift starting point to center drawing

    double center_x,center_y;
    //find the centroid for the initial shape
    center_x = (MAX_X + MIN_X) / 2.0;
    center_y = (MAX_Y + MIN_Y) / 2.0;

    //adjust starting point to center the centroid of the resulting shape
    p[0] = (starting_px + (swidth/2 - center_x));//*scale_factor;
    p[1] = (starting_py + (sheight/2 - center_y));//*scale_factor;
    printf("\n New starting position: (%f,%f)\n", p[0],p[1]);
    
    //p[0] = 0 + p[0]*scale_factor;
    //p[1] = 0 + p[1]*scale_factor;
    printf("New starting position scaled (factor: %f): (%f,%f)\n", scale_factor,p[0],p[1]);

    //temporary while autoplacer is broken
    p[0] = 400;
    p[1] = 50;
    //redraw
    for(int i = 0; i < inst_len; ++i){
        angle = turtle_walk(p,angle,instructions[i]);
    }

}

void basic_walk(double p[],char instructions[]){
    p[0] = 600;
    p[1] = 125;
    double angle = ANGLE_START;
    int inst_len = strlen(instructions);

    //redraw
    for(int i = 0; i < inst_len; ++i){
        angle = turtle_walk(p,angle,instructions[i]);
    }
}

int main()
{

    G_init_graphics (swidth,sheight) ;  // interactive graphics

    
    // clear the screen in a given color
    G_rgb (0.3, 0.3, 0.3) ; // dark gray
    G_clear () ;

    double p [2];
    p[0] = 100; // turtle starting x
    p[1] = 10; // turtle starting y
    double angle = ANGLE_START; //turtle starting angle (horizontal to the right)

    //char instructions[1000] = "f+f+ff-f";
    //code to read in instructions
    char instructions[BUFFER_SIZE];
    //scanf("%s",instructions);

    //Things to keep in mind for later use:
    /*
        char u[1000000];
        char v[1000000];
        v[0] = '\0';
        u[0] = '\0';
        strcpy(u, "dog");
        strcpy(v, "pig");
        strcat(u,v); // results in "dogpig"
    */

    struct production prods[10];
    //populate production rules based on grammar
    grammar2(prods);
    //construct string by expanding grammar to a given depth
    string_builder(instructions,DEPTH,prods);

    MIN_X = p[0];
    MAX_X = p[0];

    MIN_Y = p[1];
    MAX_Y = p[1];

    double sunset_start [] = {255/255.0, 255/255.0, 76/255.0}; 
    double sunset_end [] = {136/255.0, 60/255.0, 119/255.0};

    int key = 0;
    while(key != 113){
    if(key == 106){//rotate left with 'j' key
        ANGLE_INTERVAL += 1;
    }
    else if(key == 107){//rotate right with 'k' key
        ANGLE_INTERVAL -= 1;
    }
    else{//default movement to the left
        ANGLE_INTERVAL += 1;
    }
    //autoplacer(p,instructions,swidth,sheight);
    //G_rgb (0.3, 0.3, 0.3) ; // dark gray
    //G_clear () ;
    draw_background(sunset_end,sunset_start);

    G_rgb(0,1,0);
    basic_walk(p,instructions);
    printf("Current Angle Interval: %f\n", ANGLE_INTERVAL);
    key =  G_wait_key() ; // pause so user can see results

    }

    return 0;
}
