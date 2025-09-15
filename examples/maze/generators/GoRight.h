//
// Created by anthony.spering on 9/15/2025.
//

#ifndef GORIGHT_H
#define GORIGHT_H
#include "math/Point2D.h"

#include <map>
#include <vector>

class World;
class GoRight : public MazeGeneratorBase {
private:
  std::vector<Point2D> stack;
  std::map<int, std::map<int, bool>> visited;  // naive. not optimal
  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);

public:
  GoRight() = default;
  std::string GetName() override { return "GoRight"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};

#endif //GORIGHT_H
