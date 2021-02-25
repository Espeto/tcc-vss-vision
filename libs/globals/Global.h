#pragma once

#include "../libobjects/Robo.h"
#include "../libobjects/Ball.h"
#include "../libvision/Colors.h"

namespace Global {
  std::vector<Robo*> getRobots();
  std::vector<Robo*> getAlliedRobots();
  std::vector<Robo*> getEnemyRobots();
  Ball* getBall();
  Colors* getColors();
}

