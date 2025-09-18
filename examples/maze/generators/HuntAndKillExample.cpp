#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>
#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>

bool HuntAndKillExample::Step(World* w) {
  auto sideOver2 = w->GetSize() / 2;
  //set startimg color
  if (stack.empty()) {
    Point2D start = randomStartPoint(w);
    if (start.x == INT_MAX) return false;
    visited[start.y][start.x] = true;
    w->SetNodeColor(start, Color::Black);
    stack.push_back(start);
    return true;
  }

  w->SetNodeColor(stack.back(), Color::Black);

  Point2D cur = stack.back();
  auto neighbors = getVisitables(w, cur);

  if (!neighbors.empty()) {
    Point2D next = neighbors[Random::Range(0, (int)neighbors.size() - 1)];
    if (next.x > cur.x) { w->SetEast(cur, false); w->SetWest(next, false); }
    if (next.x < cur.x) { w->SetWest(cur, false); w->SetEast(next, false); }
    if (next.y > cur.y) { w->SetSouth(cur, false); w->SetNorth(next, false); }
    if (next.y < cur.y) { w->SetNorth(cur, false); w->SetSouth(next, false); }
    visited[next.y][next.x] = true;
    w->SetNodeColor(next, Color::White);
    stack.push_back(next);
    return true;
  }

  stack.clear();
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) {
        auto vns = getVisitedNeighbors(w, {x, y});
        if (!vns.empty()) {
          Point2D next = vns[Random::Range(0, (int)vns.size() - 1)];
          if (next.x > x) { w->SetEast({x, y}, false); w->SetWest(next, false); }
          if (next.x < x) { w->SetWest({x, y}, false); w->SetEast(next, false); }
          if (next.y > y) { w->SetSouth({x, y}, false); w->SetNorth(next, false); }
          if (next.y < y) { w->SetNorth({x, y}, false); w->SetSouth(next, false); }
          visited[y][x] = true;
          w->SetNodeColor({x, y}, Color::White);
          stack.push_back({x, y});
          return true;
        }
      }

  return false;
}

void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}
Point2D HuntAndKillExample::randomStartPoint(World* world) {
  // Todo: improve this if you want
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
  int s = w->GetSize() / 2; std::vector<Point2D> v;
  for (auto d : {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT}) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -s || n.x > s || n.y < -s || n.y > s) continue;
    if (!visited[n.y][n.x]) v.push_back(n);
  }
  return v;
}

std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
  int s = w->GetSize() / 2; std::vector<Point2D> nbs;
  for (auto d : {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT}) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -s || n.x > s || n.y < -s || n.y > s) continue;
    if (visited[n.y][n.x]) nbs.push_back(n);
  }
  return nbs;
}
