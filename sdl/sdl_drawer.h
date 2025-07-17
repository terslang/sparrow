#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <codecvt>
#include <locale>
#include <vector>
#include <iostream>
#include <functional>
#include "shape_drawer.h"
#include "sdl_drawer_interface.h"
#include <unistd.h>

using namespace std;
namespace simple_browser_sdldrawer {

struct ResourcesStruct {
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Event event;
    SDL_Rect white_rect;

    ResourcesStruct() {
        clear();
    }
    void clear() {
        memset(this, 0, sizeof(struct ResourcesStruct));
    }

};

class SdlDrawer {
private:
    int window_width;
    int window_height;

public:
    struct ResourcesStruct res;
    SdlDrawer(int window_width, int window_height):
    window_width(window_width), window_height(window_height) {
        SDL_Init(SDL_INIT_VIDEO);
    }

    void set_window_size(int width, int height) {
        window_width = width;
        window_height = height;
        res.white_rect.w = width;
        res.white_rect.h = height;
    }
    ~SdlDrawer() {
        release_sdldrawer();
        SDL_Quit();
    }

    bool init_sdldrawer() {
        string message;

        if (!(res.window = SDL_CreateWindow("My Awesome Browser", 
            SDL_WINDOWPOS_CENTERED, 0,
            window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {
            message = "window";
            goto out;
        }
        if (!(res.render = SDL_CreateRenderer(res.window, -1, SDL_RENDERER_ACCELERATED))) {
            message = "render";
            goto out;
        }
        
        SDL_RenderClear(res.render);
        SDL_SetRenderDrawBlendMode(res.render, SDL_BLENDMODE_BLEND);

        res.white_rect.x = 0;
        res.white_rect.y = 0;
        res.white_rect.w = window_width;
        res.white_rect.h = window_height;
        return true;
    out:
        printf("Initial sdl %s error!\n", message.c_str());
        return false;
    }

    void release_sdldrawer() {
        if (res.render) {
            SDL_DestroyRenderer(res.render);
        }
        if (res.window) {
            SDL_DestroyWindow(res.window);
        }
        

        res.clear();
    }

    void run_sdldrawer(const SdlDrawerInterface& drawer_interface, std::function<void(int, int)> resize_callback) {
        int wheel_offset = 0;
        bool quit = false;
    
        SdlRectDrawer drawer(res.render);

        while(!quit) {
            while (SDL_PollEvent(&res.event)) {
                if (res.event.type == SDL_QUIT) {
                    quit = true;
                } else if (res.event.type == SDL_WINDOWEVENT) {
                    if (res.event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        int new_width = res.event.window.data1;
                        int new_height = res.event.window.data2;
                        set_window_size(new_width, new_height);
                        resize_callback(new_width, new_height);
                        SDL_RenderPresent(res.render);
                    }
                }
            }
            SDL_Delay(100);
            SDL_SetRenderDrawColor(res.render, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(res.render, &res.white_rect);
            for(int i = 0; i < drawer_interface.rect_list.size(); ++i) {
                drawer.draw(drawer_interface.rect_list[i]);
            }

            for (int i = 0; i < drawer_interface.font_list.size(); ++i) {
                SDL_SetRenderDrawColor(res.render, 
                    drawer_interface.font_list[i].background_color.r,
                    drawer_interface.font_list[i].background_color.g,
                    drawer_interface.font_list[i].background_color.b,
                    drawer_interface.font_list[i].background_color.a
                    );
                SDL_RenderFillRect(res.render, &drawer_interface.font_list[i].background_rect);
                SDL_RenderCopy(res.render, drawer_interface.font_list[i].font_texture,
                    nullptr, &drawer_interface.font_list[i].font_rect);
            }
            
            SDL_RenderPresent(res.render);
        }
    }
};

};