#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
bool RecursiveBacktrackerExample::Step(World* w) {

  // If no cells yet, pick a start point
  if (stack.empty()) {
    Point2D start = randomStartPoint(w);
    if (start.x == INT_MAX || start.y == INT_MAX) {
      // no valid start point left -> finished
      return false;
    }
    visited[start.y][start.x] = true;
    stack.push_back(start);
    w->SetNodeColor(start, Color::White);
    return true;
  }

  Point2D current = stack.back();
  w->SetNodeColor(current, Color::Thistle);

  auto neighbors = getVisitables(w, current);


  if (!neighbors.empty()) {
    // choose random unvisited neighbor
    Point2D next = neighbors[Random::Range(0, (int)neighbors.size() - 1)];
    if (next.y == current.y + 1 && next.x == current.x) {
      //w->SetSouth(current, false);
      w->SetNorth(next, false);
    }else if (next.x == current.x + 1 && next.y == current.y) {
      //w->SetEast(current, false);
      w->SetWest(next, false);
    } else if (next.y == current.y - 1 && next.x == current.x) {
      //
      //w->SetNorth(current, true);
      w->SetSouth(next, false);
    } else if (next.x == current.x - 1 && next.y == current.y) {
      //w->SetWest(current, false);
      w->SetEast(next, false);
    }
    // mark and push
    visited[next.y][next.x] = true;
    w->SetNodeColor(next, Color::Red);
    stack.push_back(next);
  } else {
    // backtrack
    stack.pop_back();
    w->SetNodeColor(current, Color::Black);
  }
  return !stack.empty();
}


void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};

  for (auto d : deltas) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -sideOver2 || n.x > sideOver2 || n.y < -sideOver2 || n.y > sideOver2) {
      continue;
    }
    if (!visited[n.y][n.x]) {
      visitables.push_back(n);
    }
  }
  return visitables;
}

