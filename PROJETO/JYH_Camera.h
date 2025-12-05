//Camera.h
#ifndef JYH_CAM_L
#define JYH_CAM_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Camera{
    SDL_Rect r_box;
    SDL_Rect r_cam;
    Uint32 zoom;
}JYH_Camera;

void JYH_Converter_TelaMundo(SDL_Point* p,JYH_Camera* cam);
void JYH_Converter_MundoTela(SDL_Point* p,JYH_Camera* cam);
#endif