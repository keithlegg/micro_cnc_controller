#ifndef CNC_PLOT_H    
#define CNC_PLOT_H

//#include <iostream>
//#include <math.h>

#include <vector>
using std::vector; //point_op did not make me do this!!?? WHY?

#include "math_op.h"


class cnc_plot
{
    public:
        cnc_plot(){};
        ~cnc_plot(){};

    void test_port(void);

    void calc_3d_pulses(vector<vec3>* pt_pulsetrain,
                        vec3 fr_pt, 
                        vec3 to_pt);

    void send_pulses(vector<vec3>* pt_pulsetrain);

};



#endif

