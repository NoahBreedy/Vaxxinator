#ifndef CLAY_RENDERER_SDL2_H
#define CLAY_RENDERER_SDL2_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include "clay/clay.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t fontId;   
    TTF_Font *font;    
} SDL2_Font;

Clay_Dimensions SDL2_MeasureText(Clay_StringSlice text,
                                  Clay_TextElementConfig *config,
                                  void *userData);

void Clay_SDL2_Render(SDL_Renderer *renderer,
                      Clay_RenderCommandArray renderCommands,
                      SDL2_Font *fonts);

#ifdef __cplusplus
}
#endif

#endif