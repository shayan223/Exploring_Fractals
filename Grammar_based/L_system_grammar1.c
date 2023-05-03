

#include  "FPToolkit.c"
#include <math.h>

double ANGLE_START = 0;
double ANGLE_INTERVAL = 30; //quarter turn
double FORWARD_DISTANCE = 1;
double PROD_LEN = 10;
double DEPTH = 10;
long long BUFFER_SIZE = 500000;
double MIN_X,MIN_Y,MAX_X,MAX_Y;

struct production{
    //Note for axiom: We hold the string "B" NOT the character 'B'
    char axiom [100];
    char rule [100];
    char var;
};

int grammar1(struct production prd[]){

    /*
        Creates the following Grammar:
        Axiom: B
        Rule 1 : A -> +A-C-A+
        Rule 2 : B -> B+C+B
        Rule 3 : C -> A-C-A
    */

    //one production per "rule" in the grammar
    //first production will ONLY have axiom

    strcpy(prd[0].axiom, "B");

    //Rule 1:
    prd[1].var = 'A';
    strcpy(prd[1].rule, "+A-C-A+");

    //Rule 2:
    prd[2].var = 'B';
    strcpy(prd[2].rule, "B+C+B");

    //rule 3:
    prd[3].var = 'C';
    strcpy(prd[3].rule, "A-C-A");

    //record length of rules in global (axiom plus 1 for each rule)
    PROD_LEN = 4;

    //returns index of last rule (ie. number of rules + 1 for axiom)
    return 3;

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

    double center_x,center_y,new_x, new_y;
    //find the centroid for the initial shape
    center_x = (MAX_X + MIN_X) / 2;
    center_y = (MAX_Y + MIN_Y) / 2;

    //adjust starting point to center the centroid of the resulting shape
    p[0] = (swidth/2 - center_x)*scale_factor;
    p[1] = (sheight/2 - center_y)*scale_factor;

    //redraw
    for(int i = 0; i < inst_len; ++i){
        angle = turtle_walk(p,angle,instructions[i]);
    }

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
    grammar1(prods);
    //construct string by expanding grammar to a given depth
    string_builder(instructions,DEPTH,prods);

    MIN_X = p[0];
    MAX_X = p[0];

    MIN_Y = p[1];
    MAX_Y = p[1];

    autoplacer(p,instructions,swidth,sheight);


    int key ;   
    key =  G_wait_key() ; // pause so user can see results

    return 0;
}
