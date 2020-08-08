#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
#include "SDL.h"
#include <random>

class obstacle {
 public:
  //Constructor
  obstacle(){}
  obstacle(int xpos, int ypos): _xpos(xpos), _ypos(ypos),engine(dev()), random_w(0, 3){
        _type=random_w(engine);
        }
  obstacle(obstacle &&source) // 4 : move constructor
    {
        _xpos = source._xpos;
        _ypos = source._ypos;
    	_type = source._type;
        source._xpos = 0;
        source._ypos = 0;
    	source._type = 0;
  }
  ~obstacle() // 1 : destructor
    {
        //std::cout << "DELETING instance of MyMovableClass at " << this << std::endl;
    }
    
    obstacle(const obstacle &source) // 2 : copy constructor
    {
        _xpos = source._xpos;
        _ypos = source._ypos;
    	_type = source._type;
    }
    
    obstacle &operator=(const obstacle &source) // 3 : copy assignment operator
    {
        
        if (this == &source)
            return *this;
        _xpos = source._xpos;
        _ypos = source._ypos;
    	_type = source._type;
        return *this;
    }
        obstacle &operator=(obstacle &&source) // 5 : move assignment operator
    {
        
        if (this == &source)
            return *this;
          _xpos = source._xpos;
        _ypos = source._ypos;
    	_type = source._type;
        return *this;
    }
  
  
  int AsgnPanelity();
  bool CheckPos(int x,int y);
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;

 //private:

  int _xpos;
  int _ypos;
  int _type;
};
#endif