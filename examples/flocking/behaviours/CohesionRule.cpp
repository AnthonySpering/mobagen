#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  if (neighborhood.empty()) {
    return Vector2f::zero();
  }

  Vector2f centerOfMass = Vector2f::zero();
  int count = 0;

  // Sum neighbor positions
  for (Boid* neighbor : neighborhood) {
    if (neighbor != boid) { // don’t count itself
      centerOfMass += neighbor->getPosition();
      count++;
    }
  }

  if (count == 0) {
    return Vector2f::zero();
  }

  // Average = center of mass
  centerOfMass /= static_cast<float>(count);

  // Direction towards the COM
  Vector2f cohesionForce = centerOfMass - boid->getPosition();

  // Normalize so it’s just a direction (or leave unnormalized for stronger pull when far away)
  return Vector2f::normalized(cohesionForce);
}