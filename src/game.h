#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "bonus.h"
#include "obstacle.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  
 
 private:
  Snake snake;
  std::vector<obstacle> obs;
  SDL_Point food;
  Bonus bonus;
  Uint32 bonus_timer_start{0};

  
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> random_l;
  std::uniform_int_distribution<int> random_m;
  std::uniform_int_distribution<int> bonus_interval; 
  std::vector<obstacle> placeObstacles();
  void clearObstacles(std::vector<obstacle> &obs);
  void RemoveObstacle(std::vector<obstacle> &obs);
  void PlaceFood();
  void PlaceBonus();
  void RemoveBonus();
  void UpdateBonus();
  void Update();
  bool check;
  void ConsumeFoodOrBonus(Snake &snake);
};

#endif