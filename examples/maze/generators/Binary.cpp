#include "../World.h"
#include "Random.h"
#include "Binary.h"
#include <climits>
bool Binary::Step(World* w) {
  int sideOver2 = w->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++) {
    for (int x = -sideOver2; x <= sideOver2; x++) {
      Point2D current = {x, y};

      // Mark visited
      visited[y][x] = true;
      w->SetNodeColor(current, Color::Black);

      // Determine neighbors to carve into
      std::vector<Point2D> options;

      if (y > -sideOver2) options.push_back({x, y - 1}); // north
      if (x > -sideOver2) options.push_back({x - 1, y}); // west

      if (!options.empty()) {
        Point2D carve = options[Random::Range(0, (int)options.size() - 1)];
        // carve wall between current and chosen neighbor
        if (carve.x == x && carve.y == y - 1) { // north
          w->SetNorth(current, false);
          w->SetSouth(carve, false);
        } else if (carve.x == x - 1 && carve.y == y) { // west
          w->SetWest(current, false);
          w->SetEast(carve, false);
        }
      }
    }
  }
  return !stack.empty();
}


void Binary::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}