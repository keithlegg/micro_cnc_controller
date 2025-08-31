#include <stdio.h>
#include <unistd.h> //sleep()
#include <sys/io.h> //outb() ioperm()


#define LPT1 0xc010
//#define LPT1 0x0378

#define HIGH 255
#define LOW 0



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include <unistd.h>
//#include <asm/io.h>


/*
    // to compile                 :  gcc -O lpt_pulser.c -o lpt_pulser
    // after compiling, set suid  :  chmod +s lpt_pulser   
*/


//pulses per linear unit 
const unsigned int pp_lux = 10;
const unsigned int pp_luy = 10;
const unsigned int pp_luz = 10;
const unsigned int pp_lui = 10;
const unsigned int pp_luj = 10;


const double pulse_del = .01;


double xpos = 0;
double ypos = 0;
double zpos = 0;
//double ipos = 0;
//double jpos = 0;


double x_goal_pos  = 0;
double y_goal_pos  = 0;
double z_goal_pos  = 0;
//double i_goal_pos = 0;
//double j_goal_pos = 0;


void update_pulses(void)
{
    double diff_x = 0;
    double diff_y = 0;
    double diff_z = 0;

    double a_diff_x = 0;
    double a_diff_y = 0;
    double a_diff_z = 0;

    double dir_x = 0;
    double dir_y = 0;
    double dir_z = 0;    

    double num_x = 0;
    double num_y = 0;
    double num_z = 0; 

 
    //calc the numeric difference 
    diff_x = x_goal_pos-xpos;
    diff_y = y_goal_pos-ypos;
    diff_z = z_goal_pos-zpos;

    //calc the absolute delta 
    a_diff_x = fabs(diff_x);
    a_diff_y = fabs(diff_y);
    a_diff_z = fabs(diff_z);

    //convert that to actual pulses 
    num_x = a_diff_x*pp_lux;
    num_y = a_diff_y*pp_luy;
    num_z = a_diff_z*pp_luz;     

    double a = 0;

    printf("%lf\n", a_diff_x);
    printf("%lf\n", a_diff_y);
    printf("%lf\n", a_diff_z);



    /*******/
    //this gets nasty fast - you need to run all three at the same time!
    //just do x for now 
    //this isnt even right because we need to bitshift each channel instead of all 8 !!
    for(a=0;a<num_x;a++){
        outb(255,LPT1);  //set all pins hi
        sleep(pulse_del); 
        
        outb(0,LPT1);    //set all pins lo
        sleep(pulse_del); 
    }
    
    /*******/

    //presuming it all worked, update internals to show that 
    xpos = x_goal_pos;
    ypos = y_goal_pos;
    zpos = z_goal_pos;


}

void set_x_pos(double coord)
{
    x_goal_pos = coord;
}

void set_y_pos(double coord)
{

    y_goal_pos = coord;
}

void set_z_pos(double coord)
{
    z_goal_pos = coord;

}


/*
void go_pos_x(double coord)
{

}*/



int main(void) 
{
    if(ioperm(LPT1,1,1))
    { 
        fprintf(stderr, "Couldn't open parallel port"), exit(1);
    }


    set_x_pos(1.5);
    update_pulses();
    sleep(.2);

    set_x_pos(1.49);
    update_pulses();

    return 0;
}




