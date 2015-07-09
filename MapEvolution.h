#ifndef MAP_EVOLUTION_H
#define MAP_EVOLUTION_H

#include <deque>

class AMap;

class MapEvolution {
 public:
  void initPopulation();
  bool tickGeneration();
  bool tickGeneration(bool verbose);
  void evolveMap();
  void evolveMap(bool verbose);
  bool evolutionFinished();

 private:
  std::deque<AMap*> population;

};

#endif
