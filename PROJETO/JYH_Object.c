#include "JYH_Header.h"

int JYH_Comp_Obj(const void* p1,const void* p2){
	JYH_Objeto* obj1 = *((JYH_Objeto**)p1);
	JYH_Objeto* obj2 = *((JYH_Objeto**)p2);
	return -(obj2->g.r.y - obj1->g.r.y);
}

SDL_Rect JYH_Tile_Get_Rect(int idx, JYH_Nivel* lvl, int z){
	int w = lvl->w;
	return (SDL_Rect){(idx%w)*z,(idx/w)*z,z,z};
}

void JYH_Obj_Colisao_Parede(const int z, JYH_Nivel* lvl, SDL_Rect* ro,int* dx, int* dy){
	SDL_Rect r = (*ro);//cria uma copia
	int w = lvl->w;
	int h = lvl->h;
	r.x += (*dx);
	r.y += (*dy);
	int idxTL = (r.x/z    )%w + w*(r.y/z    );//Top Left
	int idxTR = ((r.x+z-1)/z)%w + w*(r.y/z    );//Top Right
	int idxBL = (r.x/z    )%w + w*((r.y+z-1)/z);
	int idxBR = ((r.x+z-1)/z)%w + w*((r.y+z-1)/z);
	
	if((*dx) > 0){
		if((idxTR%w<idxTL%w)||(lvl->mat[idxTR].t&16) || (lvl->mat[idxBR].t&16)){//se têm parede à direita
			r.x -=(*dx);
			(*dx) = 0;
		}	
	}else if((*dx) < 0){
		if((idxTR%w<idxTL%w)||(lvl->mat[idxTL].t&16) || (lvl->mat[idxBL].t&16)){//se têm parede à esquerda
			r.x -= (*dx);
			(*dx) = 0;
		}
	}
	
	idxTL = (r.x/z    )%w + w*(r.y/z    );//Top Left
	idxTR = ((r.x+z-1)/z)%w + w*(r.y/z    );//Top Right
	idxBL = (r.x/z    )%w + w*((r.y+z-1)/z);//Bottom Left
	idxBR = ((r.x+z-1)/z)%w + w*((r.y+z-1)/z);//Bottom Right
	
	if((*dy)>0){
		if((idxBR>=h*w)||(lvl->mat[idxBL].t&16) || (lvl->mat[idxBR].t&16)){//se têm parede abaixo
			r.y -= (*dy);
			(*dy)=0;
		}
	}else if((*dy) < 0){
		if((idxTR < 0)||(lvl->mat[idxTL].t&16) || (lvl->mat[idxTR].t&16)){//se têm parede à esquerda
			r.y -= (*dy);
			(*dy)=0;
		}
	}
	
	(*ro) = r;
	return;

}

//Start - Inicializar de acordo com o executador
void JYH_OBJ_GENERIC_START(JYH_Obj_Generic* obj,JYH_Level_Runner* ex,  int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	obj->r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->txt = ex->txts[mat[idx].o];
	obj->s = 0;//estado inicial é sempre 0
	obj->f = 0;
	obj->n_f = 4;//fazer variar com o sprite
}
void JYH_OBJ_PLAYER_START(JYH_Obj_Player* obj,JYH_Level_Runner* ex,  int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	obj->r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->txt = ex->txts[mat[idx].o];
	obj->s = 0;//estado inicial é sempre 0
	obj->f = 0;
	obj->n_f = 4;//fazer variar com o sprite
	obj->cam = &ex->cam;//Player manipula a câmera
	obj->lvl = &ex->lvl;
	JYH_Move_Camera(obj->cam,&ex->lvl,
			-(obj->cam->r_cam.x + obj->cam->r_cam.w/2 - obj->r.x - obj->r.w/2),
			-(obj->cam->r_cam.y + obj->cam->r_cam.h/2 - obj->r.y - obj->r.h/2));
	memset(obj->K,0,sizeof(short)*4);
	
}
void JYH_OBJ_ENEMY_START(JYH_Obj_Enemy* obj,JYH_Level_Runner* ex,  int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	obj->r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->txt = ex->txts[mat[idx].o];
	obj->s = 0;//estado inicial é sempre 0
	obj->f = 0;
	obj->n_f = 1;//fazer variar com o sprite
	obj->lvl = &ex->lvl;
	for(int i = 0; i < ex->lvl.qtd_obj;i++)if(ex->lvl.objetos[i].type == JYH_OBJ_PLAYER){//procura o único player
		obj->target = &ex->lvl.objetos[i].g.r;
		break;
	}
}
void JYH_OBJ_GEM_START(JYH_Obj_Gem* obj,JYH_Level_Runner* ex,  int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	obj->r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->txt = ex->txts[mat[idx].o];
	obj->s = rand()%3;//Tipo de gem aleatório
	obj->f = 0;
	obj->n_f = 12;//fazer variar com o sprite
	ex->gem_total ++;
}
void JYH_OBJ_TROFEU_START(JYH_Obj_Trofeu* obj,JYH_Level_Runner* ex,  int idx){
	int z = ex->cam.zoom,w = ex->lvl.w;
	JYH_Tile* mat = ex->lvl.mat;
	obj->r = (SDL_Rect){z*(idx%w),z*(idx/w),z,z};
	obj->txt = ex->txts[mat[idx].o];
	obj->s = 0;//estado inicial é sempre 0
	obj->f = 0;
	obj->n_f = 12;//fazer variar com o sprite
}

//Update
void JYH_OBJ_GENERIC_UPDATE(JYH_Obj_Generic* obj,SDL_Event* evt){
	switch(evt->type){
		default:
			break;
	}
}
void JYH_OBJ_PLAYER_UPDATE(JYH_Obj_Player* obj,SDL_Event* evt){
	switch(evt->type){
		case SDL_KEYDOWN:
			switch(evt->key.keysym.sym){
				case SDLK_LEFT:obj->K[0] = 1;break;
				case SDLK_RIGHT:obj->K[1] = 1;break;
				case SDLK_UP:obj->K[2] = 1;break;
				case SDLK_DOWN:obj->K[3] = 1;break;
			}
			break;
		case SDL_KEYUP:
			switch(evt->key.keysym.sym){
				case SDLK_LEFT:obj->K[0] = 0;break;
				case SDLK_RIGHT:obj->K[1] = 0;break;
				case SDLK_UP:obj->K[2] = 0;break;
				case SDLK_DOWN:obj->K[3] = 0;break;
			}
			break;
		case SDL_USEREVENT:
			switch(evt->user.code){
				case JYH_EX_UPDATE_FRAME:
					obj->f = (obj->f+1)%(obj->n_f);
					int hor = 4*(obj->K[1]-obj->K[0]), ver = 4*(obj->K[3]-obj->K[2]);
					JYH_Obj_Colisao_Parede(obj->r.w,obj->lvl,&obj->r,&hor,&ver);
					int dist1 = obj->cam->r_cam.x + obj->cam->r_cam.w/2 - obj->r.x-obj->r.w/2;
					int dist2 = obj->cam->r_cam.y + obj->cam->r_cam.h/2 - obj->r.y-obj->r.h/2;
					JYH_Move_Camera(obj->cam,obj->lvl,-dist1,-dist2);
					if(hor)obj->s = (hor == 4)?3:2;
					if(ver)obj->s = (ver == 4)?0:1;
					
					for(int i = 1; i < obj->lvl->qtd_obj; i++){
						if(SDL_HasIntersection(&obj->r,&obj->lvl->objetos[i].g.r)){
							AUX_CriarEvento(JYH_EX_COLLISION,obj,&obj->lvl->objetos[i]);
						}
					}

					break;
			}
		default:
			break;
	}
	//colisão com as paredes
}

void JYH_OBJ_GEM_UPDATE(JYH_Obj_Gem* obj,SDL_Event* evt){
	switch(evt->type){
		case SDL_USEREVENT:
			switch(evt->user.code){
				case JYH_EX_UPDATE_FRAME:
					obj->f = (obj->f+1)%(obj->n_f);
					break;
			}
		default:
			break;
	}
}

void JYH_OBJ_TROFEU_UPDATE(JYH_Obj_Trofeu* obj,SDL_Event* evt){
	switch(evt->type){
		case SDL_USEREVENT:
			switch(evt->user.code){
				case JYH_EX_UPDATE_FRAME:
					obj->f = (obj->f+1)%(obj->n_f);
					break;
			}
		default:
			break;
	}
}
void JYH_OBJ_ENEMY_UPDATE(JYH_Obj_Enemy* obj,SDL_Event* evt){
	static int flag = 0;
	switch(evt->type){
		case SDL_USEREVENT:
			switch(evt->user.code){
				case JYH_EX_UPDATE_FRAME:
					obj->f = (obj->f+1)%(obj->n_f);
					//aproveita para se mover
					int stepX = -(obj->r.x - obj->target->x);
					int stepY = -(obj->r.y - obj->target->y);
					
					if(stepX)stepX = 2 *(stepX/abs(stepX));
					if(stepY)stepY = 2 *(stepY/abs(stepY));
					JYH_Obj_Colisao_Parede(obj->r.w,obj->lvl,&obj->r,&stepX,&stepY);
					break;
			}
		default:
			break;
	}
}


//Globais - Funções que podem ser chamadas por todos

void JYH_Update_Obj(JYH_Objeto* obj, SDL_Event* evt){

	switch(obj->type){
		case JYH_OBJ_PLAYER:
			JYH_OBJ_PLAYER_UPDATE(&obj->o_p,evt);
			break;
		case JYH_OBJ_GEM:
			JYH_OBJ_GEM_UPDATE(&obj->o_g,evt);
			break;
		case JYH_OBJ_TROFEU:
			JYH_OBJ_TROFEU_UPDATE(&obj->o_t,evt);
			break;
		case JYH_OBJ_INIMIGO:
			JYH_OBJ_ENEMY_UPDATE(&obj->o_e,evt);
			break;
		default:
			JYH_OBJ_GENERIC_UPDATE(&obj->g,evt);
			break;
			
	}

}
void JYH_Start_Obj(JYH_Objeto* obj,JYH_Level_Runner* ex,  int idx){

	switch(obj->type){
		case JYH_OBJ_PLAYER:
			JYH_OBJ_PLAYER_START(&obj->o_p,ex,idx);
			break;
		case JYH_OBJ_GEM:
			JYH_OBJ_GEM_START(&obj->o_g,ex,idx);
			break;
		case JYH_OBJ_TROFEU:
			JYH_OBJ_TROFEU_START(&obj->o_t,ex,idx);
			break;
		case JYH_OBJ_INIMIGO:
			JYH_OBJ_ENEMY_START(&obj->o_e,ex,idx);
			break;
		default://caso uma nova classe é adicionada
			JYH_OBJ_GENERIC_START(&obj->g,ex,idx);
			break;
	}

}