PROBLEM STATEMENT:

Design an autonomous robot capable of navigating a complex dynamic maze. The robot must use multi-sensors integration to detect path and objects. It should employ learning algorithms to optimize its route over multiple attempts reducing time to exit and adapting to change in the maze structure.

DESCRIPTION:
	
COMPONENTS USED –
1.	Arduino nano
2.	L298N motor driver
3.	Offset dc motors
4.	Ultrasonic and ir sensors
5.	RMC
6.	Buck converter 
	
1. Solving the maze –
Left wall Rule:  The left wall rule is a maze solving technique in which the bot sticks to the left wall and keeps moving along it throughout the maze. In other words, the bot  takes left turns wherever possible. This approach is to make the way through the maze while choosing how to turn at intersections as follows: always turn left if it can i.e., preference is always given to the left turn . If you cannot turn left, go straight. If you cannot turn left or straight then turn right.

Optimization: The bot will move about the maze and scan all the possible ways to solve the maze inclusive of all the dead ends and wrong turns and will store that information. In the second try, it will optimise the best and the shortest path to reach the end.
	
Finish line strategy: Once the bot reaches the finish line, it will come to a stop by the detection of a shed using the ir sensor present on top of the bot.  


