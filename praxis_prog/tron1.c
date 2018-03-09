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


	//		1
	//	4		2
	//		3


	//initial player 1 pos and direction
	int p1_x = (getmaxx() / 2 + getmaxy() / 2) - 10;
	int p1_y = getmaxy() / 2;
	int p1_direction = 4;

	//initial player 2 pos and direction
	int p2_x = (getmaxx() / 2 - getmaxy() / 2) + 10;
	int p2_y = getmaxy() / 2;
	int p2_direction = 2;
	
	//map
	ellipse(getmaxx() / 2, getmaxy() / 2, 0, 360, getmaxx() / 2, getmaxy() / 2);
	
	delay(1000);


	// 0: no winnner yet, 1: p1 wins, 2: p2 wins, 3: draw.
	int winner = 0;
	while (winner == 0) {
		//draw current positions
		putpixel(p1_x, p1_y, RED);
		putpixel(p2_x, p2_y, LIGHTBLUE);


		//check for p2 direction change
		if (keypressed(0x41) != 0) {
			p2_direction = 4;
		}
		if (keypressed(0x57) != 0) {
			p2_direction = 1;
		}
		if (keypressed(0x44) != 0) {
			p2_direction = 2;
		}
		if (keypressed(0x53) != 0) {
			p2_direction = 3;
		}


		//check for p1 direction change
		if (keypressed(0x4A) != 0) {
			p1_direction = 4;
		}
		if (keypressed(0x49) != 0) {
			p1_direction = 1;
		}
		if (keypressed(0x4C) != 0) {
			p1_direction = 2;
		}
		if (keypressed(0x4B) != 0) {
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
	
	switch (winner) {
	case 1:
		printf("RED WINS\n");
		break;
	case 2:
		printf("BLUE WINS\n");
		break;
	case 3:
		printf("DRAW");
		break;
	default:
		printf("error no winner");
	}
	delay(2000);
	

}