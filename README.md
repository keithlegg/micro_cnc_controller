# micro_cnc_controller


This started out as a printer port testing tool. Then I wondered what would be involved to make it drive 3 servo motors with STEP/DIR.

Not a working tool, but it is a good start to mockup a CNC controller.

The "build" folder contains the C source code and everything you need to make it. 
The "py" folder contains the python code I used to prototype the tool, and examples to run it. 


Follows the pinout of the "CNC4PC" interface board:

    Bidirectional pins 2-9
    Output pins        1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 16, 17. Or 1, 14, 16, 17
    Input pins         10, 11, 12, 13, 15. Or 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15


Does not support any inputs at this time. 








