#include "graphics.h"
    #include <string.h>
    #include <stdlib.h> // abs 函数需要

    // ==========================================
    // 1. 显存定义 (The Owner)
    // ==========================================

    // --- 字符缓冲区 ---
    // 用于粒子效果、数学特效等
    char GFX_char_buffer[GFX_HEIGHT][GFX_WIDTH];

    // --- 像素缓冲区 ---
    // 用于 3D 渲染等高级绘图
    uint8_t VRAM[LCD_HEIGHT_PX][LCD_WIDTH_PX];


    // ==========================================
    // 2. 基础绘图函数
    // ==========================================

    /**
     * @brief 清空所有缓冲区 (字符和像素)
     */
    void GFX_Clear(void) {
        // 清空字符缓冲区
        memset(GFX_char_buffer, ' ', GFX_HEIGHT * GFX_WIDTH);
        // 清空像素缓冲区
        memset(VRAM, 0, LCD_HEIGHT_PX * LCD_WIDTH_PX);
    }

    /**
     * @brief 在字符缓冲区指定位置绘制一个字符
     * @param x X坐标 (0 to GFX_WIDTH-1)
     * @param y Y坐标 (0 to GFX_HEIGHT-1)
     * @param c 要绘制的字符
     */
    void GFX_Draw_Char(int x, int y, char c) {
        // 边界检查
        if (x >= 0 && x < GFX_WIDTH && y >= 0 && y < GFX_HEIGHT) {
            GFX_char_buffer[y][x] = c;
        }
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