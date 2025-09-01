

#include <stdio.h>
#include <unistd.h> //sleep()
#include <sys/io.h> //outb() ioperm()
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include "cnc_plot.h"
#include "math_op.h"
#include "point_op.h"

const double pulse_del = .01;

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

    unsigned char foo = 0x01; 

    outb(0x00,LPT1); 
    outb(foo,LPT1); 
    sleep(1); 

    int a=0;int b=0;

    for(b<0;b<4;b++)
    {
        for(a<0;a<8;a++)
        {

            foo = foo << 1;
            outb(foo,LPT1);
            sleep(1); 

            outb(0x00,LPT1); 
            sleep(1); 
        }
        
        foo = 0x01; 

    }

}

/******************************************/
/*
    take the output of calc_3d_pulses() and send the signals to the parallel port 

*/

void cnc_plot::send_pulses(vector<vec3>* pt_pulsetrain)
{

    int send_it = 1; 

    cout << "# we have pulses! count: " << pt_pulsetrain->size() << "\n";

    if(send_it==1)
    {
        if(ioperm(LPT1,1,1))
        { 
            fprintf(stderr, "# Couldn't open parallel port \n"), exit(1);
        
        }
    }
    

    if(send_it==1)
    {
        cout << "# transmitting pulses to LPT port \n";
    }

    int x=0;
    for(x=0;x<pt_pulsetrain->size();x++)
    {
        if(send_it==0)
        {
            cout<< pt_pulsetrain->at(x).x<<" " << pt_pulsetrain->at(x).y<<" " << pt_pulsetrain->at(x).z <<"\n";
        }

        if(send_it==1)
        {
            //X channel 
            if(pt_pulsetrain->at(x).x==1){
                outb(255,LPT1);  //set all pins hi
            }else{
                outb(0  ,LPT1);  //set all pins low                
            }
            
            /*
            //Y channel
            if(pt_pulsetrain->at(x).y==1){
                outb(255,LPT1);  //set all pins hi
            }else{
                outb(0  ,LPT1);  //set all pins low                
            }

            //Z channel
            if(pt_pulsetrain->at(x).z==1){
                outb(255,LPT1);  //set all pins hi
            }else{
                outb(0  ,LPT1);  //set all pins low                
            }*/

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
                              vec3 to_pt)
{

            pointgen PG;

            //pulses per linear unit - X,Y,Z unit prescaler 
            int pp_lux      = 10;
            int pp_luy      = 10;
            int pp_luz      = 10;
        
            vec3 between   = sub(fr_pt, to_pt);
            double mag     = length(between);
            double gran    = 0;  //granularity 
            double thresh  = 0;  //threshold 

            //cout << mag <<"\n";

            double delta_x = fr_pt.x-to_pt.x;
            double delta_y = fr_pt.y-to_pt.y;
            double delta_z = fr_pt.z-to_pt.z;


            int num_pul_x = abs(delta_x)*pp_lux;
            int num_pul_y = abs(delta_y)*pp_luy;
            int num_pul_z = abs(delta_z)*pp_luz;

            // get the highest number of pulses to calculate 
            int tmp[] = {num_pul_x, num_pul_y, num_pul_z};
            //cout << "before: "<<tmp[0] << " "<< tmp[1] <<" "<< tmp[2] <<"\n";
            std::sort(std::begin(tmp), std::end(tmp)  );
            //cout << "after: "<<tmp[0] << " "<< tmp[1] <<" "<< tmp[2] <<"\n";
            int most = tmp[2];

            // get the smallest division to use as granularity 
            if (mag!=0 && most !=0)
            {
                gran = (mag/most);
            }else{
                gran = 0;
            }
 
            vector<vec3> x_pts;
            vector<vec3> y_pts;
            vector<vec3> z_pts;
            vector<vec3> samples;

            vector<vec3>* pt_xpts    = &x_pts;
            vector<vec3>* pt_ypts    = &y_pts;
            vector<vec3>* pt_zpts    = &z_pts;
            vector<vec3>* pt_samples = &samples;

            
            //cout << "# num pulses " << num_pul_x <<" "<<num_pul_y<<" "<<num_pul_z <<"\n";

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
                //if DEBUGMODE:
                //    print('### most possible samples', most)
                //    print(mag, most, gran, tmp)

                thresh = gran/2;
                
                PG.locate_pt_along3d(pt_samples, to_pt, fr_pt, most);
                
                int a=0;int i=0;
                //for(a=0;a<samples.size();a++){
                //    cout<<samples[a].x  <<" "<<samples[a].y  <<" "<<samples[a].z   << "\n";
                //}

                for(a=0;a<samples.size();a++)
                {
                    
                    vec3 spt = samples[a]; 
                    int xp=0;int yp=0;int zp=0;
                    

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

                    pt_pulsetrain->push_back( newvec3(xp,yp,zp));
                    pt_pulsetrain->push_back(newvec3(0,0,0));
                }
             

           }


}

/******************************************/



 






