#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct { int x, y, w, h; bool collected; } Collectible;
typedef struct { int x, y, w, h; } Rect;
typedef struct { int x, y, w, h; } Player;

SDL_Texture* loadTexture(SDL_Renderer* ren, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return tex;
}

bool checkCollisionRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    SDL_Rect r1 = {x1, y1, w1, h1};
    SDL_Rect r2 = {x2, y2, w2, h2};
    return SDL_HasIntersection(&r1, &r2);
}

bool checkCollision(Player p, int newX, int newY, Rect walls[], int wallCount) {
    SDL_Rect future = {newX, newY, p.w, p.h};
    for (int i = 0; i < wallCount; i++) {
        SDL_Rect w = {walls[i].x, walls[i].y, walls[i].w, walls[i].h};
        if (SDL_HasIntersection(&future, &w)) return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* win = SDL_CreateWindow("Mapa SDL2 com textura no chão",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       640, 480, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_Texture* texFloor  = loadTexture(ren, "chao.png");
    SDL_Texture* texWall   = loadTexture(ren, "parede.png");
    SDL_Texture* texCalice = loadTexture(ren, "calice.png");

    Player player = {50, 50, 32, 32};
    Rect walls[] = {{0,0,640,32},{0,448,640,32},{0,0,32,480},{608,0,32,480},
                    {100,100,200,32},{300,200,32,150},{400,300,150,32},{200,350,150,32}};
    int wallCount = sizeof(walls)/sizeof(walls[0]);
    Rect calice = {500,100,32,32};
    Collectible joias[2] = {{150,400,32,32,false},{500,350,32,32,false}};

    bool running = true;
    SDL_Event e;

    while (running) {
        if (SDL_WaitEventTimeout(&e, 16)) {
            if (e.type == SDL_QUIT) running = false;
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        int newX = player.x, newY = player.y;
        if (state[SDL_SCANCODE_UP]) newY -= 4;
        if (state[SDL_SCANCODE_DOWN]) newY += 4;
        if (state[SDL_SCANCODE_LEFT]) newX -= 4;
        if (state[SDL_SCANCODE_RIGHT]) newX += 4;

        if (!checkCollision(player, newX, player.y, walls, wallCount)) player.x = newX;
        if (!checkCollision(player, player.x, newY, walls, wallCount)) player.y = newY;

        for (int i = 0; i < 2; i++) {
            if (!joias[i].collected &&
                checkCollisionRect(player.x, player.y, player.w, player.h,
                                   joias[i].x, joias[i].y, joias[i].w, joias[i].h)) {
                joias[i].collected = true;
            }
        }

        if (texFloor) {
            SDL_Rect dst = {0, 0, 640, 480};
            SDL_RenderCopy(ren, texFloor, NULL, &dst);
        } else {
            SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF);
            SDL_RenderClear(ren);
        }

        if (texWall) {
            for (int i = 0; i < wallCount; i++) {
                SDL_Rect r = {walls[i].x, walls[i].y, walls[i].w, walls[i].h};
                SDL_RenderCopy(ren, texWall, NULL, &r);
            }
        } else {
            SDL_SetRenderDrawColor(ren, 0x50,0x50,0x50,0xFF);
            for (int i = 0; i < wallCount; i++) {
                SDL_Rect r = {walls[i].x, walls[i].y, walls[i].w, walls[i].h};
                SDL_RenderFillRect(ren, &r);
            }
        }

        SDL_Rect caliceRect = {calice.x, calice.y, 32, 32};
        SDL_RenderCopy(ren, texCalice, NULL, &caliceRect);

        SDL_SetRenderDrawColor(ren,0xFF,0xFF,0x00,0xFF);
        for (int i = 0; i < 2; i++) {
            if (!joias[i].collected) {
                SDL_Rect jRect = {joias[i].x, joias[i].y, 32, 32};
                SDL_RenderFillRect(ren, &jRect);
            }
        }

        SDL_SetRenderDrawColor(ren,0xFF,0x00,0x00,0xFF);
        SDL_Rect pRect = {player.x, player.y, 32, 32};
        SDL_RenderFillRect(ren,&pRect);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(texFloor);
    SDL_DestroyTexture(texWall);
    SDL_DestroyTexture(texCalice);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
