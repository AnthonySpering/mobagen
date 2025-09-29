#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
#include <algorithm>
#include "math/Point2D.h"


struct Node {
  Point2D pos;
  int g;
  int f;
  bool operator>(const Node& other) const { return f > other.f; }
};

std::vector<Point2D> Agent::generatePath(World* w) {
  auto catPos = w->getCat();
  auto side = w->getWorldSideSize();
  Point2D borderExit = Point2D::INFINITE;

  auto heuristicToBorder = [&](const Point2D& p) {
    int half = side / 2;
    return std::min({ half - abs(p.x), half - abs(p.y) });
  };

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;
  frontier.push({catPos, 0, heuristicToBorder(catPos)});

  std::unordered_map<Point2D, Point2D> cameFrom;
  std::unordered_map<Point2D, int> gScore;
  gScore[catPos] = 0;

  std::unordered_set<Point2D> visited;

  while (!frontier.empty()) {
    Node current = frontier.top();
    frontier.pop();

    if (visited.count(current.pos)) continue;
    visited.insert(current.pos);

  if (w->catWinsOnSpace(current.pos)) {
      borderExit = current.pos;
      break;
    }

    for (auto& neighbor : w->getVisitableNeighbors(current.pos)) {
      int tentative_g = gScore[current.pos] + 1;

      if (!gScore.count(neighbor) || tentative_g < gScore[neighbor]) {
        cameFrom[neighbor] = current.pos;
        gScore[neighbor] = tentative_g;
        int f = tentative_g + heuristicToBorder(neighbor);
        frontier.push({neighbor, tentative_g, f});
      }
    }
  }

  std::vector<Point2D> path;
  if (borderExit != Point2D::INFINITE) {
    for (Point2D cur = borderExit; cur != catPos; cur = cameFrom[cur]) {
      path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
  }
  return path;
}
