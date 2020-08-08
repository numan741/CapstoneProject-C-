#include "obstacle.h"
#include <cmath>
#include <iostream>


int obstacle::AsgnPanelity(){
  int harm=0;
	if(type==Type::Box){
    	harm=1;
    }
   else if(type==Type::building){
     	harm=2;
    }else if(type==Type::mountain){
     	harm=3;
   }
}

bool obstacle::CheckPos(int x,int y){
  bool ch=false;
  if (x == static_cast<int>(_xpos) && y == static_cast<int>(_ypos)) {
    ch= true;
  }
  return ch;
}

