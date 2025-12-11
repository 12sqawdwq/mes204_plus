#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <stdint.h>

void Video_Reset(void);
uint8_t Video_Update_Frame(void);

void Video_Init_Screen(void);

#endif