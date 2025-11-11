//JYH_ImagePaths.h
#ifdef _WIN32
	//Exclusivos MM
	#define IMG_MM_BACKGROUND "img\\Menu\\Background_JYH.png"
	#define IMG_MM_TITLE "img\\Menu\\Titulo_JYH.png"
	#define IMG_MM_WS "img\\Menu\\Modo_Historia_JYH.png"
	#define IMG_MM_PL "img\\Menu\\Player_Library_JYH.png"
	#define IMG_MM_LE "img\\Menu\\Editor_JYH.png"
	//botoes
	#define IMG_B_BACK "img\\botao\\Back.png"
	#define IMG_B_SAVE "img\\botao\\Save.png"
	#define IMG_B_ZOOMIN "img\\botao\\ZoomIn.png"
	#define IMG_B_ZOOMOUT "img\\botao\\ZoomOut.png"
	#define IMG_B_RUN "img\\botao\\Run.png"
	#define IMG_B_PAINT "img\\botao\\Paint.png"
	#define IMG_B_ERASE "img\\botao\\Erase.png"
	#define IMG_B_LEFT "img\\botao\\esquerda.png"
	#define IMG_B_RIGHT "img\\botao\\direita.png"
	//Exclusivos LE
	#define IMG_LE_TB "img\\geral\\top_bar_JYH.png"
	#define IMG_LE_SB "img\\geral\\side_bar_JYH.png"
	//Exclusivos WS
	#define IMG_WS_TITLE "img\\geral\\Modo_Campanha_JYH.png"
	//Exclusivos PL
	#define IMG_PL_TITLE "img\\geral\\Biblioteca_Jogador_JYH.png"
	//temas
	#define IMG_THEME_1 "img\\geral\\tile-Sheet.png"
	
	//Paths importantes
	#define PATH_WS "JYH\\mundos.txt"
	#define PATH_PL "JYH\\MundoP\\Niveis.txt"
	#define WORLD_GET_CAPA ".\\img\\%s\\capa.png"
	#define WORLD_GET_LVLICON ".\\img\\%s\\lvlicon.png"
	#define WORLD_GET_TITLE ".\\img\\%s\\titulo.png"
	#define WORLD_GET_LEVELS ".\\JYH\\%s\\Niveis.txt"
	

#elif __linux__
	//Exclusivos MM
	#define IMG_MM_BACKGROUND "./img/menu/Background_JYH.png"
	#define IMG_MM_TITLE "./img/menu/Titulo_JYH.png"
	#define IMG_MM_WS "./img/menu/Modo_Historia_JYH.png"
	#define IMG_MM_PL "./img/menu/Player_Library_JYH.png"
	#define IMG_MM_LE "./img/menu/Editor_JYH.png"
	//botoes
	#define IMG_B_BACK "./img/botao/Back.png"
	#define IMG_B_SAVE "./img/botao/Save.png"
	#define IMG_B_ZOOMIN "./img/botao/ZoomIn.png"
	#define IMG_B_ZOOMOUT "./img/botao/ZoomOut.png"
	#define IMG_B_RUN "./img/botao/Run.png"
	#define IMG_B_PAINT "./img/botao/Paint.png"
	#define IMG_B_ERASE "./img/botao/Erase.png"
	#define IMG_B_LEFT "./img/botao/esquerda.png"
	#define IMG_B_RIGHT "./img/botao/direita.png"
	//Exclusivos LE
	#define IMG_LE_TB "./img/geral/top_bar_JYH.png"
	#define IMG_LE_SB "./img/geral/side_bar_JYH.png"
	//Exclusivos WS
	#define IMG_WS_TITLE "./img/geral/Modo_Campanha_JYH.png"
	//Exclusivos PL
	#define IMG_PL_TITLE "./img/geral/Biblioteca_Jogador_JYH.png"
	//temas
	#define IMG_THEME_1 "./img/geral/tile-Sheet.png"
	
	//Paths importantes
	#define PATH_WS "./JYH/mundos.txt"
	#define PATH_PL "./JYH/MundoP/Niveis.txt"
	#define WORLD_GET_CAPA "./img/%s/capa.png"
	#define WORLD_GET_LVLICON "./img/%s/lvlicon.png"
	#define WORLD_GET_TITLE "./img/%s/titulo.png"
	#define WORLD_GET_LEVELS "./JYH/%s/Niveis.txt"

#endif
