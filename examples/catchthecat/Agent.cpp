#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"

#include <algorithm>
using namespace std;
std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // for path reconstruction
  queue<Point2D> frontier;
  unordered_set<Point2D> frontierSet;
  unordered_map<Point2D, bool> visited;      // stick with map<bool>

  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);

  Point2D borderExit = Point2D::INFINITE;

  while (!frontier.empty()) {
    Point2D current = frontier.front();
    frontier.pop();
    frontierSet.erase(current);
    visited[current] = true;

    // if this tile is on the border, we found an exit
    if (w->isNeighbor(current, borderExit)) {
      borderExit = current;
      break;
    }

    // get valid neighbors (walls/cat position filtered already)
    for (auto& next : w->getVisitableNeighbors(current)) {
      if (visited.count(next)) continue; // already processed
      if (frontierSet.find(next) != frontierSet.end()) continue; // already queued

      cameFrom[next] = current;
      frontier.push(next);
      frontierSet.insert(next);
    }
  }

  std::vector<Point2D> path;

  // reconstruct path if exit found
  if (borderExit != Point2D::INFINITE) {
    Point2D current = borderExit;
    while (current != catPos) {
      path.push_back(current);
      current = cameFrom[current];
    }
    path.push_back(catPos);

    // reverse so [0] is the cat’s move toward escape
    std::reverse(path.begin(), path.end());
  }

  return path; // empty if no border found
}
