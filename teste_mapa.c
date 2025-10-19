#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

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

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Mapa SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* chaoSurface = IMG_Load("chao.png");
    SDL_Surface* paredeSurface = IMG_Load("parede.png");
    SDL_Surface* caliceSurface = IMG_Load("calice.png");
    SDL_Surface* joiasSurface = IMG_Load("joias.png");

    SDL_Texture* chaoTexture = SDL_CreateTextureFromSurface(ren, chaoSurface);
    SDL_Texture* paredeTexture = SDL_CreateTextureFromSurface(ren, paredeSurface);
    SDL_Texture* caliceTexture = SDL_CreateTextureFromSurface(ren, caliceSurface);
    SDL_Texture* joiasTexture = SDL_CreateTextureFromSurface(ren, joiasSurface);

    SDL_FreeSurface(chaoSurface);
    SDL_FreeSurface(paredeSurface);
    SDL_FreeSurface(caliceSurface);
    SDL_FreeSurface(joiasSurface);

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

    TTF_Font* font = TTF_OpenFont("ComicNeue-BoldItalic.ttf", 48);

    bool running = true;
    Uint32 startTime = SDL_GetTicks();
    int tempoTotal = 20;
    bool faseConcluida = false;
    bool tempoEsgotado = false;

    int colorToggle = 0;

    while (running) {
        SDL_PumpEvents();

        const Uint8* state = SDL_GetKeyboardState(NULL);
        int newX = jogador.x, newY = jogador.y;

        if (!faseConcluida && !tempoEsgotado) {
            if (state[SDL_SCANCODE_UP]) newY -= 2;
            if (state[SDL_SCANCODE_DOWN]) newY += 2;
            if (state[SDL_SCANCODE_LEFT]) newX -= 2;
            if (state[SDL_SCANCODE_RIGHT]) newX += 2;

            SDL_Rect novoRect = { newX, newY, jogador.w, jogador.h };
            bool colisao = false;
            for (int i = 0; i < qtdWalls; i++) {
                SDL_Rect wRect = { walls[i].x, walls[i].y, walls[i].w, walls[i].h };
                if (SDL_HasIntersection(&novoRect, &wRect)) {
                    colisao = true;
                    break;
                }
            }
            if (!colisao) {
                jogador.x = newX;
                jogador.y = newY;
            }

            for (int i = 0; i < 2; i++) {
                if (!joias[i].collected) {
                    SDL_Rect p = { jogador.x, jogador.y, jogador.w, jogador.h };
                    SDL_Rect j = { joias[i].x, joias[i].y, joias[i].w, joias[i].h };
                    if (SDL_HasIntersection(&p, &j))
                        joias[i].collected = true;
                }
            }

            if (joias[0].collected && joias[1].collected) {
                SDL_Rect p = { jogador.x, jogador.y, jogador.w, jogador.h };
                SDL_Rect c = { calice.x, calice.y, calice.w, calice.h };
                if (SDL_HasIntersection(&p, &c)) {
                    faseConcluida = true;
                }
            }

            Uint32 elapsed = (SDL_GetTicks() - startTime) / 1000;
            if (elapsed >= tempoTotal) tempoEsgotado = true;
        }

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(ren);

        int chaoW, chaoH;
        SDL_QueryTexture(chaoTexture, NULL, NULL, &chaoW, &chaoH);
        for (int y = 0; y < 600; y += chaoH)
            for (int x = 0; x < 800; x += chaoW) {
                SDL_Rect dst = {x, y, chaoW, chaoH};
                SDL_RenderCopy(ren, chaoTexture, NULL, &dst);
            }

        int paredeW, paredeH;
        SDL_QueryTexture(paredeTexture, NULL, NULL, &paredeW, &paredeH);
        for (int i = 0; i < qtdWalls; i++) {
            Rect w = walls[i];
            for (int y = w.y; y < w.y + w.h; y += paredeH)
                for (int x = w.x; x < w.x + w.w; x += paredeW) {
                    int wDraw = paredeW;
                    int hDraw = paredeH;
                    if (x + wDraw > w.x + w.w) wDraw = (w.x + w.w) - x;
                    if (y + hDraw > w.y + w.h) hDraw = (w.y + w.h) - y;
                    SDL_Rect src = {0, 0, wDraw, hDraw};
                    SDL_Rect dst = {x, y, wDraw, hDraw};
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

        SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);
        SDL_Rect playerRect = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_RenderFillRect(ren, &playerRect);

        int remaining = tempoTotal - (SDL_GetTicks() - startTime)/1000;
        if (remaining < 0) remaining = 0;
        char timerText[16];
        sprintf(timerText, "Tempo: %d", remaining);
        SDL_Color white = {255,255,255,255};
        SDL_Surface* timerSurface = TTF_RenderText_Solid(font, timerText, white);
        SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(ren, timerSurface);
        SDL_Rect timerRect = { 10, 540, timerSurface->w, timerSurface->h };
        SDL_RenderCopy(ren, timerTexture, NULL, &timerRect);
        SDL_FreeSurface(timerSurface);
        SDL_DestroyTexture(timerTexture);

        if (faseConcluida || tempoEsgotado) {
            const char* msg = faseConcluida ? "VITORIA!" : "TEMPO ESGOTADO!";
            SDL_Color colors[2] = { {255,255,0,255}, {255,0,0,255} };
            colorToggle = (colorToggle + 1) % 2;
            SDL_Surface* msgSurface = TTF_RenderText_Solid(font, msg, colors[colorToggle]);
            SDL_Texture* msgTexture = SDL_CreateTextureFromSurface(ren, msgSurface);
            SDL_Rect msgRect = { 400 - msgSurface->w/2, 280 - msgSurface->h/2, msgSurface->w, msgSurface->h };
            SDL_RenderCopy(ren, msgTexture, NULL, &msgRect);
            SDL_FreeSurface(msgSurface);
            SDL_DestroyTexture(msgTexture);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(chaoTexture);
    SDL_DestroyTexture(paredeTexture);
    SDL_DestroyTexture(caliceTexture);
    SDL_DestroyTexture(joiasTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

