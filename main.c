#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
//#include "random.h"
#define dim_x 5
#define dim_y 4
SDL_Rect rect[dim_x][dim_y];
int plateau[dim_x][dim_y];
int solution[dim_x][dim_y];
SDL_Texture *image[dim_x*dim_y/2];
SDL_Texture *cache;
SDL_Window *window;
SDL_Texture *texture;
SDL_Renderer *renderer;
SDL_Surface *surface;
int clik[2];
int nbr_win=0;
int tab_win[dim_x*dim_y/2+1];

void refrech(){
    int i, j;
     for(i=0; i<dim_x; i++){
        for(j=0; j<dim_y; j++){
                if(plateau[i][j]!=0)
            SDL_RenderCopy(renderer, image[plateau[i][j]-1], NULL, &rect[i][j]);
                else
                    SDL_RenderCopy(renderer, cache, NULL, &rect[i][j]);
        }
    }
    SDL_RenderPresent(renderer);
}

void desordonne(){
    int i1, i2, i3, i4, i, n;
    srand(time(0));
    for(i=0; i<50; i++){
        i1 = rand()%dim_x;
        i2 = rand()%dim_x;
        i3 = rand()%dim_y;
        i4 = rand()%dim_y;
        n=plateau[i1][i3];
        plateau[i1][i3] = plateau[i2][i4];
        plateau[i2][i4] = n;
        n=solution[i1][i3];
        solution[i1][i3] = solution[i2][i4];
        solution[i2][i4] = n;

    }
}

int image_win(int u){
    int i;
    for(i=0; i<nbr_win; i++){
        if(tab_win[i]==u)
            return 1;
    }
    return 0;
}

void cache_tout(){
    int i, j;

    for(i=0; i<dim_x; i++){
        for(j=0; j<dim_y; j++){
                if(!image_win(plateau[i][j]))
                plateau[i][j]=0;
        }
    }
}

void initialized(){
    int i, j, c=0, d=1;
    char n[10];

    for(i=0; i<dim_x; i++){
        for(j=0; j<dim_y; j++){
            c++;
            plateau[i][j]=c;
            solution[i][j]=c;
            if(d<(dim_x*dim_y)/2+1){
                itoa(d, n, 10);
                strcat(n,".bmp");
                surface = SDL_LoadBMP(n);
                image[d-1] = SDL_CreateTextureFromSurface(renderer, surface);
                printf("%d", d);
                d++;
                }
            rect[i][j].x = 50+100*i;
            rect[i][j].y = 50+100*j;
            rect[i][j].h = 100;
            rect[i][j].w = 100;
            if(c==(dim_x*dim_y)/2)
                c=0;
        }
    }
    surface = SDL_LoadBMP("cache.bmp");
    cache= SDL_CreateTextureFromSurface(renderer, surface);
    desordonne();
    refrech();
    SDL_Delay(1000);
    cache_tout();
    refrech();
}

void search_indice(SDL_Event event){
    int i, j;
    SDL_Point point;
    point.x = event.button.x;
    point.y = event.button.y;
    clik[0]=-1;
    clik[1]=-1;
    for(i=0; i<dim_x; i++){
        for(j=0; j<dim_y; j++){
            if(SDL_PointInRect(&point, &rect[i][j])){
                clik[0]=i;
                clik[1]=j;
                break;
            }
        }
    }
}

int play(SDL_Event event){
    int i, j, u, v;
    search_indice(event);
    if(clik[0]==-1)
        return -1;
    else{
        i=clik[0];j=clik[1];
        plateau[i][j]=solution[i][j];
        refrech();
        do{
            SDL_WaitEvent(&event);
        }while(event.type!=SDL_MOUSEBUTTONDOWN);
        search_indice(event);
        if(clik[0]==-1){
            cache_tout();
            refrech();
            return -1;
        }
        else{
            u=clik[0];v=clik[1];
            plateau[u][v]=solution[u][v];
            refrech();
            if(plateau[i][j]==plateau[u][v] && (i!=u || j!=v)){
                tab_win[nbr_win]=plateau[i][j];
                nbr_win++;
                if(nbr_win==dim_x*dim_y/2)
                    return 2;
                return 1;
            }
            else {
                    SDL_Delay(1000);
                    cache_tout();
                    refrech();
                    return 0;}
        }
    }
}

int main(int argc, char *argv[]){
    int jeu;
    SDL_Event event;
    SDL_bool quit;
    quit = SDL_FALSE;
    window = SDL_CreateWindow("memory", 100, 100, 800, 800, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   // texture = SDL_CreateTexture(renderer, -1, SDL_TEXTUREACCESS_TARGET, 800, 800);
    //SDL_SetRenderTarget(renderer, texture);
    initialized();

    while(!quit){
        SDL_WaitEvent(&event);
        if(event.type == SDL_MOUSEBUTTONDOWN){
            jeu = play(event);
            if(jeu==2){
                surface = SDL_LoadBMP("winner.bmp");
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
            }
        }
        if(event.type == SDL_QUIT)
            quit = SDL_TRUE;
        SDL_Delay(50);
    }
}
