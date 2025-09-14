# micro_cnc_controller


This started out as a printer port testing tool. Then I wondered what would be involved to make it drive 3 servo motors with STEP/DIR.


What it does so far:

It takes two 3D points and calculates the differecne FROM a point TO a point (a 3D vector.)
It then breaks that vector into 3 sets of STEP and DIRection pulses and sends them out a parallel port. 
It also reads three inputs (X,Y,Z) - watching for limit switches and disables the pulses if it detects a crash.

For now, thats it. 


I am in the process of building a textfile configuration tool, beacuse the number of parameters it needs is growing exponentially.  
I will be adding an AUX output pin to turn things off and on, like say a spindle of filament extruder ;)


This is not a complete CNC controller itself, it does not even track position in space beyond local single transactional movements.
BUT you can build on it as a library or call it from python and do the control logic over there.


The "pulser" folder contains the C source code and everything you need to make it. Just type "make" to build it with GCC
The "py" folder contains the python code I used to prototype the tool, and a working example to run it. 


Follows the same pinout of the "CNC4PC" interface board:
    Output pins        1, 2, 3, 4, 5, 6, 7, 8
    Input pins         10, 12, 13

If you use the CNC4PC board and an arduino GRBL sheild with DRV8825s, you can control a 3D printer or small machine for less than 50 bucks!

Happy pulsing!


-Keith Legg









