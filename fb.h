
#ifndef FB_H_	/* Include guard */
#define FB_H_

void clear_fb(void);
void draw_pixel(int x, int y, u32 color);
void draw_char(int x, int y, unsigned char ch, u32 color, u32 bg);
void draw_string(int x, int y, const char *s, u32 textcolor, u32 bgcolor);
void framebuffer_test(void);

#endif // FB_H_