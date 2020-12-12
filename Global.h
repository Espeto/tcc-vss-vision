#pragma once

#include "Robo.h"
#include "Ball.h"
#include "Colors.h"

namespace Global {
  std::vector<Robo*> getRobots();
  std::vector<Robo*> getAlliedRobots();
  std::vector<Robo*> getEnemyRobots();
  Ball* getBall();
  Colors* getColors();
}

