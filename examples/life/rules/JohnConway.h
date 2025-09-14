#ifndef MOBAGEN_JOHNCONWAY_H
#define MOBAGEN_JOHNCONWAY_H
#include "../RuleBase.h"
#include "math/Vector2.h"

class JohnConway : public RuleBase {
public:
  explicit JohnConway() = default;
  ~JohnConway() override = default;
  std::string GetName() override { return "JohnConway"; }
  void Step(World& world) override;
  int CountNeighbors(World& world, Point2D point);
  GameOfLifeTileSetEnum GetTileSet() override { return GameOfLifeTileSetEnum::Square; };
  //new stuff
  void clear(World& world) override;
  void save(World& world, const std::string& filename);
  void load(World& world, const std::string& filename);

};

#endif  // MOBAGEN_JOHNCONWAY_H
