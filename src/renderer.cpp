#include "renderer.h"
#include <iostream>
#include <string>
#include "Map.h"
int count=1;
Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() == -1) {
    std::cerr << "SDL could not initialize SDL_ttf.\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Add tile images
  Tilemap *tilemap = Tilemap::instance();
  tilemap->init(sdl_renderer, 32, 32);
  tilemap->addTile("../assets/grass.png", "grass");
  tilemap->addTile("../assets/apple.png", "food");
  tilemap->addTile("../assets/snake_head.png", "snake_head");
  tilemap->addTile("../assets/snake_body_even.png", "snake_body_even");
  tilemap->addTile("../assets/snake_body_odd.png", "snake_body_odd");
  tilemap->addTile("../assets/snake_head_dead.png", "snake_head_dead");
  tilemap->addTile("../assets/burger.png", "burger");
  tilemap->addTile("../assets/Box.png", "Box");
  tilemap->addTile("../assets/building.png", "Building");
  tilemap->addTile("../assets/mountain.png", "mountain");
  tilemap->addTile("../assets/frog.png", "bonus");
  tilemap->addTile("../assets/hole.png", "hole");

  //Create Text
  std::unique_ptr<Text> ptr_text(new Text( "../assets/FreeSans.ttf"));
  text = std::move(ptr_text);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
  TTF_Quit();
}

void Renderer::Render(Snake &snake, SDL_Point const &food, Bonus const &bonus,std::vector<obstacle> obs) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render grass
  Tilemap::instance()->fillWith("grass", 0, 0, screen_width, screen_height);

  // Render food
  Tilemap::instance()->render("food", food.x * block.w, food.y * block.h);
  for(auto o:obs){
    if(snake.SnakeCell(o._xpos,o._ypos)){
    	snake.decLive();
      text->display(screen_width - 450, screen_height - 80, sdl_renderer, 30, "Remaining Lives:::"+std::to_string(snake.getLives()), {0xFF, 0xFF, 0xFF, 0xFF});}
    if(o._type==0)
      Tilemap::instance()->render("Box", o._xpos * block.w, o._ypos * block.h);
    else if(o._type==1)
      Tilemap::instance()->render("Building", o._xpos * block.w, o._ypos * block.h);
    else if(o._type==2)
      Tilemap::instance()->render("mountain", o._xpos * block.w, o._ypos * block.h);
  }

  // Render bonus
  if (bonus.active)
    Tilemap::instance()->render("bonus", bonus.loc.x * block.w, bonus.loc.y * block.h);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
     block.x = point.x * block.w;
    block.y = point.y * block.h;
    if(count%2==0){
    Tilemap::instance()->render("snake_body_odd", block.x, block.y);}
    else{
      Tilemap::instance()->render("snake_body_even", block.x, block.y);
    }
    count++;
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    Tilemap::instance()->render("snake_head", block.x, block.y);
  } else {
    Tilemap::instance()->render("snake_head_dead", block.x, block.y);
    text->display(screen_width - 380, screen_height - 25, sdl_renderer, 18, "Game Over!", {0xFF, 0x00, 0x00, 0xFF});
    text->display(screen_width - 450, screen_height - 80, sdl_renderer, 30, "Press Enter to Reset", {0xFF, 0xFF, 0xFF, 0xFF});
  }

  

  
  if (snake.alive) {
    text->display(screen_width - 380, screen_height - 25, sdl_renderer, 18, "Enemy Dead!", {0xFF, 0xFF, 0xFF, 0xFF});
  }

  //Render score
  text->display(5, screen_height - 25, sdl_renderer, 18, "Score: " + std::to_string(snake.score), {0xFF, 0xFF, 0xFF, 0xFF});

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"Snake Game | FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}