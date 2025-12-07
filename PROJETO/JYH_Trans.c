//JYH_Transições
#include "JYH_Header.h"
void JYH_EX_to_LS(JYH_GameState* jogo){
	JYH_Level_Selection temp;
	strcpy(temp.nome,jogo->ex.lvl.nome_mundo);
	JYH_Destroy_EX(&jogo->ex);
	jogo->ls = temp;
	JYH_Load_LS(jogo);
}

void JYH_EX_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	temp.lvl = jogo->ex.lvl;
	JYH_Destroy_EX(&jogo->ex);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}

void JYH_EX_to_PL(JYH_GameState* jogo){
	JYH_Level_Selection_P temp;
	JYH_Destroy_EX(&jogo->ex);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}

void JYH_LS_to_WS(JYH_GameState* jogo){
	JYH_World_Selection temp;
	AUX_Desempilha(&jogo->state);
	JYH_Destroy_LS(&jogo->ls);
	jogo->ws = temp;
	JYH_Load_WS(jogo);
}
void JYH_LS_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	strcpy(temp.lvl.nome_nivel,jogo->ls.niveis[jogo->ls.i_sel].nome_nivel);
	strcpy(temp.lvl.nome_mundo,jogo->ls.nome);
	AUX_Empilha(&jogo->state,JYH_state_EX);
	JYH_Destroy_LS(&jogo->ls);
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}
void JYH_PL_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	AUX_Empilha(&jogo->state,JYH_state_LE);
	strcpy(temp.lvl.nome_nivel,jogo->pl.niveis[jogo->pl.i_sel].nome_nivel);
	JYH_Destroy_PL(&jogo->pl);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_PL_to_MM(JYH_GameState* jogo){
	JYH_Menu temp;
	AUX_Desempilha(&jogo->state);
	JYH_Destroy_PL(&jogo->pl);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}
void JYH_PL_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	AUX_Empilha(&jogo->state, JYH_state_EX);
	JYH_Destroy_PL(&jogo->pl);
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}
void JYH_WS_to_LS(JYH_GameState* jogo){
    JYH_Level_Selection temp;
    strcpy(temp.nome,jogo->ws.mundos[jogo->ws.i_sel].nome);//copia nome do mundo
    AUX_Empilha(&jogo->state,JYH_state_LS);
    JYH_Destroy_WS(&jogo->ws);
    jogo->ls = temp;//copiao estado
    JYH_Load_LS(jogo);
}

void JYH_WS_to_MM(JYH_GameState* jogo){
    JYH_Menu temp;
    AUX_Desempilha(&jogo->state);
    JYH_Destroy_WS(&jogo->ws);
    jogo->mm = temp;
    JYH_Load_MM(jogo);
}
void JYH_LE_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	AUX_Empilha(&jogo->state,JYH_state_EX);
	JYH_Destroy_LE(&jogo->le);
	temp.lvl = jogo->le.lvl;
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}
void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	JYH_Level_Selection_P temp;
	JYH_Destroy_LE(&jogo->le);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	JYH_Menu temp;
	JYH_Destroy_LE(&jogo->le);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}

void JYH_MM_to_WS(JYH_GameState* jogo){//Menu para World Selection
    JYH_World_Selection temp;
    AUX_Empilha(&jogo->state,JYH_state_WS);
    JYH_Destroy_MM(&jogo->mm);
    jogo->ws = temp;
    JYH_Load_WS(jogo);
}
void JYH_MM_to_LE(JYH_GameState* jogo){//Menu para Level Editor
	JYH_Editor temp;
	strcpy(temp.lvl.nome_nivel,"teste");//temporario
	AUX_Empilha(&jogo->state,JYH_state_LE);
	JYH_Destroy_MM(&jogo->mm);
	
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_MM_to_PL(JYH_GameState* jogo){//Menu para Player Library
	JYH_Level_Selection_P temp;
	AUX_Empilha(&jogo->state,JYH_state_PL);
	JYH_Destroy_MM(&jogo->mm);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}

void JYH_Trans(JYH_GameState* jogo,enum  GAME_STATE orig,enum GAME_STATE dest){
	static void(*MTrans[6][6])(JYH_GameState*) =
	{    // {MM          ,LE          ,EX           ,LS          ,PL          ,WS          }
	/*MM*/	{NULL        ,JYH_MM_to_LE,NULL        ,NULL        ,JYH_MM_to_PL,JYH_MM_to_WS},
	/*LE*/	{JYH_LE_to_MM,NULL        ,JYH_LE_to_EX,NULL        ,JYH_LE_to_PL,NULL        },
	/*EX*/	{NULL        ,JYH_EX_to_LE,NULL        ,JYH_EX_to_LS,JYH_EX_to_PL,NULL        },
	/*LS*/	{NULL        ,NULL        ,JYH_LS_to_EX,NULL        ,NULL        ,JYH_LS_to_WS},
	/*PL*/	{JYH_PL_to_MM,JYH_PL_to_LE,JYH_PL_to_EX,NULL        ,NULL        ,NULL        },
	/*WS*/	{JYH_WS_to_MM,NULL        ,NULL        ,JYH_WS_to_LS,NULL        ,NULL        }
	};
	orig -= 1;dest -= 1;//desconsiderar o estado 0;
	MTrans[orig][dest](jogo);
}
void JYH_GoBack(JYH_GameState* jogo){
	enum GAME_STATE curr = AUX_Top(&jogo->state);
	AUX_Desempilha(&jogo->state);
	enum GAME_STATE prev = AUX_Top(&jogo->state);
	JYH_Trans(jogo,curr,prev);
}