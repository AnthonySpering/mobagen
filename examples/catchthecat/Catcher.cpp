#include "Catcher.h"
#include "World.h"

#include "Agent.h"
#include <algorithm>
#include <iostream>

Point2D Catcher::Move(World* world) {
    auto path = generatePath(world); // cat’s escape path
    auto catPos = world->getCat();
    auto side = world->getWorldSideSize() / 2;

    // 1. Cat has a path → cut it off
    if (!path.empty()) {
        int trapIndex = std::min(3, (int)path.size() - 1);
        Point2D trapCenter = path[trapIndex];

        Point2D bestMove = trapCenter;
        int bestScore = -1;

        for (auto& n : world->getVisitableNeighbors(trapCenter)) {
            if (!world->catcherCanMoveToPosition(n) || world->getContent(n))
                continue;

            int neighborWalls = 0;
            for (auto& nn : world->getVisitableNeighbors(n))
                if (world->getContent(nn)) neighborWalls++;

            // Score encourages building walls, but also blocking cat’s path
            int distToCat = abs(n.x - catPos.x) + abs(n.y - catPos.y);
            int score = neighborWalls * 5 - distToCat;

            if (score > bestScore) {
                bestScore = score;
                bestMove = n;
            }
        }

        return bestMove;
    }

    // 2. Cat is trapped → close gaps
    auto neighbors = world->getVisitableNeighbors(catPos);
    if (!neighbors.empty()) {
        Point2D bestMove = neighbors[0];
        int bestWalls = -1;

        for (auto& n : neighbors) {
            int neighborWalls = 0;
            for (auto& nn : world->getVisitableNeighbors(n))
                if (world->getContent(nn)) neighborWalls++;

            if (neighborWalls > bestWalls) {
                bestWalls = neighborWalls;
                bestMove = n;
            }
        }

        return bestMove;
    }

    // 3. fallback: random valid tile
    for (;;) {
        Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
        if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p))
            return p;
    }
}

// Point2D Catcher::Move(World* world) {
//   auto path = generatePath(world);
//   auto catPos = world->getCat();
//
//   if (!path.empty()) {
//     Point2D bestMove = path[0];
//     int bestScore = -1;
//
//     for (int i = 0; i < path.size(); i++) {
//       Point2D candidate = path[i];
//
//       if (!world->catcherCanMoveToPosition(candidate) || world->getContent(candidate)) {
//         continue;
//       }
//
//       int distFromCat = i;
//       int distToBorder = (world->getWorldSideSize() / 2) - std::max(abs(candidate.x), abs(candidate.y));
//
//       int score = distFromCat * 2 + distToBorder;
//
//       if (score > bestScore) {
//         bestScore = score;
//         bestMove = candidate;
//       }
//     }
//
//     return bestMove;
//   }
//
//   auto neighbors = world->getVisitableNeighbors(catPos);
//   if (!neighbors.empty()) {
//       Point2D bestMove = neighbors[0];
//       int bestWalls = -1;
//
//       for (auto& n : neighbors) {
//           int neighborWalls = 0;
//           for (auto& nn : world->getVisitableNeighbors(n))
//               if (world->getContent(nn)) neighborWalls++;
//
//           if (neighborWalls > bestWalls) {
//               bestWalls = neighborWalls;
//               bestMove = n;
//           }
//       }
//
//       return bestMove;
//   }
// }

// Point2D Catcher::Move(World* world) {
//     auto path = generatePath(world);
//     auto catPos = world->getCat();
//     auto side = world->getWorldSideSize() / 2;
//
//     // 1. Cat has a path → try to set up a box
//     if (!path.empty()) {
//         // Pick a future step to trap (e.g., 2-3 moves ahead)
//         int trapIndex = std::min(3, (int)path.size() - 1);
//         Point2D trapCenter = path[trapIndex];
//
//         Point2D bestMove = trapCenter;
//         int bestScore = -1;
//
//         // Check neighbors around trapCenter for optimal wall placement
//         for (auto& n : world->getVisitableNeighbors(trapCenter)) {
//             if (!world->catcherCanMoveToPosition(n) || world->getContent(n))
//                 continue;
//
//             // Heuristic: prioritize tiles that complete a partial box
//             int neighborWalls = 0;
//             for (auto& nn : world->getVisitableNeighbors(n))
//                 if (world->getContent(nn)) neighborWalls++;
//
//             // Keep one open escape: ignore the tile that leads to the cat
//             if (n == path[trapIndex - 1])
//                 continue;
//
//             // Score based on closeness to trapCenter and walls around
//             int score = neighborWalls * 5 - std::max(abs(n.x - trapCenter.x), abs(n.y - trapCenter.y));
//             if (score > bestScore) {
//                 bestScore = score;
//                 bestMove = n;
//             }
//         }
//
//         return bestMove;
//     }
//
//     // 2. Cat is trapped → close remaining gaps
//     auto neighbors = world->getVisitableNeighbors(catPos);
//     if (!neighbors.empty()) {
//         Point2D bestMove = neighbors[0];
//         int bestWalls = -1;
//
//         for (auto& n : neighbors) {
//             int neighborWalls = 0;
//             for (auto& nn : world->getVisitableNeighbors(n))
//                 if (world->getContent(nn)) neighborWalls++;
//
//             if (neighborWalls > bestWalls) {
//                 bestWalls = neighborWalls;
//                 bestMove = n;
//             }
//         }
//
//         return bestMove;
//     }
//
//     // 3. fallback: any free tile
//     for (;;) {
//         Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
//         if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p)) {
//           std::cout << "Hello World" << std::endl;
//           return p;
//         }
//     }
//}