//JYH_Camera.c
#include "JYH_Header.h"

/*typedef struct JYH_Camera{
    SDL_Rect r_box;//Onde na tela é exibida a câmera
    SDL_Rect r_cam;//Câmera dentro do mundo
    int zoom;
}JYH_Camera;*/

void JYH_Converter_TelaMundo(SDL_Point* p,JYH_Camera* cam/* SDL_Rect* r_editor, SDL_Rect* r_camera*/){
	//p->x = r_camera->x + p->x - r_editor->x;
	//p->y = r_camera->y + p->y - r_editor->y;
    p->x = cam->r_cam.x + p->x - cam->r_box.x;
    p->y = cam->r_cam.y + p->y - cam->r_box.y;
}
void JYH_Converter_MundoTela(SDL_Point* p,JYH_Camera* cam/* SDL_Rect* r_editor, SDL_Rect* r_camera*/){
	//p->x = (p->x-r_camera->x) + r_editor->x;
	//p->y = (p->y-r_camera->y) + r_editor->y;
    p->x = p->x - cam->r_cam.x + cam->r_box.x;
    p->y = p->y - cam->r_cam.y + cam->r_box.y;
}
void JYH_Draw_Grade_Cam(SDL_Renderer* ren/*JYH_GameState* jogo*/,JYH_Nivel* lvl, JYH_Camera* cam){
	SDL_Point p,coord1, coord2;
    //int z = jogo->le.zoom;
    int z = cam->zoom;
    SDL_Rect* r_box = &cam->r_box;
    SDL_Rect* r_cam = &cam->r_cam;

    SDL_Rect r = {0,0,z,z+z/2};
	SDL_Rect c = {0,0,32,48};
	//coord1 = (SDL_Point){jogo->le.r_camera.x/z                        ,                        jogo->le.r_camera.y/z};
    coord1 = (SDL_Point){r_box->x/z                        ,                        r_box->y/z};
	//coord2 = (SDL_Point){(jogo->le.r_camera.x + jogo->le.r_camera.w)/z,(jogo->le.r_camera.y + jogo->le.r_camera.h)/z};
    coord2 = (SDL_Point){(r_box->x + r_cam->w)/z,(r_cam->y + r_cam->h)/z};
	for(int i = coord1.x; i <= coord2.x; i++){
		if (i == lvl->w)break;
		for(int j = coord1.y; j <= coord2.y; j++){
			if (j == lvl->h)break;
            p.x = i*z;
            p.y = j*z -z/2;
			JYH_Converter_MundoTela(&p,r_box,r_cam);
			r.x = p.x;
			r.y = p.y;
			c.x = c.w*(lvl->mat[j*(lvl->w)+i]);
			SDL_RenderCopy(ren,lvl->txt_theme,&c,&r);
		}
	}
}

void JYH_Move_Camera(/*JYH_GameState* jogo*/JYH_Camera* cam,JYH_Nivel* lvl,int dx,int dy){
    //int z = jogo->le.zoom;
    int z = cam->zoom;
	//jogo->le.r_camera.x -= dx;
    cam->r_cam.x -= dx;
	//jogo->le.r_camera.y -= dy;
    cam->r_cam.y -= dy;

	//if(jogo->le.r_camera.x < 0)jogo->le.r_camera.x = 0;
    if(cam->r_cam.x < 0)cam->r_cam.x = 0;
    //else if(jogo->le.r_camera.x+jogo->le.r_camera.w  > z*jogo->le.lvl.w)jogo->le.r_camera.x = z*jogo->le.lvl.w - jogo->le.r_camera.w ;
    else if(cam->r_cam.x+cam->r_cam.w  > z*lvl->w)cam->r_cam.x = z*lvl->w - cam->r_cam.w ;
	//if(jogo->le.r_camera.y < 0)jogo->le.r_camera.y = 0;
    if(cam->r_cam.y < 0)cam->r_cam.y = 0;
    //else if(jogo->le.r_camera.y+jogo->le.r_camera.h  > z*jogo->le.lvl.h)jogo->le.r_camera.y = z*jogo->le.lvl.h - jogo->le.r_camera.h ;
    else if(cam->r_cam.y+cam->r_cam.h  > z*lvl->h)cam->r_cam.y = z*lvl->h - cam->r_cam.h ;

}
