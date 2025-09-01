
#include <iostream>
#include <algorithm>
#include <vector>

#include <stdio.h>
#include <unistd.h> //sleep()
#include <sys/io.h> //outb() ioperm()

#define LPT1 0xc010
//#define LPT1 0x0378


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cnc_plot.h"



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



#include "math_op.h"
#include "point_op.h"


/*
std::vector<int> my_sort(const std::vector<int>& v) {
      auto result = v; // passing v by value and returning it defeats NRVO
      std::sort(result.begin(), result.end());
      return result;
}
*/


void cnc_plot::calc_3d_pulses(vector<vec3>* pt_pulsetrain,
                              vec3 fr_pt, 
                              vec3 to_pt)
{

            pointgen PG;

            int pp_lux      = 100;
            int pp_luy      = 100;
            int pp_luz      = 100;
        
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



/*



    ///////////////////////////////////////////
    def calc_3d_pulses(self, fr_pt, to_pt):
        """ 
            No idea is this is right, and probably not the best way to do this. 
            This converts a 3D vector into a series of pulses to drive the 3 (or more?) axis servos 
        """

        mag = ( fr_pt.between(to_pt).length )

        delta_x = fr_pt[0]-to_pt[0]
        delta_y = fr_pt[1]-to_pt[1]
        delta_z = fr_pt[2]-to_pt[2]
        #print(delta_x, delta_y, delta_z)
      
        num_pul_x = abs(delta_x)*self.pp_lux
        num_pul_y = abs(delta_y)*self.pp_luy
        num_pul_z = abs(delta_z)*self.pp_luz

        #print(num_pul_x, num_pul_y, num_pul_z)

        #get the highest number of pulses to calculate 
        tmp = [num_pul_x, num_pul_y, num_pul_z]
        sort_puls = sorted(tmp, reverse=True)
        most = sort_puls[0]
        
        #get the smallest division to use as granularity 
        if mag!=0 and most !=0:
            gran = (mag/most)
        else:
            gran = 0

        #calculate a series of points along vector for each axis 
        x_pts = self.locate_pt_along3d(to_pt, fr_pt, int(num_pul_x))
        y_pts = self.locate_pt_along3d(to_pt, fr_pt, int(num_pul_y))
        z_pts = self.locate_pt_along3d(to_pt, fr_pt, int(num_pul_z))


        DEBUGMODE = 0

        pulsetrain = []
        # build a sampleset of all points along the vector - then iterate and match each axis to those points
        # converting the spatial points into a pulse train 
        if most!=0 and gran!=0:
            if DEBUGMODE:
                print('### most possible samples', most)
                print(mag, most, gran, tmp)

            thresh = gran/2 
            samples = self.locate_pt_along3d(to_pt, fr_pt, int(most))
            for spt in samples:
                xp=0;yp=0;zp=0;

                for xpt in x_pts:
                    if( (xpt-spt).length<thresh):
                        #print((xpt-spt).length)
                        xp=1;break
                for ypt in y_pts:
                    if( (ypt-spt).length<thresh ):
                        yp=1;break
                for zpt in z_pts:
                    if( (zpt-spt).length<thresh ):
                        zp=1;break

                pulsetrain.append([xp,yp,zp])
                if not DEBUGMODE:
                    pulsetrain.append([0 ,0 ,0])
        
        if DEBUGMODE:
            xct=0;yct=0;zct=0
            for p in pulsetrain:
                if p[0]==1:
                    xct+=1
                if p[1]==1:
                    yct+=1
                if p[2]==1:
                    zct+=1                                        
            print(xct, yct, zct)


        if not DEBUGMODE:
            for p in pulsetrain:
                print(p)
*/


////////////////////////////////////

/*
plot = cnc_plot()
x = vec3(0 ,0 ,0  )
y = vec3(7 ,.1 ,15  )
p = plot.calc_3d_pulses(x,y)
#pts = plot.locate_pt_along3d(x,y,5)
*/






 



/*
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


    //////
    for(a=0;a<num_x;a++){
        outb(255,LPT1);  //set all pins hi
        sleep(pulse_del); 
        
        outb(0,LPT1);    //set all pins lo
        sleep(pulse_del); 
    }
    
    //////

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
*/




