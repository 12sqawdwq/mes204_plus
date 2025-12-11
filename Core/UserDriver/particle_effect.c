#include "particle_effect.h"
#include "graphics.h"

#include <stdlib.h> // For rand()

// --- 为粒子系统定义结构体 ---
typedef struct {
    float x, y;     // 粒子位置
    float vx, vy;   // 粒子速度
    int life;       // 粒子生命周期
} Particle;

// --- 粒子数组 ---
static Particle particles[MAX_PARTICLES];

/**
 * @brief 初始化或重置所有粒子
 */
void Particle_Init(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].x = rand() % GFX_WIDTH;
        particles[i].y = rand() % GFX_HEIGHT;
        // 给予一个随机的初始速度
        particles[i].vx = ((rand() % 100) / 50.0f) - 1.0f; // 速度范围 [-1.0, 1.0]
        particles[i].vy = ((rand() % 100) / 50.0f) - 0.5f; // 速度范围 [-0.5, 0.5]
        particles[i].life = rand() % PARTICLE_MAX_LIFE;
    }
}

/**
 * @brief 更新并渲染所有粒子到 GFX 缓冲区
 */
void Particle_UpdateAndRender(void) {
    GFX_Clear(); // 清空上一帧

    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 更新位置
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        particles[i].life--;

        // 如果粒子生命结束或移出屏幕，则重置它
        if (particles[i].life <= 0 || particles[i].x < 0 || particles[i].x >= GFX_WIDTH || particles[i].y < 0 || particles[i].y >= GFX_HEIGHT) {
            particles[i].x = GFX_WIDTH / 2.0f; // 从中心重生
            particles[i].y = GFX_HEIGHT / 2.0f;
            particles[i].vx = ((rand() % 100) / 50.0f) - 1.0f;
            particles[i].vy = ((rand() % 100) / 50.0f) - 1.0f;
            particles[i].life = PARTICLE_MAX_LIFE;
        }

        // 将粒子绘制到缓冲区，使用不同字符代表不同生命阶段
        char c = '.';
        if (particles[i].life > PARTICLE_MAX_LIFE * 0.75) c = '*';
        else if (particles[i].life > PARTICLE_MAX_LIFE * 0.4) c = 'o';

        GFX_Draw_Char((int)particles[i].x, (int)particles[i].y, c);
    }
}
