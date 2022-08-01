/* Author: M Tran */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct {
  unsigned short val;
} color_t;

#define RGB(r, g, b) (color_t) { .val = ((r << 11) & 0xf800) | ((g << 5) & 0x07e0) | (b & 0x001f) }

void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void clear_screen(void *img);
void draw_pixel(void *img, int x, int y, color_t color);
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c);
void *create_buffer();
void blit(void *src);

#endif // GRAPHICS_H_
