#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"
#include <cmath>

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f separatingForce = Vector2f::zero();
  Vector2f positiosn = boid->transform.position;

  int countCloseFlockmates = 0;

  for (Boid* neighbor : neighborhood) {
    if (neighbor == boid) continue;

    Vector2f diff = positiosn - neighbor->transform.position;
    float distSq = diff.x * diff.x + diff.y * diff.y;

    if (distSq > 0.0f) {
      float dist = std::sqrt(distSq);

      if (dist < desiredMinimalDistance) {
        Vector2f normDiff = diff / dist;
        separatingForce += normDiff / dist;
        countCloseFlockmates++;
      }
    }
  }

  if (countCloseFlockmates > 0) {
    separatingForce /= (float)countCloseFlockmates;
  }

  float forceLenSq = separatingForce.x * separatingForce.x + separatingForce.y * separatingForce.y;
  if (forceLenSq > 0.0f) {
    float forceLen = std::sqrt(forceLenSq);
    separatingForce /= forceLen;
  }

  return separatingForce;
}


bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
