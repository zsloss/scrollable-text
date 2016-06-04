#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include "ScrollableText.h"
#include <memory>

const int SCREEN_WIDTH = 400, SCREEN_HEIGHT = 300;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *font = nullptr;
SDL_Texture *texture = nullptr;
std::shared_ptr<ScrollableText> scrollable_text;

bool init();
bool init_font();
std::string get_lorem_ipsum();
void cleanup();

int main(int argc, char* argv[]) {
	printf("App started!\n");

	bool quit = false;
	SDL_Event e;
	if (init()) {
		while (!quit) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
					quit = true;
				}
			}

			const Uint8 *keystate = SDL_GetKeyboardState(NULL);

			scrollable_text->update(keystate);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			scrollable_text->render(0, 0);

			SDL_RenderPresent(renderer);
		}
	}

	cleanup();
	return 0;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) != -1)
		if (window = SDL_CreateWindow("Zac's Amazing Scrollable Text!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))
			if (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))
				if (init_font()) {
						scrollable_text = std::make_shared<ScrollableText>(renderer, font, SCREEN_WIDTH, SCREEN_HEIGHT);		
						scrollable_text->set_text(get_lorem_ipsum());
						return true;
					}
					else printf("Get renderer error: %s\n", SDL_GetError());
				else printf("Create window error: %s\n", SDL_GetError());
				return false;
}

bool init_font() {
	if (TTF_Init() == 0)
		if (font = TTF_OpenFont("OpenSans-Regular.ttf", 32))
			return true;
		else
			printf("TTF_OpenFont error: %s", TTF_GetError());
	else
		printf("TTF_Init error: %s", TTF_GetError());

	return false;
}

void cleanup() {
	scrollable_text = nullptr;
	TTF_CloseFont(font);
	font = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
}

std::string get_lorem_ipsum() {
	return "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas vel lacus a nibh pellentesque efficitur. Curabitur ac nulla a est maximus malesuada sit amet sit amet enim. In euismod iaculis ligula, ac congue justo dignissim ac. In consequat in odio nec malesuada. Pellentesque feugiat orci nec nunc vestibulum, eu imperdiet lorem imperdiet. Maecenas eu fringilla mauris, at fringilla massa. Cras sollicitudin ultricies velit, id tristique eros.";
}