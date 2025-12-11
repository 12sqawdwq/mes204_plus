#include "graphics.h"
#include <math.h> // 需要开启 math 库
#include "lcd_extreme.h"

// 引入 VRAM
extern uint8_t VRAM[LCD_HEIGHT_PX][LCD_WIDTH_PX];

// 引用底层的 CGRAM 刷新函数 (复用 graphics_3d 中的那个刷新逻辑)
// 为了方便，我们在 main 里统一调用刷新，这里只负责算 VRAM
// 但我们需要一个简单的 wrapper 来复用 graphics_3d.c 里的 LCD_Refresh_Kaleidoscope
// 暂时假设我们在 main 中调用 graphics_3d.h 里的刷新函数即可

// ==========================================
// 液态金属算法 (Plasma / Metaballs)
// ==========================================
// 纯数学计算，不占 Flash 空间
void Math_Effect_Render(float time) {
    GFX_Clear();

    // 逻辑分辨率：20 x 16 (对应 8 个字符块)
    // 我们在这个小区域内计算高密度的波形
    int width = 20;
    int height = 16;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float v = 0.0f;
            
            // 波源 1：水平移动的正弦波
            v += sinf((x * 0.6f) + time);
            
            // 波源 2：垂直移动的余弦波
            v += cosf((y * 0.4f) + (time / 2.0f));
            
            // 波源 3：移动的点源 (Metaball核心)
            // 让中心点绕圈跑
            float cx = 10.0f + 10.0f * sinf(time / 3.0f);
            float cy = 8.0f + 8.0f * cosf(time / 2.0f);
            
            float dist = sqrtf((x - cx)*(x - cx) + (y - cy)*(y - cy));
            v += sinf(dist * 0.3f + time);
            
            // 阈值化：将模拟波形转化为 1-bit 黑白像素
            // v 的范围大约在 -3 到 +3 之间
            // 我们可以取中间值切一刀，产生流动的斑块
            if (v > 0.2f) {
                // 写入 VRAM
                // 注意：graphics_3d 的刷新逻辑只取 VRAM 的前 20x16
                // 所以我们直接写在这个范围内
                VRAM[y][x] = 1;
            }
        }
    }
}