#include "game.h"
#include <iostream>
#include "SDL.h"
#include "debug.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)-1),
      random_h(0, static_cast<int>(grid_height)-1),
	  random_m(3,10),
      bonus_interval(20, 40) {
  PlaceFood();
  bonus.active = false;
  bonus.interval = 10;
}
void Game::clearObstacles(std::vector<obstacle> &obs){
}
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  long frame_count = 0;
  bool running = true;
  bonus_timer_start = SDL_GetTicks();
  std::vector<obstacle> Obstacles;
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Uint32 elapsed_time;
  	Uint32 bonus_timer_end = SDL_GetTicks();
  	elapsed_time =  bonus_timer_end - bonus_timer_start;
  	// Place bonus every `interval` seconds
  	if (bonus_timer_end - bonus_timer_start >= bonus.interval * 1000) {
      bonus_timer_start = bonus_timer_end;
       Obstacles=placeObstacles();}
    Update();
    renderer.Render(snake, food, bonus,Obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}
std::vector<obstacle> Game::placeObstacles(){
  obs.clear();
  bool check=false;
  int s = random_m(engine);
  int x;
  int y;
  for(int i=1; i<=s; i++){
    int x = random_w(engine);
    int y = random_h(engine);
    for(auto it:obs){
    	if(it._xpos==x && it._ypos==y){
        	check=true;
          	break;
        }
    }
    if(check || snake.SnakeCell(x,y)){check=false;continue;}
    //int t = random_l(engine);
    obs.push_back(obstacle(x,y));
  }
  return obs;
}
void Game::Update() {
  if (!snake.alive) return;
  //placeObstacles();
  snake.Update();
  UpdateBonus();
  ConsumeFoodOrBonus(snake);

}

void Game::PlaceBonus() {
  bonus.active = true;
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      bonus.loc.x = x;
      bonus.loc.y = y;
      DEBUG_PRINT(std::cout << "Bonus placed at: " << x << "," << y << "\n");
      return;
    }
  }
}

void Game::RemoveBonus() {
  bonus.active = false;
  bonus.loc.x = -1;
  bonus.loc.y = -1;
  bonus.interval = bonus_interval(engine);
  DEBUG_PRINT(std::cout << "Bonus interval: " << bonus.interval << "\n");
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      DEBUG_PRINT(std::cout << "Food placed at: " << x << "," << y << "\n");
      return;
    }
  }
}

void Game::UpdateBonus() {
  Uint32 elapsed_time;
  Uint32 bonus_timer_end = SDL_GetTicks();
  elapsed_time =  bonus_timer_end - bonus_timer_start;
  // Place bonus every `interval` seconds
  if (bonus_timer_end - bonus_timer_start >= bonus.interval * 1000) {
    bonus_timer_start = bonus_timer_end;
    PlaceBonus();
  }
  // Remove bonus in 5 seconds
  if (bonus.active && bonus_timer_end - bonus_timer_start >= 5 * 1000) {
    RemoveBonus();
  }
}

void Game::ConsumeFoodOrBonus(Snake &snake) {
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    DEBUG_PRINT(std::cout << "Snake at: " << new_x << "," << new_y << "\n");
    snake.score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  if (bonus.loc.x == new_x && bonus.loc.y == new_y) {
    snake.score += 2;
    // Grow snake.
    snake.double_growth = true;
    snake.GrowBody();
    snake.speed += 0.02;
    RemoveBonus();
  }
}

int Game::GetScore() const { return snake.score; }
int Game::GetSize() const { return snake.size; }