//
// Created by atolstenko on 2/9/2023.
//

#include "HexagonGameOfLife.h"

#include <fstream>
#include <stdexcept>
void HexagonGameOfLife::Step(World& world) {
  int size = world.SideSize();

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      Point2D point(x, y);
      bool alive = world.Get(point);
      int neighbors = CountNeighbors(world, point);

      if (alive) {
        // Default: Conway-like rules for hex grid
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

int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) {
  static const int evenOffsets[6][2] = {
    {-1, -1}, {0, -1},
    {-1,  0}, {1,  0},
    {-1,  1}, {0,  1}
  };

  static const int oddOffsets[6][2] = {
    {0, -1}, {1, -1},
    {-1, 0}, {1, 0},
    {0, 1}, {1, 1}
  };

  int count = 0;
  bool isOddRow = (point.y % 2 != 0);

  for (int i = 0; i < 6; i++) {
    int nx = point.x + (isOddRow ? oddOffsets[i][0] : evenOffsets[i][0]);
    int ny = point.y + (isOddRow ? oddOffsets[i][1] : evenOffsets[i][1]);

    if (nx >= 0 && nx < world.SideSize() && ny >= 0 && ny < world.SideSize()) {
      if (world.Get({nx, ny})) {
        count++;
      }
    }
  }

  return count;
}

void HexagonGameOfLife::clear(World& world) {
  for (int y = 0; y < world.SideSize(); y++) {
    for (int x = 0; x < world.SideSize(); x++) {
      world.SetCurrent({x, y}, false);
    }
  }
}

void HexagonGameOfLife::save(World& world, const std::string& filename) {
  std::ofstream out(filename + ".txt");
  if (!out.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + filename);
  }

  for (int y = 0; y < world.SideSize(); y++) {
    for (int x = 0; x < world.SideSize(); x++) {
      out << (world.Get({x, y}) ? '1' : '0');
    }
    out << "\n";
  }
}

void HexagonGameOfLife::load(World& world, const std::string& filename) {
  std::ifstream in(filename + ".txt");
  if (!in.is_open()) {
    throw std::runtime_error("Failed to open file for reading: " + filename);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }

  int size = static_cast<int>(lines.size());
  world.Resize(size);

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      bool alive = (lines[y][x] == '1');
      world.SetCurrent({x, y}, alive);
    }
  }
}