#ifndef DOS_H
#define DOS_H

#include <vector>


enum Color {BLUE=0, GREEN=1, RED=2, YELLOW=3, DOS=4};
enum Number {HASHTAG=0, ONE=1, TWO=2, THREE=3, FOUR=4, FIVE=5,
     SIX=6, SEVEN=7, EIGHT=8, NINE=9, TEN=10};

const char* ColorLUT[] = {"BLUE", "GREEN", "RED", "YELLOW", "DOS"};
const char* NumberLUT[] = {"HASHTAG", "ONE", "TWO", "THREE", "FOUR", "FIVE",
    "SIX", "SEVEN", "EIGHT", "NINE", "TEN"};

class Game {
public:
    SDL_Window* window; 
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;
    SDL_Surface* surfaceText1;
    SDL_Texture* textureText1;
    SDL_Surface* surfaceText2;
    SDL_Texture* textureText2;
    TTF_Font* myFont;
    SDL_Event event;
    bool running;


    Game();
    void initwindow();
    void initmenu();
    void render();
    std::vector<SDL_Rect*> rect_shapes;
    std::vector<SDL_Point> shape_points;
    SDL_Rect basic_rect;
    SDL_Rect menu_text_rect1;
    SDL_Rect menu_text_rect2;
    SDL_Color text_color = {192,192,192};
    std::string search_for_game_string = "Search for a game";
    std::string quit_game_string = "Quit";

} game;

#endif // DOS_H 
