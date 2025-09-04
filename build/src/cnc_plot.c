

#include <stdio.h>
#include <unistd.h> //sleep()
#include <sys/io.h> //outb() ioperm()
#include <stdlib.h>
#include <math.h>
#include <iomanip>

#define HEX(x) setw(2) << setfill('0') << hex << (int)( x )

#include <iostream>
#include <algorithm>
#include <vector>

#include "cnc_plot.h"
#include "math_op.h"
#include "point_op.h"

//microsecond duration between all pulses 
const double pulse_del = 500000;

double xpos = 0;
double ypos = 0;
double zpos = 0;




#define LPT1 0xc010
//#define LPT1 0x0378



/******************************************/
/*
std::vector<int> my_sort(const std::vector<int>& v) {
      auto result = v; // passing v by value and returning it defeats NRVO
      std::sort(result.begin(), result.end());
      return result;
}
*/



void cnc_plot::test_port(void)
{
    if(ioperm(LPT1,1,1))
    { 
        fprintf(stderr, "# Couldn't open parallel port \n"), exit(1);
    
    }

    unsigned char send_byte = 0x00;
    int a=0;int b=0;

    outb(0x00,LPT1); 
    //for(b=0;b<4;b++)
    //{
        send_byte = 0x01;
        for(a<0;a<8;a++)
        {
            outb(send_byte,LPT1);
            usleep(500000); 
                       
            outb(0x00,LPT1); 
            usleep(500000); 
            send_byte = send_byte << 1;
            cout <<"bit "<< a <<" value: "<< HEX(send_byte) <<"\n";

        }
    //}

}

/******************************************/
/*
    take the output of calc_3d_pulses() and send the signals to the parallel port 
    
    The first electent of the array denotes direction pulses
    


    DB25 PINOUT (using the CNC4PC/LinuxCNC board as my "defualt")

    2- X pulse 0x01
    3- X dir   0x02
    4- Y pulse 0x04
    5- Y dir   0x08
    6- Z pulse 0x10
    7- Z dir   0x20



*/

void cnc_plot::send_pulses(vector<vec3>* pt_pulsetrain)
{
    unsigned char send_byte = 0x00;
    int send_it = 1; 

    cout << "# we have pulses! count: " << pt_pulsetrain->size() << "\n";

    if(send_it==1)
    {
        if(ioperm(LPT1,1,1))
        { 
            fprintf(stderr, "# Couldn't open parallel port \n"), exit(1);
        }
        cout << "# transmitting pulses to LPT port \n";
    }

    //**************************//
    vec3 dirpulses = pt_pulsetrain->at(0);
    if(send_it==0)
    {
        cout <<"# debug - direction "<< dirpulses.x<<" " << dirpulses.y<<" " << dirpulses.z <<"\n";
    }

    if(send_it==1)
    {    
        //x direction high 
        if (dirpulses.x>1){
            //outb(0x02, LPT1);
            send_byte = send_byte |= (1 << 2);
            outb(send_byte, LPT1);            
        }else{
             //outb(0x00, LPT1);  
            send_byte = send_byte &= ~(1 << 2);
            outb(send_byte, LPT1);               
        }

        //y direction high 
        if (dirpulses.y>1){
            send_byte = send_byte |= (1 << 4);
            outb(send_byte, LPT1);               
        }else{
            //y direction low         
            send_byte = send_byte &= ~(1 << 4);
            outb(send_byte, LPT1);               
        }

        //z direction high 
        if (dirpulses.z>1){
            send_byte = send_byte |= (1 << 6);
            outb(send_byte, LPT1);               
        }else{
            send_byte = send_byte &= ~(1 << 6);   
            outb(send_byte, LPT1);                   
        }
    }


    //**************************//
    int x=0;
    //intentionally skipping over the first
    for(x=1;x<pt_pulsetrain->size();x++)
    {
        if(send_it==0)
        {
            cout<< pt_pulsetrain->at(x).x<<" " << pt_pulsetrain->at(x).y<<" " << pt_pulsetrain->at(x).z <<"\n";
        }

        if(send_it==1)
        {
            //X channel 
            if(pt_pulsetrain->at(x).x==1){
                send_byte = send_byte |= (1 << 1);
                outb(send_byte, LPT1); 
            }else{

                send_byte = send_byte &= ~ (1 << 1);
                outb(send_byte, LPT1);  
        
            }
                
            //Y channel
            if(pt_pulsetrain->at(x).y==1){
                send_byte = send_byte |= (1 << 3);
                outb(send_byte, LPT1);    
            }else{
                send_byte = send_byte &= ~(1 << 3);
                outb(send_byte, LPT1);                
            }

            //Z channel
            if(pt_pulsetrain->at(x).z==1){
                send_byte = send_byte |= (1 << 5);
                outb(send_byte, LPT1);   
            }else{
                send_byte = send_byte &= ~(1 << 5);
                outb(send_byte, LPT1);                 
            }

            sleep(pulse_del); 
        }

    }

    if(send_it==1)
    {
        cout << "finished transmitting pulses.\n";
    }


}



/******************************************/

void cnc_plot::calc_3d_pulses(vector<vec3>* pt_pulsetrain,
                              vec3 fr_pt, 
                              vec3 to_pt,
                              int numdivs)
{

            pointgen PG;

            //set the pulses per linear unit (spatial unit divions) - X,Y,Z unit prescaler 
            //for now use one number for all 3 - we will add the others in later
            // int pp_lux      = 10;
            // int pp_luy      = 10;
            // int pp_luz      = 10;
            int pp_lux      = numdivs;
            int pp_luy      = numdivs;
            int pp_luz      = numdivs;


            //make some storage for the data to work in 
            vector<vec3> x_pts;
            vector<vec3> y_pts;
            vector<vec3> z_pts;
            vector<vec3> samples;

            //make some pointers to those data.
            //(people who say THOSE data are technically correct, but they are pedantic dillholes) 
            vector<vec3>* pt_xpts    = &x_pts;
            vector<vec3>* pt_ypts    = &y_pts;
            vector<vec3>* pt_zpts    = &z_pts;
            vector<vec3>* pt_samples = &samples;


            //set up variables to do vector-y stuff
            vec3 between   = sub(fr_pt, to_pt);
            double mag     = length(between);
            double gran    = 0;  //granularity 
            double thresh  = 0;  //threshold 
            //cout << mag <<"\n";
            
            int xp=0;int yp=0;int zp=0;

            //calculate the absolute change for each axis  
            double delta_x = fr_pt.x-to_pt.x;
            double delta_y = fr_pt.y-to_pt.y;
            double delta_z = fr_pt.z-to_pt.z;

            //calc the direction of the vector 
            if (to_pt.x>fr_pt.x){
                xp=2;
            }else{
                xp=0; 
            }
            //calc the direction of the vector 
            if (to_pt.y>fr_pt.y){
                yp=2;
            }else{
                yp=0; 
            }
            //calc the direction of the vector 
            if (to_pt.z>fr_pt.z){
                zp=2;
            }else{
                zp=0; 
            }
            //first element of pulse train stores the direction 
            pt_pulsetrain->push_back(newvec3(xp,yp,zp));

 

            //use the total change to calculate the maximum possible pulses for each axis
            int num_pul_x = abs(delta_x)*pp_lux;
            int num_pul_y = abs(delta_y)*pp_luy;
            int num_pul_z = abs(delta_z)*pp_luz;
            //cout << "# num pulses " << num_pul_x <<" "<<num_pul_y<<" "<<num_pul_z <<"\n";

            // get the absolute highest number of pulses (on any axis) to calculate 
            int tmp[] = {num_pul_x, num_pul_y, num_pul_z};
            //cout << "before: "<<tmp[0] << " "<< tmp[1] <<" "<< tmp[2] <<"\n";
            std::sort(std::begin(tmp), std::end(tmp)  );
            //cout << "after: "<<tmp[0] << " "<< tmp[1] <<" "<< tmp[2] <<"\n";
            int most = tmp[2];
                                 
            // get the smallest division to use for sampling (granularity) 
            if (mag!=0 && most !=0)
            {
                gran = (mag/most);
            }else{
                gran = 0;
            }


            // calculate a series of points along vector for each axis 
            if(num_pul_x!=0)
            {
                PG.locate_pt_along3d(pt_xpts, to_pt, fr_pt, num_pul_x);
            }
            if(num_pul_y!=0) 
            {
                PG.locate_pt_along3d(pt_ypts, to_pt, fr_pt, num_pul_y);
            }
            if(num_pul_z!=0)
            {            
                PG.locate_pt_along3d(pt_zpts, to_pt, fr_pt, num_pul_z);
            }
      
            //# build a sampleset of all points along the vector - then iterate and match each axis to those points
            //# converting the spatial points into a pulse train 
            if (most!=0 && gran!=0)
            {
                //take the smallest possible sample.. and cut it in half for good measure
                thresh = gran/2;
                
                PG.locate_pt_along3d(pt_samples, to_pt, fr_pt, most);
                
                int a=0;int i=0;
                for(a=0;a<samples.size();a++)
                {
                    
                    vec3 spt = samples[a]; 

                    //X 
                    for (i=0;i<x_pts.size();i++)
                    {
                        vec3 xpt = x_pts[i];
                        vec3 ss = sub(xpt,spt);
                        if( length(ss)<thresh)
                        {
                            xp=1;
                            break;
                        }
                    }

                    //Y
                    for (i=0;i<y_pts.size();i++)
                    {
                        vec3 ypt = y_pts[i];
                        vec3 ss = sub(ypt,spt);
                        if( length(ss)<thresh)
                        {
                            yp=1;
                            break;
                        }
                    }


                    //Z
                    for (i=0;i<z_pts.size();i++)
                    {
                        vec3 zpt = z_pts[i];
                        vec3 ss = sub(zpt,spt);
                        if( length(ss)<thresh)
                        {
                            zp=1;
                            break;
                        }
                    }

                    pt_pulsetrain->push_back(newvec3(xp,yp,zp));
                    pt_pulsetrain->push_back(newvec3(0,0,0));
                }
             

           }


}

/******************************************/



 






