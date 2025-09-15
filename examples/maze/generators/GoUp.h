//
// Created by anthony.spering on 9/15/2025.
//

#ifndef GOUP_H
#define GOUP_H
#include "math/Point2D.h"

#include <map>
#include <vector>

class World;
class GoUp : public MazeGeneratorBase {
private:
  std::vector<Point2D> stack;
  std::map<int, std::map<int, bool>> visited;  // naive. not optimal
  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);

public:
  GoUp() = default;
  std::string GetName() override { return "GoUp"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};

#endif //GOUP_H
