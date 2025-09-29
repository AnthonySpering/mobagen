#include "Catcher.h"
#include "World.h"
Point2D Catcher::Move(World* world) {
  auto catPos = world->getCat();
  auto neighbors = world->getVisitableNeighbors(catPos);

  // Count how many blocked neighbors the cat currently has
  int blockedCount = 0;
  auto allDirs = world->getVisitableNeighbors(catPos); // actually all hex directions
  for (auto& n : world->getVisitableNeighbors(catPos)) {
    if (world->getContent(n))
      blockedCount++;
  }

  // Only trigger the "tighten trap" heuristic if cat has 4+ walls around it
  if (blockedCount >= 4 && !neighbors.empty()) {
    Point2D bestMove = neighbors[0];
    int minFreedom = 7;  // max hex neighbors = 6

    // Evaluate each free neighbor
    for (auto& n : neighbors) {
      // Count free neighbors around this candidate
      int freedom = world->getVisitableNeighbors(n).size();
      if (freedom < minFreedom && world->catcherCanMoveToPosition(n)) {
        minFreedom = freedom;
        bestMove = n;
      }
    }

    return bestMove;  // block the least “free” tile around cat
  }

  // Normal AI logic if cat isn’t nearly trapped
  auto path = generatePath(world);
  if (!path.empty()) {
    return path[0]; // block first optimal path tile
  }

  // fallback: random free tile
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p))
      return p;
  }
}

/*Point2D Catcher::Move(World* world) {
    auto path = generatePath(world);
    auto catPos = world->getCat();
    auto side = world->getWorldSideSize() / 2;

    // 1. Cat has a path → try to set up a box
    if (!path.empty()) {
        // Pick a future step to trap (e.g., 2-3 moves ahead)
        int trapIndex = std::min(4, (int)path.size() - 1);
        Point2D trapCenter = path[trapIndex];

        Point2D bestMove = trapCenter;
        int bestScore = -1;

        // Check neighbors around trapCenter for optimal wall placement
        for (auto& n : world->getVisitableNeighbors(trapCenter)) {
            if (!world->catcherCanMoveToPosition(n) || world->getContent(n))
                continue;

            // Heuristic: prioritize tiles that complete a partial box
            int neighborWalls = 0;
            for (auto& nn : world->getVisitableNeighbors(n))
                if (world->getContent(nn)) neighborWalls++;

            // Keep one open escape: ignore the tile that leads to the cat
            if (n == path[trapIndex - 1])
                continue;

            // Score based on closeness to trapCenter and walls around
            int score = neighborWalls * 5 - std::max(abs(n.x - trapCenter.x), abs(n.y - trapCenter.y));
            if (score > bestScore) {
                bestScore = score;
                bestMove = n;
            }
        }

        return bestMove;
    }

    // 2. Cat is trapped → close remaining gaps
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

    // 3. fallback: any free tile
    for (;;) {
        Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
        if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p))
            return p;
    }
}*/


/*Point2D Catcher::Move(World* world) {
    auto path = generatePath(world);

    if (!path.empty()) {
        // Cat can escape: block first optimal tile along path
        Point2D bestMove = path[0];
        int bestScore = -1;

        for (int i = 0; i < path.size(); i++) {
            Point2D candidate = path[i];
            if (!world->catcherCanMoveToPosition(candidate) || world->getContent(candidate))
                continue;

            int neighborWalls = 0;
            for (auto& n : world->getVisitableNeighbors(candidate)) {
                if (world->getContent(n)) neighborWalls++;
            }

            int distFromCat = i;
            int distToBorder = (world->getWorldSideSize() / 2) - std::max(abs(candidate.x), abs(candidate.y));
            int score = distFromCat * 2 + distToBorder + neighborWalls * 5;

            if (score > bestScore) {
                bestScore = score;
                bestMove = candidate;
            }
        }

        return bestMove;
    }

    // Cat is trapped → place block on free neighbor around cat
    Point2D catPos = world->getCat();
    auto neighbors = world->getVisitableNeighbors(catPos);

    if (!neighbors.empty()) {
        // Pick the neighbor with the most existing walls around it
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

    // fallback: pick any free tile (should rarely happen)
    auto side = world->getWorldSideSize() / 2;
    for (;;) {
        Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
        if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p))
            return p;
    }
}*/


/*Point2D Catcher::Move(World* world) {
  auto path = generatePath(world);

  if (!path.empty()) {
    Point2D bestMove = path[0];
    int bestScore = -1;

    for (int i = 0; i < path.size(); i++) {
      Point2D candidate = path[i];

      if (!world->catcherCanMoveToPosition(candidate) || world->getContent(candidate)) {
        continue;
      }

      int distFromCat = i;
      int distToBorder = (world->getWorldSideSize() / 2) - std::max(abs(candidate.x), abs(candidate.y));

      int score = distFromCat * 2 + distToBorder;

      if (score > bestScore) {
        bestScore = score;
        bestMove = candidate;
      }
    }

    return bestMove;
  }

  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();
    if ((p.x != cat.x || p.y != cat.y) && !world->getContent(p)) {
      return p;
    }
  }
}*/


/*Point2D Catcher::Move(World* world) {
  auto path = generatePath(world);

  if (!path.empty()) {
    Point2D blockHere = path[0];

    if (world->catcherCanMoveToPosition(blockHere) && !world->getContent(blockHere)) {
      return blockHere;
    }
  }

  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {
      Random::Range(-side, side), Random::Range(-side, side)
    };
    auto cat = world->getCat();
    if ((p.x != cat.x || p.y != cat.y) && !world->getContent(p)) {
      return p;
    }
  }
}*/




/*Point2D Catcher::Move(World* world) {
    auto catPos = world->getCat();
    auto side = world->getWorldSideSize() / 2;

    Cat tempCat;
    Point2D predictedMove = tempCat.Move(world);

    int wallCount = 0;
    std::vector<Point2D> freeSpaces;
    Point2D neighbors[] = {world->NE(catPos), world->NW(catPos), world->E(catPos), world->W(catPos),
                           world->SE(catPos), world->SW(catPos)};

    for (auto& n : neighbors) {
        if (!world->isValidPosition(n)) continue;
        if (world->getContent(n)) wallCount++;
        else freeSpaces.push_back(n);
    }

    if (wallCount >= 4 && freeSpaces.size() > 1) {
        bool disconnected = true;
        for (size_t i = 0; i < freeSpaces.size(); i++) {
            for (size_t j = i + 1; j < freeSpaces.size(); j++) {
                if (world->isNeighbor(freeSpaces[i], freeSpaces[j])) {
                    disconnected = false;
                    goto DoneChecking;
                }
            }
        }
    DoneChecking:

        if (disconnected) {
            return freeSpaces[0];
        }
    }


    if (wallCount <= 2) {
        if (predictedMove.x != catPos.x || predictedMove.y != catPos.y) {
            if (world->catcherCanMoveToPosition(predictedMove) && !world->getContent(predictedMove)) {
                return predictedMove;
            }
        }
    } else {
        if (world->catcherCanMoveToPosition(predictedMove) && !world->getContent(predictedMove)) {
            return predictedMove;
        }
    }

    auto catPath = generatePath(world);
    for (auto& tile : catPath) {
        if (world->catcherCanMoveToPosition(tile) && !world->getContent(tile))
            return tile;
    }

    for (;;) {
        Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
        if ((p.x != catPos.x || p.y != catPos.y) && !world->getContent(p))
            return p;
    }
}*/
