#include "graphics.h"
#include <string.h>
#include <stdlib.h> // abs 函数需要

// ==========================================
// 1. 显存定义 (The Owner)
// ==========================================
// 这里分配真正的内存空间。
// 必须确保 graphics.h 里定义了 LCD_HEIGHT_PX 和 LCD_WIDTH_PX
uint8_t VRAM[LCD_HEIGHT_PX][LCD_WIDTH_PX];

// ==========================================
// 2. 基础绘图函数
// ==========================================

void GFX_Clear(void) {
    // 清空 VRAM
    memset(VRAM, 0, LCD_HEIGHT_PX * LCD_WIDTH_PX);
}

void GFX_DrawPixel(int x, int y, uint8_t color) {
    // 边界检查
    if (x >= 0 && x < LCD_WIDTH_PX && y >= 0 && y < LCD_HEIGHT_PX) {
        VRAM[y][x] = color ? 1 : 0;
    }
}

void GFX_DrawLine(int x0, int y0, int x1, int y1, uint8_t color) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        GFX_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}