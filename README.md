# micro_cnc_controller
drank too much coffee - wanted to code the smallest parport cnc controller


This is a mock up both in python and C. It translates a 3D vector into a series of pulses in X,Y,Z. 
Those pulses then are sent to a parallel port, or PCI parallel insteface. 

The bits on the port correspond to the CNC4PC breakout board that I enojoy using because its rleatively cheap and works well.

Link to the board:
    https://www.cnc4pc.com/shop/c10-c10-bi-directional-parallel-port-cnc-breakout-board-33?category=7

You can certainly make your own interface or use others, I just did it this way because I have this on hand. 


PINOUT of CNC4PC DB25:

	Bidirectional pins 2-9
	Output pins        1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 16, 17. Or 1, 14, 16, 17
	Input pins         10, 11, 12, 13, 15. Or 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15








