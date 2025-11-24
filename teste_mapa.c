#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

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

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Mapa SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* chaoSurface = IMG_Load("chao.png");
    SDL_Surface* paredeSurface = IMG_Load("parede.png");
    SDL_Surface* caliceSurface = IMG_Load("calice.png");
    SDL_Surface* joiasSurface = IMG_Load("joias.png");
    SDL_Surface* spriteSurface = IMG_Load("sprite.png");
    SDL_Surface* inimigoSurface = IMG_Load("inimigo.png");

    SDL_Texture* chaoTexture = SDL_CreateTextureFromSurface(ren, chaoSurface);
    SDL_Texture* paredeTexture = SDL_CreateTextureFromSurface(ren, paredeSurface);
    SDL_Texture* caliceTexture = SDL_CreateTextureFromSurface(ren, caliceSurface);
    SDL_Texture* joiasTexture = SDL_CreateTextureFromSurface(ren, joiasSurface);
    SDL_Texture* jogadorTexture = SDL_CreateTextureFromSurface(ren, spriteSurface);
    SDL_Texture* inimigoTexture = SDL_CreateTextureFromSurface(ren, inimigoSurface);

    SDL_FreeSurface(chaoSurface);
    SDL_FreeSurface(paredeSurface);
    SDL_FreeSurface(caliceSurface);
    SDL_FreeSurface(joiasSurface);
    SDL_FreeSurface(spriteSurface);
    SDL_FreeSurface(inimigoSurface);

    Rect walls[] = {
        {0, 0, 800, 16},  {0, 580, 800, 20}, {0, 0, 16, 600}, {788, 0, 16, 600},
        {100, 100, 200, 32}, {300, 200, 32, 150}, {400, 300, 150, 32},
        {183, 350, 150, 32}, {80, 200, 32, 150}, {700, 300, 32, 150},
        {500, 150, 200, 32}, {350, 450, 250, 32}, {0, 530, 230, 100}
    };
    int qtdWalls = sizeof(walls) / sizeof(walls[0]);

    Jogador jogador = {100, 470, 50, 50};
    Rect calice = {600, 50, 80, 80};

    Coletavel joias[2] = {
        {150, 400, 80, 80, false},
        {500, 350, 80, 80, false}
    };

    int jogadorVel = 4;
    int inimigoVel = 3;

    Rect inimigos[2] = {
        {600, 400, 50, 50},
        {200, 150, 50, 50}
    };

    int inimigoSpriteW, inimigoSpriteH;
    SDL_QueryTexture(inimigoTexture, NULL, NULL, &inimigoSpriteW, &inimigoSpriteH);
    int inimigoFrameW = inimigoSpriteW / 2;
    int inimigoFrameH = inimigoSpriteH / 2;
    int inimigoFrameAtual = 0;
    double inimigoAnimTempo = 0;

    TTF_Font* font = TTF_OpenFont("ComicNeue-BoldItalic.ttf", 48);
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

    int spriteX = 0, spriteY = 0;

    bool keys[SDL_NUM_SCANCODES] = {false};
    SDL_Event e;

    while (running) {

        while (SDL_WaitEventTimeout(&e, 5)) {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_KEYDOWN) {
                keys[e.key.keysym.scancode] = true;
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
            }
            if (e.type == SDL_KEYUP) {
                keys[e.key.keysym.scancode] = false;
            }
        }
        if (!running) break;

if (!faseConcluida && !tempoEsgotado && !derrota) {

    int newX = jogador.x;
    int newY = jogador.y;

    if (keys[SDL_SCANCODE_UP]) {
        newY -= jogadorVel;
        spriteX = frameW;
        spriteY = 0;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        newY += jogadorVel;
        spriteX = 0;
        spriteY = 0;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        newX -= jogadorVel;
        spriteX = frameW;
        spriteY = frameH;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        newX += jogadorVel;
        spriteX = 0;
        spriteY = frameH;
    }

    SDL_Rect novoRect = { newX, newY, jogador.w, jogador.h };
    if (!colideParedes(novoRect, walls, qtdWalls)) {
        jogador.x = newX;
        jogador.y = newY;
    }

    for (int i = 0; i < 2; i++) {

        double dx = jogador.x - inimigos[i].x;
        double dy = jogador.y - inimigos[i].y;
        double dist = sqrt(dx*dx + dy*dy);

        if (dist > 0) {
            int stepX = (int)(dx / dist * inimigoVel);
            int stepY = (int)(dy / dist * inimigoVel);

            SDL_Rect iMoveX = { inimigos[i].x + stepX, inimigos[i].y, inimigos[i].w, inimigos[i].h };
            SDL_Rect iMoveY = { inimigos[i].x, inimigos[i].y + stepY, inimigos[i].w, inimigos[i].h };

            if (!colideParedes(iMoveX, walls, qtdWalls)) inimigos[i].x += stepX;
            if (!colideParedes(iMoveY, walls, qtdWalls)) inimigos[i].y += stepY;
        }

        SDL_Rect jR = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_Rect iR = { inimigos[i].x, inimigos[i].y, inimigos[i].w, inimigos[i].h };
        if (SDL_HasIntersection(&jR, &iR)) derrota = true;
    }

    for (int i = 0; i < 2; i++) {
        if (!joias[i].collected) {
            SDL_Rect pR = { jogador.x, jogador.y, jogador.w, jogador.h };
            SDL_Rect jR = { joias[i].x, joias[i].y, joias[i].w, joias[i].h };
            if (SDL_HasIntersection(&pR, &jR))
                joias[i].collected = true;
        }
    }

    if (joias[0].collected && joias[1].collected) {
        SDL_Rect pR = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_Rect cR = { calice.x, calice.y, calice.w, calice.h };
        if (SDL_HasIntersection(&pR, &cR))
            faseConcluida = true;
    }
}

        if (((SDL_GetTicks() - startTime) / 1000) >= tempoTotal)
            tempoEsgotado = true;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        int chaoW, chaoH;
        SDL_QueryTexture(chaoTexture, NULL, NULL, &chaoW, &chaoH);
        for (int y = 0; y < 600; y += chaoH)
            for (int x = 0; x < 800; x += chaoW) {
                SDL_Rect dst = { x, y, chaoW, chaoH };
                SDL_RenderCopy(ren, chaoTexture, NULL, &dst);
            }

        int paredeW, paredeH;
        SDL_QueryTexture(paredeTexture, NULL, NULL, &paredeW, &paredeH);
        for (int i = 0; i < qtdWalls; i++) {
            Rect w = walls[i];
            for (int y = w.y; y < w.y + w.h; y += paredeH)
                for (int x = w.x; x < w.x + w.w; x += paredeW) {
                    int wDraw = paredeW, hDraw = paredeH;
                    if (x + wDraw > w.x + w.w) wDraw = (w.x + w.w) - x;
                    if (y + hDraw > w.y + w.h) hDraw = (w.y + w.h) - y;
                    SDL_Rect src = { 0, 0, wDraw, hDraw };
                    SDL_Rect dst = { x, y, wDraw, hDraw };
                    SDL_RenderCopy(ren, paredeTexture, &src, &dst);
                }
        }

        SDL_RenderCopy(ren, caliceTexture, NULL, (SDL_Rect*)&calice);

        for (int i = 0; i < 2; i++) {
            if (!joias[i].collected) {
                SDL_Rect jRect = { joias[i].x, joias[i].y, joias[i].w, joias[i].h };
                SDL_RenderCopy(ren, joiasTexture, NULL, &jRect);
            }
        }

        SDL_Rect srcRect = { spriteX, spriteY, frameW, frameH };
        SDL_Rect dstRect = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_RenderCopy(ren, jogadorTexture, &srcRect, &dstRect);

        int frameCol = inimigoFrameAtual % 2;
        int frameRow = inimigoFrameAtual / 2;
        SDL_Rect inimigoSrc = { frameCol * inimigoFrameW, frameRow * inimigoFrameH, inimigoFrameW, inimigoFrameH };

        for (int i = 0; i < 2; i++) {
            SDL_Rect dst = { inimigos[i].x, inimigos[i].y, inimigos[i].w, inimigos[i].h };
            SDL_RenderCopy(ren, inimigoTexture, &inimigoSrc, &dst);
        }

        int remaining = tempoTotal - (SDL_GetTicks() - startTime) / 1000;
        if (remaining < 0) remaining = 0;

        char timerText[16];
        sprintf(timerText, "Tempo: %d", remaining);
        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Surface* timerSurface = TTF_RenderText_Solid(font, timerText, white);
        SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(ren, timerSurface);
        SDL_Rect timerRect = { 10, 540, timerSurface->w, timerSurface->h };
        SDL_RenderCopy(ren, timerTexture, NULL, &timerRect);
        SDL_FreeSurface(timerSurface);
        SDL_DestroyTexture(timerTexture);

        if (faseConcluida || tempoEsgotado || derrota) {
            const char* msg;
            SDL_Color color;
            if (faseConcluida) {
                msg = "VITORIA!";
                color = (SDL_Color){255, 255, 0, 255};
            } else if (derrota) {
                msg = "DERROTA!";
                color = (SDL_Color){255, 0, 0, 255};
            } else {
                msg = "TEMPO ESGOTADO!";
                color = (SDL_Color){255, 0, 0, 255};
            }
            SDL_Surface* msgSurface = TTF_RenderText_Solid(font, msg, color);
            SDL_Texture* msgTexture = SDL_CreateTextureFromSurface(ren, msgSurface);
            SDL_Rect msgRect = { 400 - msgSurface->w / 2, 280 - msgSurface->h / 2, msgSurface->w, msgSurface->h };
            SDL_RenderCopy(ren, msgTexture, NULL, &msgRect);
            SDL_FreeSurface(msgSurface);
            SDL_DestroyTexture(msgTexture);
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
