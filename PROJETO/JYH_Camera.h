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

typedef struct JYH_Nivel JYH_Nivel;
void JYH_Converter_TelaMundo(SDL_Point* p,JYH_Camera* cam);
void JYH_Converter_MundoTela(SDL_Point* p,JYH_Camera* cam);
void JYH_Move_Camera(JYH_Camera* cam,JYH_Nivel* lvl,int dx,int dy);
void JYH_Inicia_Camera(JYH_Camera* cam,SDL_Rect r_box,SDL_Rect r_cam, Uint32 zoom);
#endif