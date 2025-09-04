#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  if (neighborhood.empty()) {
    return Vector2f::zero();
  }

  Vector2f centerOfMass = Vector2f::zero();
  int count = 0;

  for (Boid* neighbor : neighborhood) {
    if (neighbor != boid) {
      centerOfMass += neighbor->getPosition();
      count++;
    }
  }

  if (count == 0) {
    return Vector2f::zero();
  }

  centerOfMass /= static_cast<float>(count);

  Vector2f cohesionForce = centerOfMass - boid->getPosition();

  return Vector2f::normalized(cohesionForce);
}