#include "PrimExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>
#include <map>

// keep a visited grid like RecursiveBacktracker
static std::map<int, std::map<int, bool>> visited;

bool PrimExample::Step(World* w) {
  int sideOver2 = w->GetSize() / 2;

  // --- initialization ---
  if (!initialized) {
    Point2D start = {Random::Range(-sideOver2, sideOver2),
                     Random::Range(-sideOver2, sideOver2)};
    visited[start.y][start.x] = true;
    w->SetNodeColor(start, Color::Black);

    // add frontier neighbors of start
    auto neighbors = getVisitables(w, start);
    toBeVisited.insert(toBeVisited.end(), neighbors.begin(), neighbors.end());

    initialized = true;
    return true;
  }

  // --- stop if frontier is empty ---
  if (toBeVisited.empty())
    return false;

  // pick a random frontier cell
  int idx = Random::Range(0, (int)toBeVisited.size() - 1);
  Point2D current = toBeVisited[idx];
  toBeVisited.erase(toBeVisited.begin() + idx);

  // if already visited, skip this cell
  if (visited[current.y][current.x])
    return !toBeVisited.empty();

  // get its visited neighbors
  auto neighbors = getVisitedNeighbors(w, current);
  if (neighbors.empty())
    return !toBeVisited.empty();

  // pick one visited neighbor
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

  // mark current as visited
  visited[current.y][current.x] = true;
  w->SetNodeColor(current, Color::Black);

  // add its frontier neighbors (only unvisited ones)
  auto newFrontier = getVisitables(w, current);
  toBeVisited.insert(toBeVisited.end(), newFrontier.begin(), newFrontier.end());

  return !toBeVisited.empty();
}

void PrimExample::Clear(World* world) {
  toBeVisited.clear();
  visited.clear();
  initialized = false;

  auto sideOver2 = world->GetSize() / 2;
  for (int y = -sideOver2; y <= sideOver2; y++) {
    for (int x = -sideOver2; x <= sideOver2; x++) {
      visited[y][x] = false;
    }
  }
}

std::vector<Point2D> PrimExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  std::vector<Point2D> deltas = {
      Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};

  for (auto d : deltas) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -sideOver2 || n.x > sideOver2 ||
        n.y < -sideOver2 || n.y > sideOver2)
      continue;

    if (!visited[n.y][n.x]) {
      // only add if it's not already marked as frontier
      if (w->GetNodeColor(n) == Color::Gray) {
        visitables.push_back(n);
        w->SetNodeColor(n, Color::DarkGray);
      }
    }
  }
  return visitables;
}

std::vector<Point2D> PrimExample::getVisitedNeighbors(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  std::vector<Point2D> deltas = {
      Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};

  for (auto d : deltas) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -sideOver2 || n.x > sideOver2 ||
        n.y < -sideOver2 || n.y > sideOver2)
      continue;

    if (visited[n.y][n.x]) {
      neighbors.push_back(n);
    }
  }
  return neighbors;
}

