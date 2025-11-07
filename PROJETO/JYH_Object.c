#include "JYH_Header.h"

//Objetos
int JYH_Colisao_Parede(JYH_GameState* jogo, JYH_Objeto* obj1){//Utiliza a matriz do Nível
	return 1;
}
int JYH_Colisao_Objeto(JYH_Objeto* obj1, JYH_Objeto* obj2){//Utiliza o SDL_Rect para dizer se dois retângulos se intersecionam
	return SDL_HasIntersection(&obj1->hitbox,&obj2->hitbox);//posição
}

void JYH_Update_Obj(JYH_GameState* jogo, JYH_Objeto* obj){
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
}
void JYH_Start_Obj(JYH_GameState* jogo, JYH_Objeto* obj){
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
}
void JYH_TimeUpdate_Obj(JYH_GameState* jogo,JYH_Objeto* obj){//atualiza frames e outras coisas
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
}