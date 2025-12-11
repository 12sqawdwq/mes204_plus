#include "video_player.h"
#include "video_data.h"
#include "lcd_extreme.h" // 包含底层驱动
#include <stdint.h>

// 添加外部函数声明
extern void LCD_SendCommand(uint8_t command);
extern void LCD_SendData(uint8_t data);


static uint32_t current_video_frame = 0;

void Video_Reset(void) {
    current_video_frame = 0;
}

// 初始化视频模式的屏幕背景
// 这一步只需要做一次：把屏幕填满 0123...4567 的克隆图案
void Video_Init_Screen(void) {
    // 1. 设置第一行: 0 1 2 3 (重复4次)
    uint8_t line1[] = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
    LCD_SendCommand(0x80);
    for(int i=0; i<16; i++) LCD_SendData(line1[i]);

    // 2. 设置第二行: 4 5 6 7 (重复4次)
    uint8_t line2[] = {4,5,6,7, 4,5,6,7, 4,5,6,7, 4,5,6,7};
    LCD_SendCommand(0xC0);
    for(int i=0; i<16; i++) LCD_SendData(line2[i]);
}

uint8_t Video_Update_Frame(void) {
    if (current_video_frame >= TOTAL_FRAMES) {
        return 0; // 播放结束
    }

    // --- 核心：写入 CGRAM ---
    // LCD1602 的 CGRAM 地址是从 0x40 开始的
    // 8个字符 x 8字节 = 64字节，地址也是连续的，可以一口气写完
    LCD_SendCommand(0x40);

    // 极速突发写入 64 字节
    for (int i = 0; i < 64; i++) {
        LCD_SendData(VIDEO_DATA[current_video_frame][i]);
    }

    current_video_frame++;
    return 1; // 继续播放
}
