#include <SDL.h>
#include <cstdio>
#include <string>
#include "ScrollableText.h"
#include <memory>
#include <SDL_image.h>

const int SCREEN_WIDTH = 400, SCREEN_HEIGHT = 300;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
std::shared_ptr<ScrollableText> scrollable_text1;
std::shared_ptr<ScrollableText> scrollable_text2;

bool init();
std::string get_lorem_ipsum();
void cleanup();

int main(int argc, char* argv[]) {
	printf("App started!\n");

	bool quit = false;
	bool in_1 = false, in_2 = false;


	SDL_Event e;
	if (init()) {
		while (!quit) {
			int mousewheel = 0;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
					quit = true;
				}
				else if (e.type == SDL_MOUSEMOTION) {
					int mx, my;
					SDL_GetMouseState(&mx, &my);
					if (my > 0 && my < SCREEN_HEIGHT)
						if (mx > 0 && mx < SCREEN_WIDTH / 2) {
							in_1 = true; in_2 = false;
						}
						else {
							in_1 = false; in_2 = true;
						}
					else { in_1 = false; in_2 = false; }
				}
				else if (e.type == SDL_MOUSEWHEEL)
					mousewheel = e.wheel.y;
			}

			const Uint8 *keystate = SDL_GetKeyboardState(NULL);

			if (in_1)
				scrollable_text1->update(keystate, mousewheel);
			else if (in_2)
				scrollable_text2->update(keystate, mousewheel);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			scrollable_text1->render(0, 0);
			scrollable_text2->render(SCREEN_WIDTH / 2, 0);

			SDL_RenderPresent(renderer);
		}
	}

	cleanup();
	return 0;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) != -1)
		if (window = SDL_CreateWindow("Zac's Amazing Scrollable Text!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))
			if (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) {
				int imgFlags = IMG_INIT_PNG;
				if (IMG_Init(imgFlags) & imgFlags)	{
					scrollable_text1 = std::make_shared<ScrollableText>(renderer, "nova", SCREEN_WIDTH / 2, SCREEN_HEIGHT);
					scrollable_text1->set_text(get_lorem_ipsum());
					scrollable_text2 = std::make_shared<ScrollableText>(renderer, "nova", SCREEN_WIDTH / 2, SCREEN_HEIGHT);
					scrollable_text2->set_text(get_lorem_ipsum());
					return true;
				}
				else printf("IMG_Init error: %s\n", IMG_GetError());
			}
			else printf("Get renderer error: %s\n", SDL_GetError());
		else printf("Create window error: %s\n", SDL_GetError());
		return false;
}

void cleanup() {
	scrollable_text1 = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
}

std::string get_lorem_ipsum() {
	return "Philosophy being nothing else but THE STUDY OF WISDOM AND TRUTH, it may with reason be expected that those who have spent most time and pains in it should enjoy a greater calm and serenity of mind, a greater clearness and evidence of knowledge, and be less disturbed with doubts and difficulties than other men. Yet so it is, we see the illiterate bulk of mankind that walk the high - road of plain common sense, and are governed by the dictates of nature, for the most part easy and undisturbed. To them nothing THAT IS FAMILIAR appears unaccountable or difficult to comprehend. They complain not of any want of evidence in their senses, and are out of all danger of becoming SCEPTICS.But no sooner do we depart from sense and instinct to follow the light of a superior principle, to reason, meditate, and reflect on the nature of things, but a thousand scruples spring up in our minds concerning those things which before we seemed fully to comprehend. Prejudices and errors of sense do from all parts discover themselves to our view; and, endeavouring to correct these by reason, we are insensibly drawn into uncouth paradoxes, difficulties, and inconsistencies, which multiply and grow upon us as we advance in speculation, till at length, having wandered through many intricate mazes, we find ourselves just where we were, or, which is worse, sit down in a forlorn Scepticism.";
}