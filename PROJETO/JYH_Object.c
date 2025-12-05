#include "JYH_Object.h"
#include "JYH_EX.h"
#include <stdio.h>
/*void JYH_Update_Obj(JYH_GameState* jogo, JYH_Objeto* obj){
	switch(obj->type){
		case JYH_OBJ_PLAYER:
			//JYH_OBJ_PLAYER_UPDATE();
			break;
		case JYH_OBJ_GEM:
			//JYH_OBJ_GEM_UPDATE();
			break;
		case JYH_OBJ_TROFEU:
			//JYH_OBJ_TROFEU_UPDATE();
			break;
		case JYH_OBJ_INIMIGO:
			//JYH_OBJ_ENEMY_UPDATE();
			break;
	}
}*/
void JYH_Start_Obj(JYH_Level_Runner* ex, JYH_Objeto* obj, int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	printf("z = %d\n",z);
	obj->g.r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->g.txt = ex->txts[mat[idx].o];
	obj->g.s = 0;//estado inicial é sempre 0
	obj->g.f = 0;
	obj->g.n_f = 4;//fazer variar com o sprite
	
	SDL_Rect r = obj->g.r;
	printf("%d -> R{%d, %d, %d, %d}\n",idx,r.x,r.y,r.w,r.h);
	
	switch(obj->type){
		case JYH_OBJ_PLAYER:
			//JYH_OBJ_PLAYER_START(&obj->o_p,ex,idx);
			break;
		case JYH_OBJ_GEM:
			//JYH_OBJ_GEM_START(&obj->o_g,ex,idx);
			break;
		case JYH_OBJ_TROFEU:
			//JYH_OBJ_TROFEU_START(&obj->o_t,ex,idx);
			break;
		case JYH_OBJ_INIMIGO:
			//JYH_OBJ_ENEMY_START(&obj->o_e,ex,idx);
			break;
	}
}
/*void JYH_Draw_Obj(JYH_GameState* jogo,JYH_Objeto* obj){//atualiza frames e outras coisas
	switch(obj->type){
		case JYH_OBJ_PLAYER:
			
			break;
		case JYH_OBJ_GEM:
			
			break;
		case JYH_OBJ_TROFEU:
			
			break;
		case JYH_OBJ_INIMIGO:
			
			break;
	}
}*/