//
// Created by anthony.spering on 9/22/2025.
//

#ifndef BINARY_H
#define BINARY_H

#include "../MazeGeneratorBase.h"
#include "math/Point2D.h"
#include <map>
#include <vector>

class Binary : public MazeGeneratorBase {
private:
  std::vector<Point2D> stack;
  std::map<int, std::map<int, bool>> visited;  // naive. not optimal
  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);

public:
  Binary() = default;
  std::string GetName() override { return "Binary"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};



#endif //BINARY_H
