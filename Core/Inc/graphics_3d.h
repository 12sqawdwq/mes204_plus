#ifndef GRAPHICS_3D_H
#define GRAPHICS_3D_H

#include <stdint.h>
#include <math.h>
#include "graphics.h" // 包含 graphics.h 以获取 GFX_DrawLine 和 GFX_Clear 的声明

// 结构体定义
typedef struct {
    float x, y, z;
} Point3D;

typedef struct {
    int start_idx;
    int end_idx;
} Edge;

// 显存宏
#define VRAM_WIDTH  80
#define VRAM_HEIGHT 16

// 接口声明 (注意：不声明 GFX_Clear，因为它在 graphics.h)
void GFX_Render_Cube(float angle_x, float angle_y, float angle_z);
void LCD_Refresh_Kaleidoscope(void);

#endif