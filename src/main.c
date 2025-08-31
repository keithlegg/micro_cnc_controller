

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


//#include "gl_setup.h"     // common to all - moved to reduce size 

#include "math_op.h"        // general math operations
#include "point_op.h"         
#include "cnc_plot.h"         



char* obj_filepath;  



vector<vec3> scene_drawvec3;
vector<vec3> scene_drawvecclr;   

int num_drawvec3; 


void test_bezier( vec3 start, vec3 ctrl1, vec3 ctrl2, vec3 end)
{
    /*
    USAGE:

    vec3 start = newvec3(0.0 ,3.0 ,1.0 );
    vec3 ctrl1 = newvec3(2.5  ,0.0 ,0.0 );
    vec3 ctrl2 = newvec3(0.0 ,1.0  ,0.0 );
    vec3 end   = newvec3(-1.0 ,0.0 ,-5.0 );
    test_bezier(start, ctrl1, ctrl2, end);
    int a=0;
    for(a=0;a<scene_drawvec3.size();a++){
        cout<<scene_drawvec3[a].x  <<" "<<scene_drawvec3[a].y  <<" "<<scene_drawvec3[a].z   << "\n";
    }*/

    pointgen PG;

    vector<vec3> * ptDrawvec = &scene_drawvec3;
    vector<vec3> * ptDrawClr = &scene_drawvecclr;
    int * ptnum_drawvec3 = &num_drawvec3;
    
    PG.cubic_bezier(ptDrawvec, ptDrawClr,  ptnum_drawvec3, 10, start, ctrl1, ctrl2, end);

}


/************/
void test_pt_along(void)
{



    pointgen PG;

    vector<vec3> * ptDrawvec = &scene_drawvec3;
    vec3 s_p = newvec3(0,0,0);
    vec3 e_p = newvec3(1,0,0);

    
    PG.locate_pt_along3d(ptDrawvec, s_p, e_p, 4);
    int a=0;
    for(a=0;a<scene_drawvec3.size();a++){
        cout<<scene_drawvec3[a].x  <<" "<<scene_drawvec3[a].y  <<" "<<scene_drawvec3[a].z   << "\n";
    } 

}


/************/

int main(int argc, char **argv) 
{  
    /*
        // calculate the face normal  
        vec3 a = sub(p1,p2);
        vec3 b = sub(p1,p3);
        vec3 n = normalize(cross(a,b));
    */

    
    test_pt_along();
    
    cout<<"all good\n";

    return 0;
}

