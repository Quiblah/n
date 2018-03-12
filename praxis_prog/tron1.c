// BGIFirstTest.c : Example graphic application
//


#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>

void main(void)
{
	//TODO: add better maps.
	// Initialise graphic window
	int gd = DETECT, gm = 0;
	initgraph(&gd, &gm, "");

	const int mid_x = getmaxx() / 2;
	const int mid_y = getmaxy() / 2;


	//		1
	//	4		2
	//		3


	//initial player 1 pos and direction
	int p1_x = (mid_x + mid_y) - 10;
	int p1_y = mid_y;
	int p1_direction = 4;

	//initial player 2 pos and direction
	int p2_x = (mid_x - mid_y) + 10;
	int p2_y = mid_y;
	int p2_direction = 2;
	
	//map
	ellipse(mid_x, mid_y, 0, 360, , mid_y);
	
	delay(1000);


	// 0: no winnner yet, 1: p1 wins, 2: p2 wins, 3: draw.
	int winner = 0;
	while (winner == 0) {
		//draw current positions
		putpixel(p1_x, p1_y, RED);
		putpixel(p2_x, p2_y, LIGHTBLUE);


		//check for p2 direction change
		if (keypressed(0x41) != 0 && p2_direction != 2) {
			p2_direction = 4;
		}
		if (keypressed(0x57) != 0 && p2_direction != 3) {
			p2_direction = 1;
		}
		if (keypressed(0x44) != 0 && p2_direction != 4) {
			p2_direction = 2;
		}
		if (keypressed(0x53) != 0 && p2_direction != 1) {
			p2_direction = 3;
		}


		//check for p1 direction change
		if (keypressed(0x4A) != 0 && p1_direction != 2) {
			p1_direction = 4;
		}
		if (keypressed(0x49) != 0 && p1_direction != 3) {
			p1_direction = 1;
		}
		if (keypressed(0x4C) != 0 && p1_direction != 4) {
			p1_direction = 2;
		}
		if (keypressed(0x4B) != 0 && p1_direction != 1) {
			p1_direction = 3;
		}


		//calculate p1's next pos
		switch (p1_direction) {
		case 1:
			p1_y -= 1;
			break;
		case 2:
			p1_x += 1;
			break;
		case 3:
			p1_y += 1;
			break;
		case 4:
			p1_x -= 1;
			break;
		default:
			printf("Error no direction");
		}

		
		//cacluclate p2's next pos.
		switch (p2_direction) {
		case 1:
			p2_y -= 1;
			break;
		case 2:
			p2_x += 1;
			break;
		case 3:
			p2_y += 1;
			break;
		case 4:
			p2_x -= 1;
			break;
		default:
			printf("Error no direction");
		}


		//check for a collision
		if (getpixel(p1_x, p1_y) != getbkcolor()) winner = 2;
		if (getpixel(p2_x, p2_y) != getbkcolor()) winner = 1;
		if (p1_x == p2_x && p1_y == p2_y) winner = 3;

		delay(25);

		
	}
	delay(2000);
	closegraph();
	
	if(winner==1) printf("RED WINS\n");
	if(winner==2) printf("BLUE WINS\n");
	if(winner==3) printf("DRAW\n");

	delay(2000);
	

}
