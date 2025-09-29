#include "Cat.h"
#include "World.h"

Point2D Cat::Move(World* world) {
  auto path = generatePath(world);

  if (!path.empty()) {
    return path[0];  // best move toward border
  }

  // Cat is trapped, so move randomly to a free neighboring tile
  auto neighbors = world->getVisitableNeighbors(world->getCat());
  if (!neighbors.empty()) {
    return neighbors[Random::Range(0, neighbors.size() - 1)];
  }

  // No free neighbors, must stay in place
  return world->getCat();
}


