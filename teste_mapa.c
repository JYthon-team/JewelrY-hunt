#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef struct {
    int x, y, w, h;
    bool collected;
} Coletavel;

typedef struct {
    int x, y, w, h;
} Rect;

typedef struct {
    int x, y, w, h;
} Jogador;

bool colideParedes(SDL_Rect r, Rect* walls, int qtdWalls) {
    for (int i = 0; i < qtdWalls; i++) {
        SDL_Rect wRect = { walls[i].x, walls[i].y, walls[i].w, walls[i].h };
        if (SDL_HasIntersection(&r, &wRect)) return true;
    }
    return false;
}

int main(int argc,char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Mapa SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, 0);

    /*SDL_Surface* chaoSurface = IMG_Load("chao.png");
    SDL_Surface* paredeSurface = IMG_Load("parede.png");
    SDL_Surface* caliceSurface = IMG_Load("calice.png");
    SDL_Surface* joiasSurface = IMG_Load("joias.png");
    SDL_Surface* spriteSurface = IMG_Load("sprite.png");
    SDL_Surface* inimigoSurface = IMG_Load("inimigo.png");

    assert(chaoSurface != NULL);
    assert(paredeSurface != NULL);
    assert(caliceSurface != NULL);
    assert(joiasSurface != NULL);
    assert(spriteSurface != NULL);
    assert(inimigoSurface != NULL);*/

    SDL_Texture* chaoTexture = IMG_LoadTexture(ren,"chao.png");//SDL_CreateTextureFromSurface(ren, chaoSurface);
    SDL_Texture* paredeTexture = IMG_LoadTexture(ren,"parede.jpg");//SDL_CreateTextureFromSurface(ren, paredeSurface);
    SDL_Texture* caliceTexture = IMG_LoadTexture(ren,"calice.png");//SDL_CreateTextureFromSurface(ren, caliceSurface);
    SDL_Texture* joiasTexture = IMG_LoadTexture(ren,"joias.png");//SDL_CreateTextureFromSurface(ren, joiasSurface);
    SDL_Texture* jogadorTexture = IMG_LoadTexture(ren,"sprite.png");//SDL_CreateTextureFromSurface(ren, spriteSurface);
    SDL_Texture* inimigoTexture = IMG_LoadTexture(ren,"inimigo.png");//SDL_CreateTextureFromSurface(ren, inimigoSurface);
    
    assert(chaoTexture != NULL);
    assert(paredeTexture != NULL);
    assert(caliceTexture != NULL);
    assert(joiasTexture != NULL);
    assert(jogadorTexture != NULL);
    assert(inimigoTexture != NULL);
    
    /*SDL_FreeSurface(chaoSurface);
    SDL_FreeSurface(paredeSurface);
    SDL_FreeSurface(caliceSurface);
    SDL_FreeSurface(joiasSurface);
    SDL_FreeSurface(spriteSurface);
    SDL_FreeSurface(inimigoSurface);*/

    Rect walls[] = {
        {0, 0, 800, 64},
        {0, 536, 800, 64},
        {0, 0, 64, 600},
        {736, 0, 64, 600},
        {100, 100, 200, 32},
        {300, 200, 32, 150},
        {400, 300, 150, 32},
        {200, 350, 150, 32}
    };
    int qtdWalls = sizeof(walls)/sizeof(walls[0]);

    Jogador jogador = {100, 500, 32, 32};
    Rect calice = {600, 100, 64, 64};

    Coletavel joias[2] = {
        {150, 400, 64, 64, false},
        {500, 350, 64, 64, false}
    };

    Rect inimigo = {600, 400, 30, 30};
    double inimigoVel = 120;


    int inimigoSpriteW, inimigoSpriteH;
    SDL_QueryTexture(inimigoTexture, NULL, NULL, &inimigoSpriteW, &inimigoSpriteH);
    int inimigoFrameW = inimigoSpriteW / 2;
    int inimigoFrameH = inimigoSpriteH / 2;
    int inimigoFrameAtual = 0;
    double inimigoAnimTempo = 0;

	//Onde está esta fonte?
    //TTF_Font* font = TTF_OpenFont("ComicNeue-BoldItalic.ttf", 48);
    TTF_Font* font = TTF_OpenFont("PROJETO\\tiny.ttf", 48);

    bool running = true;
    Uint32 startTime = SDL_GetTicks();
    int tempoTotal = 20;
    bool faseConcluida = false;
    bool tempoEsgotado = false;
    bool derrota = false;

    int spriteW, spriteH;
    SDL_QueryTexture(jogadorTexture, NULL, NULL, &spriteW, &spriteH);
    int frameW = spriteW / 2;
    int frameH = spriteH / 2;

    Uint64 lastFrame = SDL_GetPerformanceCounter();
    double deltaTime = 0;
    double speed = 200;
    int colorToggle = 0;

    while (running) {
        SDL_PumpEvents();
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_ESCAPE]) break;

        Uint64 now = SDL_GetPerformanceCounter();
        deltaTime = (double)((now - lastFrame) * 1000 / (double)SDL_GetPerformanceFrequency()) / 1000.0;
        lastFrame = now;

        int move = (int)(speed * deltaTime);
        int newX = jogador.x, newY = jogador.y;
        int spriteX = 0, spriteY = 0;

        if (!faseConcluida && !tempoEsgotado && !derrota) {
            if (state[SDL_SCANCODE_UP]) { newY -= move; spriteX = frameW; spriteY = 0; }
            else if (state[SDL_SCANCODE_DOWN]) { newY += move; spriteX = 0; spriteY = 0; }
            else if (state[SDL_SCANCODE_LEFT]) { newX -= move; spriteX = frameW; spriteY = frameH; }
            else if (state[SDL_SCANCODE_RIGHT]) { newX += move; spriteX = 0; spriteY = frameH; }

            SDL_Rect novoRect = { newX, newY, jogador.w, jogador.h };
            if (!colideParedes(novoRect, walls, qtdWalls)) {
                jogador.x = newX;
                jogador.y = newY;
            }

            double dx = jogador.x - inimigo.x;
            double dy = jogador.y - inimigo.y;
            double dist = sqrt(dx * dx + dy * dy);
            if (dist > 1) {
                int stepX = (int)(dx / dist * inimigoVel * deltaTime);
                int stepY = (int)(dy / dist * inimigoVel * deltaTime);

                SDL_Rect inimigoXMove = { inimigo.x + stepX, inimigo.y, inimigo.w, inimigo.h };
                SDL_Rect inimigoYMove = { inimigo.x, inimigo.y + stepY, inimigo.w, inimigo.h };

                if (!colideParedes(inimigoXMove, walls, qtdWalls)) inimigo.x += stepX;
                if (!colideParedes(inimigoYMove, walls, qtdWalls)) inimigo.y += stepY;
            }

            inimigoAnimTempo += deltaTime;
            if (inimigoAnimTempo >= 0.2) {
                inimigoFrameAtual = (inimigoFrameAtual + 1) % 4;
                inimigoAnimTempo = 0;
            }

            for (int i = 0; i < 2; i++) {
                if (!joias[i].collected) {
                    SDL_Rect pRect = { jogador.x, jogador.y, jogador.w, jogador.h };
                    SDL_Rect jRect = { joias[i].x, joias[i].y, joias[i].w, joias[i].h };
                    if (SDL_HasIntersection(&pRect, &jRect)) joias[i].collected = true;
                }
            }
            if (joias[0].collected && joias[1].collected) {
                SDL_Rect pRect = { jogador.x, jogador.y, jogador.w, jogador.h };
                SDL_Rect cRect = { calice.x, calice.y, calice.w, calice.h };
                if (SDL_HasIntersection(&pRect, &cRect)) faseConcluida = true;
            }

            Uint32 elapsed = (SDL_GetTicks() - startTime) / 1000;
            if (elapsed >= tempoTotal) tempoEsgotado = true;

            SDL_Rect jogadorRect = { jogador.x, jogador.y, jogador.w, jogador.h };
            SDL_Rect inimigoRect = { inimigo.x, inimigo.y, inimigo.w, inimigo.h };
            if (SDL_HasIntersection(&jogadorRect, &inimigoRect)) derrota = true;
        }

        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0xFF);
        SDL_RenderClear(ren);

        int chaoW, chaoH;
        SDL_QueryTexture(chaoTexture, NULL, NULL, &chaoW, &chaoH);
        for (int y = 0; y < 600; y += chaoH) {
            for (int x = 0; x < 800; x += chaoW) {
                SDL_Rect dst = { x, y, chaoW, chaoH };
                SDL_RenderCopy(ren, chaoTexture, NULL, &dst);
            }
        }

        int paredeW, paredeH;
        SDL_QueryTexture(paredeTexture,NULL,NULL,&paredeW,&paredeH);
        for (int i=0;i<qtdWalls;i++) {
            Rect w = walls[i];
            for (int y=w.y; y<w.y+w.h; y+=paredeH) {
                for (int x=w.x; x<w.x+w.w; x+=paredeW) {
                    int wDraw = paredeW, hDraw = paredeH;
                    if (x + wDraw > w.x + w.w) wDraw = (w.x + w.w) - x;
                    if (y + hDraw > w.y + w.h) hDraw = (w.y + w.h) - y;
                    SDL_Rect src = {0,0,wDraw,hDraw};
                    SDL_Rect dst = {x,y,wDraw,hDraw};
                    SDL_RenderCopy(ren,paredeTexture,&src,&dst);
                }
            }
        }

        SDL_RenderCopy(ren, caliceTexture, NULL, (SDL_Rect*)&calice);

        for (int i=0;i<2;i++) {
            if (!joias[i].collected) {
                SDL_Rect jRect = {joias[i].x, joias[i].y, joias[i].w, joias[i].h};
                SDL_RenderCopy(ren, joiasTexture, NULL, &jRect);
            }
        }

        SDL_Rect srcRect={spriteX,spriteY,frameW,frameH};
        SDL_Rect dstRect={jogador.x,jogador.y,(int)(jogador.w*1.1),(int)(jogador.h*1.1)};
        SDL_RenderCopy(ren,jogadorTexture,&srcRect,&dstRect);

        int frameCol = inimigoFrameAtual % 2;
        int frameRow = inimigoFrameAtual / 2;
        SDL_Rect inimigoSrc = { frameCol*inimigoFrameW, frameRow*inimigoFrameH, inimigoFrameW, inimigoFrameH };
        SDL_Rect inimigoDst = { inimigo.x, inimigo.y, inimigo.w, inimigo.h };
        SDL_RenderCopy(ren, inimigoTexture, &inimigoSrc, &inimigoDst);

        int remaining = tempoTotal - (SDL_GetTicks() - startTime)/1000;
        if (remaining<0) remaining=0;
        char timerText[16]; sprintf(timerText,"Tempo: %d",remaining);
        SDL_Color white={0xFF,0xFF,0xFF,0xFF};
        SDL_Surface* timerSurface = TTF_RenderText_Solid(font,timerText,white);
        SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(ren,timerSurface);
        SDL_Rect timerRect={10,540,timerSurface->w,timerSurface->h};
        SDL_RenderCopy(ren,timerTexture,NULL,&timerRect);
        SDL_FreeSurface(timerSurface); SDL_DestroyTexture(timerTexture);

        if (faseConcluida || tempoEsgotado || derrota) {
            const char* msg;
            SDL_Color color;
            if (faseConcluida) { msg="VITORIA!"; color=(SDL_Color){0xFF,0xFF,0x00,0xFF}; }
            else if (derrota) { msg="DERROTA!"; color=(SDL_Color){0xFF,0x00,0x00,0xFF}; }
            else { msg="TEMPO ESGOTADO!"; color=(SDL_Color){0xFF,0x00,0x00,0xFF}; }

            SDL_Surface* msgSurface=TTF_RenderText_Solid(font,msg,color);
            SDL_Texture* msgTexture=SDL_CreateTextureFromSurface(ren,msgSurface);
            SDL_Rect msgRect={400 - msgSurface->w/2,280 - msgSurface->h/2,msgSurface->w,msgSurface->h};
            SDL_RenderCopy(ren,msgTexture,NULL,&msgRect);
            SDL_FreeSurface(msgSurface); SDL_DestroyTexture(msgTexture);
        }

        SDL_RenderPresent(ren);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(chaoTexture);
    SDL_DestroyTexture(paredeTexture);
    SDL_DestroyTexture(caliceTexture);
    SDL_DestroyTexture(joiasTexture);
    SDL_DestroyTexture(jogadorTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
