#include "Agent.h"
#include <unordered_map>
#include <queue>
#include <vector>
#include <limits>
#include "World.h"

#include <algorithm>

using namespace std;

struct Node {
    Point2D pos;
    int g;  // cost so far
    int f;  // total estimated cost (g + h)
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

// heuristic: distance to nearest border
static int heuristic(const Point2D& p, int side) {
    int dx = side - abs(p.x);
    int dy = side - abs(p.y);
    return min(dx, dy);
}

vector<Point2D> Agent::generatePath(World* w) {
    unordered_map<Point2D, Point2D> cameFrom;
    unordered_map<Point2D, int> gScore;

    auto cmp = [](const Node& a, const Node& b) { return a.f > b.f; };
    priority_queue<Node, vector<Node>, decltype(cmp)> frontier(cmp);

    auto catPos = w->getCat();
    int side = w->getWorldSideSize() / 2;

    frontier.push({catPos, 0, heuristic(catPos, side)});
    gScore[catPos] = 0;

    Point2D borderExit = Point2D::INFINITE;

    while (!frontier.empty()) {
        Node current = frontier.top();
        frontier.pop();

        // reached border → goal
        if (abs(current.pos.x) == side || abs(current.pos.y) == side) {
            borderExit = current.pos;
            break;
        }

        for (auto& neigh : w->getVisitableNeighbors(current.pos)) {
            int tentativeG = gScore[current.pos] + 1;

            if (!gScore.count(neigh) || tentativeG < gScore[neigh]) {
                gScore[neigh] = tentativeG;
                int f = tentativeG + heuristic(neigh, side);
                frontier.push({neigh, tentativeG, f});
                cameFrom[neigh] = current.pos;
            }
        }
    }

    // reconstruct path
    vector<Point2D> path;
    if (borderExit != Point2D::INFINITE) {
        Point2D cur = borderExit;
        while (cur != catPos) {
            path.push_back(cur);
            cur = cameFrom[cur];
        }
        ranges::reverse(path.begin(), path.end());
    }

    return path;
}
