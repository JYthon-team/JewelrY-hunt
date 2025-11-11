//JYH_Camera.c
#include "JYH_Header.h"

void JYH_Converter_TelaMundo(SDL_Point* p,JYH_Camera* cam){
    p->x = cam->r_cam.x + p->x - cam->r_box.x;
    p->y = cam->r_cam.y + p->y - cam->r_box.y;
}
void JYH_Converter_MundoTela(SDL_Point* p,JYH_Camera* cam){
    p->x = p->x - cam->r_cam.x + cam->r_box.x;
    p->y = p->y - cam->r_cam.y + cam->r_box.y;
}
void JYH_Draw_Grade_Cam(SDL_Renderer* ren,JYH_Nivel* lvl, JYH_Camera* cam){
	SDL_Point p,coord1, coord2;
    int z = cam->zoom;
    SDL_Rect* r_box = &cam->r_box;
    SDL_Rect* r_cam = &cam->r_cam;

    SDL_Rect r = {0,0,z,z+z/2};
	SDL_Rect c = {0,0,32,48};
    coord1 = (SDL_Point){r_cam->x/z             ,             r_cam->y/z};
    coord2 = (SDL_Point){(r_cam->x + r_cam->w)/z,(r_cam->y + r_cam->h)/z};
	for(int i = coord1.x; i <= coord2.x; i++){
		if (i == lvl->w)break;
		for(int j = coord1.y; j <= coord2.y; j++){
			if (j == lvl->h)break;
            p.x = i*z;
            p.y = j*z -z/2;
			JYH_Converter_MundoTela(&p,cam);
			r.x = p.x;
			r.y = p.y;
			c.x = c.w*(lvl->mat[j*(lvl->w)+i]);
			SDL_RenderCopy(ren,lvl->txt_theme,&c,&r);
		}
	}
}

void JYH_Inicia_Camera(JYH_Camera* cam,SDL_Rect r_box,SDL_Rect r_cam, Uint32 zoom){
	cam->r_box = r_box;
	cam->r_cam = r_cam;
	cam->zoom = zoom;
}

void JYH_Move_Camera(JYH_Camera* cam,JYH_Nivel* lvl,int dx,int dy){
    int z = cam->zoom;
    cam->r_cam.x += dx;
    cam->r_cam.y += dy;
    if(cam->r_cam.x < 0)cam->r_cam.x = 0;
    else if(cam->r_cam.x+cam->r_cam.w  > z*lvl->w)cam->r_cam.x = z*lvl->w - cam->r_cam.w ;
    if(cam->r_cam.y < 0)cam->r_cam.y = 0;
    else if(cam->r_cam.y+cam->r_cam.h  > z*lvl->h)cam->r_cam.y = z*lvl->h - cam->r_cam.h ;

}
