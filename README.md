# micro_cnc_controller


This was prototyped in a single weekend! 

I drank too much coffee and wanted to make a simple tool to control a CNC/3D printer/Laser cutter.

Give it 2 3d points and it tells a servo/stepper how to get from one location to the other. 

I challenge you to come up with a simpler method of controlling a 3 Axis machine. 



The "build" folder contains the C sourde code and everything you need to make it. 

The "py" folder contains the python code I used to prototype the tool, and examples to run it. 



TODO:
    I still need to wire up the direction pulse, easy enough, just havent done it. 
    
    I need to work out the pinout (see below) and properly bitchift the pins to match. 
    It currently only flashes all 8 bits ON or OFF. I need to bitshift the port to control single pins/channels/axes.
    
    The bits on the port will correspond to the CNC4PC breakout board pin assignment (and "standard" LinuxCNC).
    I enojoy using this board because its rleatively cheap and works well. You can certainly make your own interface or use others, I just did it this way because I had it on hand.


PINOUT of CNC4PC DB25:

    Bidirectional pins 2-9
    Output pins        1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 16, 17. Or 1, 14, 16, 17
    Input pins         10, 11, 12, 13, 15. Or 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15








