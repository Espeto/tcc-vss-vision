#include "Global.h"

namespace {
  std::vector<Robo*> robots = {new Robo, new Robo, new Robo, new Robo, new Robo, new Robo};
  std::vector<Robo*> alliedteam(robots.begin(), robots.begin() + 3);
  std::vector<Robo*> enemyteam(robots.begin() + 3, robots.end());
  Ball* const bola = new Ball;
}

std::vector<Robo*> Global::getRobots() {return robots;}
std::vector<Robo*> Global::getAlliedRobots() {return alliedteam;}
std::vector<Robo*> Global::getEnemyRobots() {return enemyteam;}

Ball* Global::getBall() {return bola;}
