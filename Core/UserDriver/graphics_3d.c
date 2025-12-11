#include "graphics_3d.h"
#include <stdlib.h>
#include <string.h>

// 引用外部 LCD 驱动 (确保 lcd_extreme.c 里有这些函数)
extern void LCD_SendCommand(uint8_t cmd);
extern void LCD_SendData(uint8_t data);

// 引用显存 (来自 graphics.c)
extern uint8_t VRAM[VRAM_HEIGHT][VRAM_WIDTH];

// ================= 模型数据定义 =================
Point3D cube_vertices[8] = {
    {-10, -10, -10}, {10, -10, -10}, {10, 10, -10}, {-10, 10, -10},
    {-10, -10,  10}, {10, -10,  10}, {10, 10,  10}, {-10, 10,  10}
};

Edge cube_edges[12] = {
    {0,1}, {1,2}, {2,3}, {3,0}, // Back
    {4,5}, {5,6}, {6,7}, {7,4}, // Front
    {0,4}, {1,5}, {2,6}, {3,7}  // Connections
};

// ================= 3D 渲染引擎 =================

// 注意：删除了 GFX_Init_VRAM 和 GFX_Clear，因为 graphics.c 里已经有了

void GFX_Render_Cube(float ax, float ay, float az) {
    int cx = 10;
    int cy = 8;
    float scale = 0.5;
    int px[8], py[8];

    // 1. 顶点变换
    for (int i = 0; i < 8; i++) {
        float x = cube_vertices[i].x;
        float y = cube_vertices[i].y;
        float z = cube_vertices[i].z;
        float tmp;
        // X
        tmp = y*cos(ax) - z*sin(ax); z = y*sin(ax) + z*cos(ax); y = tmp;
        // Y
        tmp = x*cos(ay) + z*sin(ay); z = -x*sin(ay) + z*cos(ay); x = tmp;
        // Z
        tmp = x*cos(az) - y*sin(az); y = x*sin(az) + y*cos(az); x = tmp;
        // Proj
        px[i] = cx + (int)(x * scale);
        py[i] = cy + (int)(y * scale);
    }

    // 2. 绘制连线 (调用 graphics.c 中的 GFX_DrawLine)
    for (int i = 0; i < 12; i++) {
        GFX_DrawLine(px[cube_edges[i].start_idx], py[cube_edges[i].start_idx],
                     px[cube_edges[i].end_idx],   py[cube_edges[i].end_idx],
                     1);
    }
}

// ================= 刷新机制 =================

void LCD_Refresh_Kaleidoscope(void) {
    // ---------------------------------------------------------
    // 1. 上传 CGRAM (这部分不用动，保持原样)
    // ---------------------------------------------------------
    LCD_SendCommand(0x40); // Set CGRAM Address

    // 对应左侧 20x16 区域 (4x2个字符)
    int block_coords[8][2] = {
        {0,0}, {5,0}, {10,0}, {15,0}, // Top: 0, 1, 2, 3
        {0,8}, {5,8}, {10,8}, {15,8}  // Bot: 4, 5, 6, 7
    };

    for (int b = 0; b < 8; b++) {
        int sx = block_coords[b][0];
        int sy = block_coords[b][1];
        for (int row = 0; row < 8; row++) {
            uint8_t line_data = 0;
            for (int col = 0; col < 5; col++) {
                if (sy + row < VRAM_HEIGHT && sx + col < VRAM_WIDTH) {
                    if (VRAM[sy + row][sx + col]) {
                        line_data |= (1 << (4 - col));
                    }
                }
            }
            LCD_SendData(line_data);
        }
    }

    // ---------------------------------------------------------
    // 2. 刷新屏幕 DDRAM (关键修改：改为纯克隆模式)
    // ---------------------------------------------------------

    // 第一行：重复显示 0,1,2,3 (四个立方体的上半身)
    // 之前是 {0,1,2,3, 3,2,1,0...} 导致了颠倒
    // 现在改为全正序：
    uint8_t line1[] = {
        0, 1, 2, 3,   // 第1列阵 (1-4)
        0, 1, 2, 3,   // 第2列阵 (5-8)
        0, 1, 2, 3,   // 第3列阵 (9-12)
        0, 1, 2, 3    // 第4列阵 (13-16)
    };

    LCD_SendCommand(0x80); // 移动到第一行
    for(int i=0; i<16; i++) {
        LCD_SendData(line1[i]);
    }

    // 第二行：重复显示 4,5,6,7 (四个立方体的下半身)
    uint8_t line2[] = {
        4, 5, 6, 7,   // 第1列阵
        4, 5, 6, 7,   // 第2列阵
        4, 5, 6, 7,   // 第3列阵
        4, 5, 6, 7    // 第4列阵
    };

    LCD_SendCommand(0xC0); // 移动到第二行
    for(int i=0; i<16; i++) {
        LCD_SendData(line2[i]);
    }
}
