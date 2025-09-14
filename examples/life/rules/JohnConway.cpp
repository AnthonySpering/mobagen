#include "JohnConway.h"

#include "math/Vector2.h"

#include <fstream>

void JohnConway::Step(World& world) {
  int size = world.SideSize();

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      Point2D point(x, y);
      bool alive = world.Get(point);
      int neighbors = CountNeighbors(world, point);

      if (alive) {
        if (neighbors < 2 || neighbors > 3) {
          world.SetNext(point, false);
        } else {
          world.SetNext(point, true);
        }
      } else {
        if (neighbors == 3) {
          world.SetNext(point, true);
        } else {
          world.SetNext(point, false);
        }
      }
    }
  }
  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point)
{
  int count = 0;
  for (int dx = -1; dx <= 1; dx++)
  {
    for (int dy = -1; dy <= 1; dy++)
    {
      if (dx == 0 && dy == 0) continue; // skip self
        if (world.Get({point.x + dx, point.y + dy}))
        {
          count++;
        }
    }
  }
  return count;
}

//new stuff
void JohnConway::clear(World& world) {
  for (int y = 0; y < world.SideSize(); y++) {
    for (int x = 0; x < world.SideSize(); x++) {
      Point2D point(x, y);
      bool alive = world.Get(point);
      if (alive) {}
      world.SetCurrent(point, false);
    }
  }
}

void JohnConway::save(World& world, const std::string& filename) {
  std::ofstream out((filename + ".txt").c_str());
  for (int y = 0; y < world.SideSize(); y++) {
    for (int x = 0; x < world.SideSize(); x++) {
      out << world.Get({x, y}) ? '1' : '0';
    }
    out << "\n";
  }
}

void JohnConway::load(World& world, const std::string& filename) {
  std::ifstream in((filename + ".txt").c_str());

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(in, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }

  int size = static_cast<int>(lines.size());
  world.Resize(size);
  for (int y = 0; y < world.SideSize(); y++) {
    for (int x = 0; x < world.SideSize(); x++) {
      bool alive = (lines[y][x] == '1');
      if (alive) {
        world.SetCurrent({x, y}, true);
      }
    }
  }
}

