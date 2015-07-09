#ifndef MAP_EVOLUTION_H
#define MAP_EVOLUTION_H

#include <list>

#include "AMap.h"

class MapEvolution {
 public:
  void initPopulation();
  bool tickGeneration();
  bool tickGeneration(bool verbose);
  AMap evolveMap();
  AMap evolveMap(bool verbose);
  AMap getBestMapSoFar();
  bool evolutionFinished();

 private:
  std::list<AMap> population;

  void printPath(AMap);
  void printMap(AMap::map_rep);
};

#endif
