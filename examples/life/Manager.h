#ifndef LIFE_MANAGER_H
#define LIFE_MANAGER_H

#include "scene/GameObject.h"
#include "RuleBase.h"
#include "World.h"

class Manager : GameObject {
private:
  int sideSize = 13;
  World world;
  bool isSimulating = false;
  bool clearing = false; //This is new
  float accumulatedTime = 0;
  float timeBetweenSteps = 0.2f;
  void step();
  void clear();
  void clearBoard(); //This is new
  void save(); //This is new
  void load(); //This is new
  std::vector<RuleBase*> rules;
  int ruleId = 0;
  Point2D mousePositionToIndex(ImVec2& pos);

public:
  explicit Manager(Engine* pEngine);
  ~Manager();

  void Start() override;
  void OnGui(ImGuiContext* context) override;
  void OnDraw(SDL_Renderer* renderer) override;
  void Update(float deltaTime) override;
};

#endif  // MOBAGEN_MANAGER_H
