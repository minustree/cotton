/*
COTTON
Author: M Tran
Character Design: Slitherbop
    _     _
   ||____||
  // o  o \\
  \\_____//

This file draws a character named Cotton to the screen.
You can close the left or right eye with the J and L keys, respectively.
You can change the direction Cotton is looking at using the WASD keys.
Exit the program using Q.
*/

#include "graphics.h"

/*
* Given an id, returns the associated color.
* The color ids are as follows:
* 0 = black
* 1 = white
* 2 = red
* 3 = purple
*/
color_t get_color(int id) {
	switch (id) {
		case 0: return RGB(0,0,0); // black
		case 1: return RGB(31,63,31); // white
		case 2: return RGB(31,0,10); // red
		case 3: return RGB(15,0,15); // purple
	}
}

/*
* Draws a rectangle at the specified coordinates and color to the given buffer.
* Round value determines how much to round the corners of the rectangle.
* Higher round value = more roundness.
*/
void draw_rectangle(void *img, int x1, int y1, int x2, int y2, int round, color_t c) {
	int i; int r;

	// if necessary, swap x and/or y values
	if (x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
	if (y1 > y2) { int temp = y1; y1 = y2; y2 = temp; }

	// calculate width and height
	int width = x2-x1;
	int height = y2-y1;

	// main drawing loop
	for (i = 0; i < height; i++) {
		if (height - i > height - round) {
			r = (height-i) - (height-round);
			draw_line(img,x1+r,y1+i,x1+width-r,y1+i,c);
		} else if (i > (height - round)) {
			r = i - (height - round);
			draw_line(img,x1+r,y1+i,x1+width-r,y1+i,c);
		} else {
			draw_line(img,x1,y1+i,x1+width,y1+i,c);
		}
	}
}

/*
* Draws a diamond at the specified coordinate, width, height, and color to the given buffer.
* The coordinate indicates the center of the diamond.
*/
void draw_diamond(void *img, int x, int y, int width, int height, color_t c) {
	// get radii
	width = width/2;
	height = height/2;

	int size; // biggest radius of diamond
	if (width >= height) size = width;
	else size = height;

	// main drawing loop
	int i; int xi = 0; int yi = 0;
	for (i = 0; i < size; i++) {
		draw_line(img,x+xi,y,x,y+yi,c);
		draw_line(img,x+xi,y,x,y-yi,c);
		draw_line(img,x-xi,y,x,y+yi,c);
		draw_line(img,x-xi,y,x,y-yi,c);
		if (xi < width) xi++;
		if (yi < height) yi++;
	}
}

/*
* Draws Cotton to the given buffer.
*/
void draw_cotton(void *img) {
	draw_pixel(img, 150, 200, get_color(3));

	// draw head
	draw_rectangle(img, 150, 200, 490, 440, 30, get_color(3)); // hood
	draw_rectangle(img, 190, 240, 450, 400, 30, get_color(1)); // face

	// draw ears
	draw_rectangle(img, 180, 230, 260, 20, 15, get_color(3)); // left ear
	draw_rectangle(img, 460, 230, 380, 20, 15, get_color(3)); // right ear

	// draw eyes
	draw_diamond(img, 260, 300, 100, 60, get_color(2)); // left eye
	draw_diamond(img, 380, 300, 100, 60, get_color(2)); // right eye

	// draw eyelashes
	int i;
	for (i = 0; i < 6; i++) {
		// left
		draw_line(img, 260+i, 300-i, 300+i, 330, get_color(2));
		draw_line(img, 260-i, 300-i, 220-i, 330, get_color(2));
		draw_line(img, 257+i, 310, 257+i, 340, get_color(2));

		// right
		draw_line(img, 380+i, 300-i, 420+i, 330, get_color(2));
		draw_line(img, 380-i, 300-i, 340-i, 330, get_color(2));
		draw_line(img, 377+i, 310, 377+i, 340, get_color(2));
	}

	// draw pupils
	draw_diamond(img, 260, 300, 10, 30, get_color(0)); // left pupil
	draw_diamond(img, 380, 300, 10, 30, get_color(0)); // right pupil
}

/*
* Updates Cotton's appearance on the given buffer given status of both eyes and
* location of pupils.
*/
void update_cotton(void *img, int left_eye, int right_eye, int pupil_h, int pupil_v) {
	int offset = 15; // how much pupil moves

	// left eye
	if (left_eye) {
		draw_diamond(img, 260, 300, 100, 60, get_color(2));
		draw_diamond(img, 260, 290, 100, 60, get_color(1));
	} else if (!left_eye) {
		draw_diamond(img, 260, 300, 100, 60, get_color(2));
		draw_diamond(img, 260+(pupil_h*offset), 300+(pupil_v*offset), 10, 30, get_color(0));
	}

	// right eye
	if (right_eye) {
		draw_diamond(img, 380, 300, 100, 60, get_color(2));
		draw_diamond(img, 380, 290, 100, 60, get_color(1));
	} else if (!right_eye) {
		draw_diamond(img, 380, 300, 100, 60, get_color(2));
		draw_diamond(img, 380+(pupil_h*offset), 300+(pupil_v*offset), 10, 30, get_color(0));
	}

}

int main(int argc, char **argv) {
	init_graphics();
	void* img = create_buffer();
	char key;

	// control values
	int left_eye = 0;
	int right_eye = 0;
	int pupil_h = 0;
	int pupil_v = 0;
	int update = 0;

	// initial draw
	draw_cotton(img);
	blit(img);

	// main loop
	do {
		update = 0;
		key = getkey();
		if (key == 'q') break;

		switch (key) {
			// eye control
			case 'j':
				if (left_eye) left_eye = 0;
				else left_eye = 1;
				update = 1; break;
			case 'l':
				if (right_eye) right_eye = 0;
				else right_eye = 1;
				update = 1; break;

			// pupil control
			case 'w':
				if (pupil_v > -1 && pupil_h == 0) {
					pupil_v--;
					update = 1;
				} break;
			case 'a':
				if (pupil_h > -1 && pupil_v == 0) {
					pupil_h--;
					update = 1;
				} break;
			case 's':
				if (pupil_v < 1 && pupil_h == 0) {
					pupil_v++;
					update = 1;
				} break;
			case 'd':
				if (pupil_h < 1 && pupil_v == 0) {
					pupil_h++;
					update = 1;
				} break;
		}
		if (update) update_cotton(img, left_eye, right_eye, pupil_h, pupil_v);
		blit(img);
		sleep_ms(100);
	}
	while (1);

	// clear screen and exit
	clear_screen(img);
	blit(img);
	exit_graphics();

	return 0;
}
