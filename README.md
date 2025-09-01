# micro_cnc_controller

written in one weekend! I drank too much coffee and wanted to make a simple tool to control a CNC/3D printer/Laser cutter.

This is a mock up both in python and C. All it does at the moment is translate a single 3D vector into a series of pulses in X,Y,Z. 

Those pulses then are sent to a parallel port, or PCI parallel interface. 


TODO:
    it currently only flashes all 8 bits ON or OFF. I need to bitchift the port to control single pins/channels/axes.



The bits on the port will correspond to the CNC4PC breakout board pin assignment (and "standard" LinuxCNC).
I enojoy using this board because its rleatively cheap and works well. You can certainly make your own interface or use others, I just did it this way because I had it on hand. 

	Link to the board:
	    https://www.cnc4pc.com/shop/c10-c10-bi-directional-parallel-port-cnc-breakout-board-33?category=7

PINOUT of CNC4PC DB25:

	Bidirectional pins 2-9
	Output pins        1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 16, 17. Or 1, 14, 16, 17
	Input pins         10, 11, 12, 13, 15. Or 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15








