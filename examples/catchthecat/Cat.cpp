#include "Cat.h"
#include "World.h"
#include <vector>
#include <algorithm>

Point2D Cat::Move(World* world) {
  // Use the same A* routine the Agent has, but goal is the border
  auto path = generatePath(world);

  // If a path exists, move to the first step (path[0])
  if (!path.empty()) {
    return path.front();
  }

  // fallback: random move if trapped
  auto pos = world->getCat();
  auto neighs = world->getVisitableNeighbors(pos);
  if (!neighs.empty()) {
    return neighs[Random::Range(0, (int)neighs.size() - 1)];
  }

  return pos; // completely stuck
}
