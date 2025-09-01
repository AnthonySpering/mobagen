#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to match the heading of neighbors = Average velocity
  Vector2f averageVelocity = Vector2f::zero();

	if (neighborhood.empty()) {
		return Vector2f::zero();
	}

	for (Boid* neighbor : neighborhood) {
		if (neighbor != boid) {
			averageVelocity += neighbor->getVelocity();
		}
	}

	averageVelocity /= static_cast<float>(neighborhood.size());
	return Vector2f::normalized(averageVelocity);
}