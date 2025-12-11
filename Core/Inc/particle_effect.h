#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

#include "graphics.h" // For GFX_WIDTH, GFX_HEIGHT

// --- 粒子系统参数 ---
#define MAX_PARTICLES 80
#define PARTICLE_MAX_LIFE 50

// --- 粒子系统函数原型 ---
void Particle_Init(void);
void Particle_UpdateAndRender(void);

#endif // PARTICLE_EFFECT_H
