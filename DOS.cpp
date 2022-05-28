#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "DOS.h"
#include "networking.h"

// Screen dimension constants
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

// Statemachines
enum GameState {LAUNCH, MENU, PLAY, MATCHMAKING, PREGAME, INGAME} GameState;
enum MatchState {MY_TURN, OPP_TURN} MatchState;
enum MenuCursorState {START, QUIT} MenuCursorState;


Game::Game() {
	printf("Constructing game");
	SDL_Window* window = nullptr;
	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;
	SDL_Surface* surfaceText = nullptr;
	SDL_Texture* textureText = nullptr;
	TTF_Font* myFont = nullptr;
	//Renderer
	SDL_Renderer *renderer = nullptr;
	SDL_Event event;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not be initialized: ", SDL_GetError());
	}
	else {
		printf("Video system ready to go!");
	}
	running = true;
}

void Game::initwindow() {
		window = SDL_CreateWindow( "GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		// Create new renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		screenSurface = SDL_GetWindowSurface(window);
}

void Game::initmenu() {
		// Create the cursor
		SDL_Point a = {250, 150}, b = {225, 125}, c = {225, 175};
		game.shape_points.push_back(a);
		game.shape_points.push_back(b);
		game.shape_points.push_back(c);
		SDL_Rect rectangle;
		game.menu_text_rect1.x = 275;
		game.menu_text_rect1.y = 125;
		game.menu_text_rect1.w = 275;
		game.menu_text_rect1.h = 50;

		game.menu_text_rect2.x = 275;
		game.menu_text_rect2.y = 200;
		game.menu_text_rect2.w = 75;
		game.menu_text_rect2.h = 50;
}


void Game::render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
	//SDL_RenderDrawRect(renderer, &game.menu_text_rect);
	SDL_SetRenderDrawColor(renderer,255,255,255,0);
	//SDL_RenderDrawRect(game.renderer, &game.menu_text_rect); // Box around the button
	SDL_RenderDrawLine(renderer, game.shape_points[0].x, game.shape_points[0].y,
		 game.shape_points[1].x, game.shape_points[1].y);
	SDL_RenderDrawLine(renderer, game.shape_points[1].x, game.shape_points[1].y,
 		game.shape_points[2].x, game.shape_points[2].y);
	SDL_RenderDrawLine(renderer, game.shape_points[2].x, game.shape_points[2].y,
	 	game.shape_points[0].x, game.shape_points[0].y);

	char test_str[128];
	sprintf(test_str, "Search for game");
	game.surfaceText1 = TTF_RenderText_Solid(game.myFont,"Search for a game",text_color);
	game.textureText1 = SDL_CreateTextureFromSurface(game.renderer,game.surfaceText1);

	game.surfaceText2 = TTF_RenderText_Solid(game.myFont,"Quit",game.text_color);
	game.textureText2 = SDL_CreateTextureFromSurface(game.renderer,game.surfaceText2);
	// Free the surface
	// We are done with it after we have uploaded to
	// the texture
	SDL_FreeSurface(game.surfaceText1);
	SDL_FreeSurface(game.surfaceText2);
	// Render our text on a rectangle

	SDL_RenderCopy(game.renderer,game.textureText1,NULL,&game.menu_text_rect1);
	SDL_RenderCopy(game.renderer,game.textureText2,NULL,&game.menu_text_rect2);
	SDL_RenderPresent(game.renderer);
}


int main(int argc, char *argv[]) {
	const Uint8 *kbm_state = SDL_GetKeyboardState(NULL);
    int new_socket = init_socket(); // Initialize a socket
    char *outgoing_matchmaking_message = "MATCHMAKING";
	char *outgoing_disconnect_message = "DISCONNECT";

	bool down_key_switch, up_key_switch;

    game.initwindow();
	game.initmenu();
	if(TTF_Init() == -1) {
		std::cout << "Could not initailize SDL2_ttf, error: " << TTF_GetError() << std::endl;
	}
	else {
		std::cout << "SDL2_ttf system ready to go!" << std::endl;
	}

	// Load our font file and set the font size
	game.myFont = TTF_OpenFont("./fonts/Lato-Light.ttf", 128);
	// Confirm that it was loaded
	if(game.myFont == nullptr){
		std::cout << "Could not load font" << std::endl;
		exit(1);
	}




	// Create a rectangle to draw on


	GameState = MENU;


    while (game.running) {
    	auto time = SDL_GetTicks();
        while(SDL_PollEvent(&game.event)) {
            switch(game.event.type) {
            case SDL_QUIT:
                // shut down
                game.running = false;
				send_to_server(new_socket, outgoing_disconnect_message);
                break;
			case SDL_KEYDOWN:
				if(game.event.key.keysym.sym == SDLK_DOWN) {
					for (int i=0;i<game.shape_points.size();i++) {
						game.shape_points[i].y += 75;
					}
					if (game.shape_points[1].y == game.menu_text_rect1.y) {
						MenuCursorState = START;
					}
					else if (game.shape_points[1].y == game.menu_text_rect2.y) {
						MenuCursorState = QUIT;
					}
				}
				break;
			case SDL_KEYUP:
				if(game.event.key.keysym.sym == SDLK_UP) {
					for (int i=0;i<game.shape_points.size();i++) {
						game.shape_points[i].y -= 75;
					}
					if (game.shape_points[1].y == game.menu_text_rect1.y) {
						MenuCursorState = START;
					}
					else if (game.shape_points[1].y == game.menu_text_rect2.y) {
						MenuCursorState = QUIT;
					}
				}
				break;
			}
		}

		if (kbm_state[SDL_SCANCODE_RETURN] && GameState == MENU) {
			if (MenuCursorState == START) {
				// start matchmaking
				GameState = PLAY;
				printf("Entered MM\n");
			}
			else if (MenuCursorState == QUIT) {
				// quit game
				printf("Quiting\n");
				game.running = false;
				send_to_server(new_socket, outgoing_disconnect_message);
			}

		}

		if (GameState == PLAY) {
			printf("starting to send\n");
			send_to_server(new_socket, outgoing_matchmaking_message);  // Inform the server about started matchmaking
			printf("sent to server\n");
			GameState = MATCHMAKING;
		}

		if (GameState == MATCHMAKING) {
			receive_from_server(new_socket);
			GameState = PREGAME;
		}

		if (kbm_state[SDL_SCANCODE_Q]) {
			send_to_server(new_socket, "QUIT SERVER");
		}
		game.render();

		if ((SDL_GetTicks() - time) < 10) {
    		SDL_Delay(10);
  		}
  		SDL_DestroyTexture(game.textureText1);
		SDL_DestroyTexture(game.textureText2);



	}


    SDL_DestroyWindow(game.window);
    TTF_CloseFont(game.myFont);
    SDL_Quit();



	exit(0);
    return 0;
}
