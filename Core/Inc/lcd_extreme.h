#ifndef __LCD_EXTREME_H__
#define __LCD_EXTREME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <math.h>

    // // 定义一个简单的 3D 结构
    // typedef struct {
    //     float x, y, z;
    // } Point3D;
    //
    // typedef struct {
    //     int start_idx; // 起始点索引
    //     int end_idx;   // 结束点索引
    // } Edge;
    //
    // // === 声明立方体模型 ===
    // // 使用 extern 声明全局变量，定义放在 .c 文件中
    // extern Point3D cube_vertices[8];
    // extern Edge cube_edges[12];

    // ==========================================
    // 1. 公共配置与定义 (Public Definitions)
    // ==========================================

    // 虚拟屏幕分辨率
    // 物理：16列 x 2行字符
    // 逻辑：80像素 x 16像素 (每个字符 5x8)
#define LCD_WIDTH_PX  80
#define LCD_HEIGHT_PX 16

    // 显存访问宏 (可选，用于后续代码的可读性)
#define PIXEL_ON  1
#define PIXEL_OFF 0

    // ==========================================
    // 2. 全局变量声明 (Global Variables)
    // ==========================================

    // 将 VRAM 暴露给外部文件 (如 graphics.c)
    // 外部文件可以直接修改这个数组，然后调用刷新函数
    extern uint8_t VRAM[LCD_HEIGHT_PX][LCD_WIDTH_PX];

    // ==========================================
    // 3. 函数原型 (Function Prototypes)
    // ==========================================

    /**
     * @brief 初始化 LCD1602 的 GPIO 和控制器
     * @note  配置为 8-bit 并行模式，使用极限速度推挽输出
     */
    void LCD_Extreme_Init(void);

    /**
     * @brief 发送指令到 LCD (如清屏、光标移动)
     * @param cmd 指令字节
     */
    void LCD_Write_Cmd(uint8_t cmd);

    /**
     * @brief 发送数据到 LCD (显示字符)
     * @param data 字符的 ASCII 码或自定义字符索引
     */
    void LCD_Write_Data(uint8_t data);

    /**
     * @brief 清空 STM32 内部的虚拟显存 (VRAM)
     * @note  这不会立即清除 LCD 屏幕，需要运行刷新算法后才生效
     */
    void VRAM_Clear(void);

    // 建议添加一个简单的字符串打印函数用于测试硬件连接 (在 lcd_extreme.c 中实现)
    // void LCD_ShowString(uint8_t x, uint8_t y, char *str);

    void LCD_Clear(void);

    void LCD_Render_Frame(void);

    void GFX_K3D_Render(float angle_x, float angle_y, float angle_z, float scale);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_EXTREME_H__ */
