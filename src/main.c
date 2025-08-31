

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


//#include "gl_setup.h"     // common to all - moved to reduce size 

#include "math_op.h"        // general math operations
#include "point_op.h"         
#include "cnc_plot.h"         



char* obj_filepath;  

//std::vector scene_drawvec3;
//std::vector scene_drawvecclr;



vector<vec3> scene_drawvec3;
vector<vec3> scene_drawvecclr;   

int num_drawvec3; 


void test_bezier( vec3 start, vec3 ctrl1, vec3 ctrl2, vec3 end)
{

    pointgen PG;

    vector<vec3> * ptDrawvec = &scene_drawvec3;
    vector<vec3> * ptDrawClr = &scene_drawvecclr;
    int * ptnum_drawvec3 = &num_drawvec3;
    
    PG.cubic_bezier(ptDrawvec, ptDrawClr,  ptnum_drawvec3, 10, start, ctrl1, ctrl2, end);

}


void test_pt_along( vec3 start, vec3 ctrl1, vec3 ctrl2, vec3 end)
{

    pointgen PG;

    vector<vec3> * ptDrawvec = &scene_drawvec3;
    vector<vec3> * ptDrawClr = &scene_drawvecclr;
    int * ptnum_drawvec3 = &num_drawvec3;
    
    PG.cubic_bezier(ptDrawvec, ptDrawClr,  ptnum_drawvec3, 10, start, ctrl1, ctrl2, end);

}

int main(int argc, char **argv) 
{  



  
    pointgen PG;
    
    // locate_pt_along3d

    /*
    vec3 start = newvec3(0.0 ,3.0 ,1.0 );
    vec3 ctrl1 = newvec3(2.5  ,0.0 ,0.0 );
    vec3 ctrl2 = newvec3(0.0 ,1.0  ,0.0 );
    vec3 end   = newvec3(-1.0 ,0.0 ,-5.0 );
    test_bezier(start, ctrl1, ctrl2, end);
    cout<<scene_drawvec3[0].x  <<" "<<scene_drawvec3[0].y  <<" "<<scene_drawvec3[0].z   << "\n";
    cout<<scene_drawvecclr[0].x<<" "<<scene_drawvecclr[0].y<<" "<<scene_drawvecclr[0].z << "\n";
    */
    
    cout<<"all good\n";

    return 0;
}

