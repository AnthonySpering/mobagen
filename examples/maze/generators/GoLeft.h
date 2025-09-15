//
// Created by anthony.spering on 9/15/2025.
//

#ifndef GOLEFT_H
#define GOLEFT_H
#include "math/Point2D.h"

#include <map>
#include <vector>

class World;
class GoLeft: public MazeGeneratorBase {
private:
  std::vector<Point2D> stack;
  std::map<int, std::map<int, bool>> visited;  // naive. not optimal
  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);

public:
  GoLeft() = default;
  std::string GetName() override { return "GoLeft"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};

#endif //GOLEFT_H
