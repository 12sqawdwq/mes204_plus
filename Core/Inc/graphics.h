#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

// 必须定义尺寸，供 graphics.c 使用
#define LCD_WIDTH_PX  80
#define LCD_HEIGHT_PX 16

// 声明外部可用的函数
void GFX_Clear(void);
void GFX_DrawPixel(int x, int y, uint8_t color);
void GFX_DrawLine(int x0, int y0, int x1, int y1, uint8_t color);

// 声明 VRAM 让其他人也能看到 (可选，但为了规范)
extern uint8_t VRAM[LCD_HEIGHT_PX][LCD_WIDTH_PX];

#endif