#ifndef CNC_PLOT_H    
#define CNC_PLOT_H

#include "math_op.h"


class cnc_plot
{
    public:
        cnc_plot(){
        };

        ~cnc_plot(){};

    void calc_3d_pulses(vec3 fr_pt, 
                        vec3 to_pt);
};



#endif

