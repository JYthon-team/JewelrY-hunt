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

void processaInput(bool* keys, SDL_Event* e, bool* running) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) *running = false;

        if (e->type == SDL_KEYDOWN) {
            keys[e->key.keysym.scancode] = true;
            if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                *running = false;
        }

        if (e->type == SDL_KEYUP)
            keys[e->key.keysym.scancode] = false;
    }
}

void moveJogador(Jogador* jogador, bool* keys, Rect* walls, int qtdWalls,
                  int vel, int* spriteX, int* spriteY, int frameW, int frameH)
{
    int newX = jogador->x;
    int newY = jogador->y;

    if (keys[SDL_SCANCODE_UP]) {
        newY -= vel;
        *spriteX = frameW;
        *spriteY = 0;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        newY += vel;
        *spriteX = 0;
        *spriteY = 0;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        newX -= vel;
        *spriteX = frameW;
        *spriteY = frameH;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        newX += vel;
        *spriteX = 0;
        *spriteY = frameH;
    }

    SDL_Rect novoRect = { newX, newY, jogador->w, jogador->h };

    if (!colideParedes(novoRect, walls, qtdWalls)) {
        jogador->x = newX;
        jogador->y = newY;
    }
}

void moveInimigos(Rect* inimigos, Jogador jogador, Rect* walls,
                  int qtdWalls, int vel, bool* derrota)
{
    for (int i = 0; i < 2; i++) {

        double dx = jogador.x - inimigos[i].x;
        double dy = jogador.y - inimigos[i].y;
        double dist = sqrt(dx*dx + dy*dy);

        if (dist > 0) {
            int stepX = (int)(dx / dist * vel);
            int stepY = (int)(dy / dist * vel);

            SDL_Rect tryX = { inimigos[i].x + stepX, inimigos[i].y,
                              inimigos[i].w, inimigos[i].h };
            SDL_Rect tryY = { inimigos[i].x, inimigos[i].y + stepY,
                              inimigos[i].w, inimigos[i].h };

            if (!colideParedes(tryX, walls, qtdWalls))
                inimigos[i].x += stepX;

            if (!colideParedes(tryY, walls, qtdWalls))
                inimigos[i].y += stepY;
        }

        SDL_Rect jr = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_Rect ir = { inimigos[i].x, inimigos[i].y, inimigos[i].w, inimigos[i].h };

        if (SDL_HasIntersection(&jr, &ir))
            *derrota = true;
    }
}

void checaColetaveis(Jogador jogador, Coletavel* joias) {
    for (int i = 0; i < 2; i++) {
        if (!joias[i].collected) {
            SDL_Rect p = { jogador.x, jogador.y, jogador.w, jogador.h };
            SDL_Rect r = { joias[i].x, joias[i].y, joias[i].w, joias[i].h };
            if (SDL_HasIntersection(&p, &r))
                joias[i].collected = true;
        }
    }
}

void checaCalice(Jogador jogador, Rect calice, Coletavel* joias, bool* faseConcluida) {
    if (joias[0].collected && joias[1].collected) {
        SDL_Rect p = { jogador.x, jogador.y, jogador.w, jogador.h };
        SDL_Rect c = { calice.x, calice.y, calice.w, calice.h };
        if (SDL_HasIntersection(&p, &c))
            *faseConcluida = true;
    }
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* ren) {
    SDL_Surface* s = IMG_Load(path);
    SDL_Texture* t = SDL_CreateTextureFromSurface(ren, s);
    SDL_FreeSurface(s);
    return t;
}

void desenhaMapa(SDL_Renderer* ren, SDL_Texture* chao, SDL_Texture* parede,
                 Rect* walls, int qtdWalls)
{
    int cw, ch, pw, ph;
    SDL_QueryTexture(chao, NULL, NULL, &cw, &ch);
    SDL_QueryTexture(parede, NULL, NULL, &pw, &ph);

    for (int y = 0; y < 600; y += ch)
        for (int x = 0; x < 800; x += cw)
            SDL_RenderCopy(ren, chao, NULL, &(SDL_Rect){x, y, cw, ch});

    for (int i = 0; i < qtdWalls; i++) {
        Rect w = walls[i];
        for (int y = w.y; y < w.y + w.h; y += ph)
            for (int x = w.x; x < w.x + w.w; x += pw) {
                int wDraw = pw, hDraw = ph;
                if (x + wDraw > w.x + w.w) wDraw = (w.x + w.w) - x;
                if (y + hDraw > w.y + w.h) hDraw = (w.y + w.h) - y;

                SDL_RenderCopy(ren, parede, &(SDL_Rect){0, 0, wDraw, hDraw},
                               &(SDL_Rect){x, y, wDraw, hDraw});
            }
    }
}

void desenhaHUD(SDL_Renderer* ren, TTF_Font* font, Uint32 startTime, int tempoTotal)
{
    int remaining = tempoTotal - (SDL_GetTicks() - startTime)/1000;
    if (remaining < 0) remaining = 0;

    char timerText[32];
    sprintf(timerText, "Tempo: %d", remaining);

    SDL_Color white = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, timerText, white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);

    SDL_Rect r = {10, 540, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &r);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void desenhaMensagem(SDL_Renderer* ren, TTF_Font* font,
                     bool faseConcluida, bool tempoEsgotado, bool derrota)
{
    const char* msg;
    SDL_Color color;

    if (faseConcluida) {
        msg = "VITORIA!";
        color = (SDL_Color){255,255,0,255};
    } else if (derrota) {
        msg = "DERROTA!";
        color = (SDL_Color){255,0,0,255};
    } else {
        msg = "TEMPO ESGOTADO!";
        color = (SDL_Color){255,0,0,255};
    }

    SDL_Surface* s = TTF_RenderText_Solid(font, msg, color);
    SDL_Texture* t = SDL_CreateTextureFromSurface(ren, s);

    SDL_Rect r = {400 - s->w/2, 280 - s->h/2, s->w, s->h};
    SDL_RenderCopy(ren, t, NULL, &r);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* win = SDL_CreateWindow("Mapa SDL2", SDL_WINDOWPOS_CENTERED,
                      SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* chaoTexture    = loadTexture("chao.png", ren);
    SDL_Texture* paredeTexture  = loadTexture("parede.png", ren);
    SDL_Texture* caliceTexture  = loadTexture("calice.png", ren);
    SDL_Texture* joiasTexture   = loadTexture("joias.png", ren);
    SDL_Texture* jogadorTexture = loadTexture("sprite.png", ren);
    SDL_Texture* inimigoTexture = loadTexture("inimigo.png", ren);

    Rect walls[] = {
        {0,0,800,16},{0,580,800,20},{0,0,16,600},{784,0,16,600},
        {100,100,200,32},{300,200,32,150},{400,300,150,32},
        {183,350,150,32},{80,200,32,150},{700,300,32,150},
        {500,150,200,32},{350,450,250,32},{0,530,230,50}
    };
    int qtdWalls = sizeof(walls)/sizeof(walls[0]);

    Jogador jogador = {100,470,50,50};
    Rect calice = {600,50,80,80};

    Coletavel joias[2] = {
        {150,400,80,80,false},
        {500,350,80,80,false}
    };

    Rect inimigos[2] = {
        {600,400,50,50},
        {200,150,50,50}
    };

    int spriteW, spriteH;
    SDL_QueryTexture(jogadorTexture, NULL, NULL, &spriteW, &spriteH);
    int frameW = spriteW/2;
    int frameH = spriteH/2;

    int spriteX = 0, spriteY = 0;

    TTF_Font* font = TTF_OpenFont("ComicNeue-BoldItalic.ttf", 48);

    bool running = true;
    bool keys[SDL_NUM_SCANCODES] = {false};
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();
    bool faseConcluida = false, tempoEsgotado = false, derrota = false;
    int tempoTotal = 20;

    while (running) {

        processaInput(keys, &e, &running);

        if (!faseConcluida && !tempoEsgotado && !derrota) {
            moveJogador(&jogador, keys, walls, qtdWalls, 4, &spriteX, &spriteY, frameW, frameH);
            moveInimigos(inimigos, jogador, walls, qtdWalls, 3, &derrota);
            checaColetaveis(jogador, joias);
            checaCalice(jogador, calice, joias, &faseConcluida);
        }

        if (((SDL_GetTicks() - startTime)/1000) >= tempoTotal)
            tempoEsgotado = true;

        SDL_SetRenderDrawColor(ren, 0,0,0,255);
        SDL_RenderClear(ren);

        desenhaMapa(ren, chaoTexture, paredeTexture, walls, qtdWalls);

        SDL_RenderCopy(ren, caliceTexture, NULL,
                       &(SDL_Rect){calice.x,calice.y,calice.w,calice.h});

        for (int i = 0; i < 2; i++)
            if (!joias[i].collected)
                SDL_RenderCopy(ren, joiasTexture, NULL,
                               &(SDL_Rect){joias[i].x,joias[i].y,joias[i].w,joias[i].h});

        SDL_RenderCopy(ren, jogadorTexture,
                       &(SDL_Rect){spriteX,spriteY,frameW,frameH},
                       &(SDL_Rect){jogador.x,jogador.y,jogador.w,jogador.h});

        for (int i = 0; i < 2; i++)
            SDL_RenderCopy(ren, inimigoTexture, NULL,
                           &(SDL_Rect){inimigos[i].x,inimigos[i].y,inimigos[i].w,inimigos[i].h});

        desenhaHUD(ren, font, startTime, tempoTotal);

        if (faseConcluida || tempoEsgotado || derrota)
            desenhaMensagem(ren, font, faseConcluida, tempoEsgotado, derrota);

        SDL_RenderPresent(ren);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
