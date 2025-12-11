#include "lcd_extreme.h"
#include "main.h"

// ================= 硬件定义 (请再次确认与原理图一致) =================
#define LCD_DATA_PORT GPIOC
#define DATA_PIN_MASK 0x03FC // PC2-PC9

#define LCD_CTRL_PORT GPIOB
#define LCD_RS_PIN    GPIO_PIN_8
#define LCD_EN_PIN    GPIO_PIN_9

// ================= 内部辅助函数 =================
static void Delay_us_Rough(volatile uint32_t count) {
    volatile uint32_t k = count * 30;
    while(k--) { __NOP(); }
}

static void LCD_Pulse_Enable(void) {
    LCD_CTRL_PORT->BSRR = LCD_EN_PIN;
    Delay_us_Rough(50);
    LCD_CTRL_PORT->BSRR = (uint32_t)LCD_EN_PIN << 16;
    Delay_us_Rough(50);
}

static void LCD_Write_Bus(uint8_t data) {
    uint32_t val = LCD_DATA_PORT->ODR;
    val &= ~DATA_PIN_MASK;
    val |= ((uint32_t)data << 2);
    LCD_DATA_PORT->ODR = val;
}

// ================= 公开 API (不能加 static) =================

void LCD_SendCommand(uint8_t cmd) {
    LCD_CTRL_PORT->BSRR = (uint32_t)LCD_RS_PIN << 16; // RS Low
    LCD_Write_Bus(cmd);
    LCD_Pulse_Enable();
    Delay_us_Rough(2000);
}

void LCD_SendData(uint8_t data) {
    LCD_CTRL_PORT->BSRR = LCD_RS_PIN; // RS High
    LCD_Write_Bus(data);
    LCD_Pulse_Enable();
    Delay_us_Rough(100);
}

void LCD_Init_Extreme(void) {
    // 确保 EN 初始为低
    LCD_CTRL_PORT->BSRR = (uint32_t)LCD_EN_PIN << 16;
    HAL_Delay(50);

    LCD_SendCommand(0x38);
    HAL_Delay(10);
    LCD_SendCommand(0x38);
    HAL_Delay(1);
    LCD_SendCommand(0x38);

    LCD_SendCommand(0x0C);
    LCD_SendCommand(0x01);
    HAL_Delay(5);
    LCD_SendCommand(0x06);
}