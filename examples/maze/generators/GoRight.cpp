#include "../World.h"
#include "Random.h"
#include "GoRight.h"
#include <climits>
bool GoRight::Step(World* w) {
    auto sideOver2 = w->GetSize() / 2;

    // pick a random start if stack is empty
    if (stack.empty()) {
        Point2D start = randomStartPoint(w);
        if (start.x == INT_MAX) return false; // no unvisited cells left
        visited[start.y][start.x] = true;
        stack.push_back(start);
        w->SetNodeColor(start, Color::Red);
        return true;
    }

    // reset previous current cell to white
    Point2D previous = stack.back();
    w->SetNodeColor(previous, Color::White);

    Point2D current = stack.back();
    // mark current cell red
    w->SetNodeColor(current, Color::Black);

    auto neighbors = getVisitables(w, current);

    if (!neighbors.empty()) {
        // prefer going south
        Point2D next;
        bool foundDown = false;
        Point2D down{current.x +1, current.y};
        for (auto &n : neighbors) {
            if (n.x == down.x && n.y == down.y) {
                next = n;
                foundDown = true;
                break;
            }
        }
        if (!foundDown) {
            // fallback: random neighbor
            next = neighbors[Random::Range(0, (int)neighbors.size() - 1)];
        }

        // break walls between current and next
        if (next.x == current.x + 1 && next.y == current.y) {
            w->SetEast(current, false);
            w->SetWest(next, false);
        } else if (next.x == current.x - 1 && next.y == current.y) {
            w->SetWest(current, false);
            w->SetEast(next, false);
        } else if (next.y == current.y + 1 && next.x == current.x) {
            w->SetSouth(current, false);
            w->SetNorth(next, false);
        } else if (next.y == current.y - 1 && next.x == current.x) {
            w->SetNorth(current, false);
            w->SetSouth(next, false);
        }

        // mark as visited and push to stack
        visited[next.y][next.x] = true;
        w->SetNodeColor(next, Color::White); // already visited, keep white
        stack.push_back(next);
    } else {
        // backtrack if no neighbors
        stack.pop_back();
    }

    return !stack.empty();
}



void GoRight::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D GoRight::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> GoRight::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};


  for (auto d : deltas) {
    Point2D n = {p.x + d.x, p.y + d.y};
    if (n.x < -sideOver2 || n.x > sideOver2 || n.y < -sideOver2 || n.y > sideOver2) continue;
    if (!visited[n.y][n.x]) {
      visitables.push_back(n);
    }
  }
  return visitables;
}

