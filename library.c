#include "graphics.h"
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <termios.h>

int fb; // frame buffer file
int tty; // terminal file
void* map; // map to frame buffer

// screen info
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

/*
* Opens framebuffer, maps memory, and disables configures keypress settings.
*/
void init_graphics() {
	// open file
	fb = open("/dev/fb0",O_RDWR);

	// get screen information
	ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fb, FBIOGET_FSCREENINFO, &finfo);

	// initialize map
	map = mmap(0, vinfo.yres_virtual*finfo.line_length, PROT_WRITE | PROT_READ, MAP_SHARED, fb, 0);

	// disable keypress echo and buffer keypresses
	tty = open("/dev/tty",O_RDWR);
	struct termios term;
	ioctl(tty, TCGETS, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	ioctl(tty, TCSETS, &term);
}

/*
* Reset keypress settings, removes mapping to framebuffer, and closes files.
*/
void exit_graphics() {
	// reverse keypress changes
	struct termios term;
	ioctl(tty, TCGETS, &term);
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	ioctl(tty, TCSETS, &term);

	// remove mapping and close files
	munmap(map, vinfo.yres_virtual*finfo.line_length);
	close(tty);
	close(fb);
}

/*
* Checks stdin for keyboard input, and if so, returns the key pressed.
*/
char getkey() {
	char key[1]; // key read

	// watch stdin
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);

	// check if stdin has data to read
	if (select(1, &fds, 0, 0, NULL)==1) {
		read(0, key, 1);
	}
	return key[0];
}

/*
* Sleeps for a given amount of milliseconds.
*/
void sleep_ms(long ms) {
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = ms*1000000;
	nanosleep(&time,NULL);
}

/*
* Blanks out given buffer.
*/
void clear_screen(void *img) {
	char *d = img;
	int i = 0;
	for (i = 0; i < (vinfo.yres_virtual*finfo.line_length); i++) {
		d[0] = 0;
		*d++;
	}
}

/*
* Draws a pixel at the specified coordinates and color to the given buffer.
*/
void draw_pixel(void *img, int x, int y, color_t color) {
	// checks if coordinates are within resolution
	if ((x > vinfo.xres) || (y > vinfo.yres) || (x < 0) || (y < 0)) {
		return;
	}
	// sets data at location on map
	long location = x * 2 + y * finfo.line_length;
	*((unsigned short*)(img + location)) = color.val;
}

/*
* Draws a line using Bresenham's algorithm with the specified color to the given buffer.
* (source: Wikipedia)
*/
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c) {
	int sx; int sy;

	// evaluate dx and sx
	int dx = x2 - x1;
	if (dx < 0) dx *= -1;
	if (x1<x2) sx = 1; else sx = -1;

	// evaluate dy and sy
	int dy = (y2 - y1);
	if (dy < 0) dy *= -1; dy *= -1;
	if (y1<y2) sy = 1; else sy = -1;

	int err = dx+dy; // error value

	// main drawing loop
	while (1) {
		draw_pixel(img, x1, y1, c);
		if (x1 == x2 && y1 == y2) break;
		int e2 = 2*err;
		if (e2 >= dy) {
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y1 += sy;
		}
	}
}

/*
* Creates a buffer to draw on.
*/
void *create_buffer() {
	// initialize map
	void *img = mmap(0, vinfo.yres_virtual*finfo.line_length, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	return img;
}

/*
* Memory copy from offscreen buffer to the framebuffer.
*/
void blit(void *src) {
	char *m = map;
	const char *s = src;
	int i = 0;
	for (i = 0; i < (vinfo.yres_virtual*finfo.line_length); i++) {
		*m++ = *s++;
	}
}
