#include "MapEvolution.h"
#include "MapComparator.h"
#include <iostream>
// for srand
#include <stdlib.h>

// Bigger populations take longer, but allow more diversity in the gene pool.
static const int POPULATION_SIZE = 100;

// STEADY_STATE = true -> means that we keep the parents for the next generation. This is
// useful if you are churning a lot and your crossover methods are unreliable, because it means
// you never throw away good solutions.
static const bool STEADY_STATE = false;

// NOVELTY_INJECTION = true -> means that we add new maps to the population at each new generation.
// This is useful, particularly in systems that don't use mutation (like this one) to keep the gene
// pool fresh and to try and avoid it getting stuck with lots of variations on the same one map.
static const bool NOVELTY_INJECTION = false;

static const int MAX_GENERATIONS = 1000;

static int GENERATIONS_PASSED = 0;

void MapEvolution::initPopulation() {
  for (int i = 0; i < POPULATION_SIZE; i += 1) {
    AMap new_amap;
    population.push_back(new_amap);
  }
}

bool MapEvolution::tickGeneration() {
  return tickGeneration(true);
}

bool MapEvolution::tickGeneration(bool verbose) {
  // sort the population by fitness
  population.sort(compare);

  // This is debug code, really, but watching how fitness changes over time
  // can give you vital info on your evolutionary process. You might find that
  // nothing happens after 500 generations, in which case you're either wasting 
  // time running your program for longer, or there's a problem with your setup.
  if (verbose && GENERATIONS_PASSED % 100 == 0) {
    AMap to_print = population.front();
    std::cout << to_print.length() << "\n";
    to_print.print();
    // to_print.print();
    // printPath(to_print);
  }
  std::list<AMap> nextGeneration;

  while (nextGeneration.size() < POPULATION_SIZE) {
    AMap parent1 = population.front();
    population.pop_front();
    AMap parent2 = population.front();
    population.pop_front();

    if (STEADY_STATE) {
      nextGeneration.push_back(parent1);
      nextGeneration.push_back(parent2);
    }

    // Mix and match crossover styles as you wish. I tend to blend a few together.
    nextGeneration.push_back(parent1.crossoverOnePoint(parent2));
    nextGeneration.push_back(parent2.crossoverOnePoint(parent1));
    nextGeneration.push_back(parent1.crossoverViaPointSwap(parent2));
    nextGeneration.push_back(parent2.crossoverViaPointSwap(parent1));
    nextGeneration.push_back(parent1.mutate());
    nextGeneration.push_back(parent2.mutate());

    if (NOVELTY_INJECTION) {
      // Add in a purely random map, to discourage early convergence.
      AMap a;
      nextGeneration.push_back(a);
      AMap b;
      nextGeneration.push_back(b);
    }
  }
		
  population = nextGeneration;
  GENERATIONS_PASSED += 1;
		
  return GENERATIONS_PASSED >= MAX_GENERATIONS;
}

AMap MapEvolution::evolveMap() {
  initPopulation();
		
  for (int i = 0; i < MAX_GENERATIONS; i += 1) {
    tickGeneration();
  }

  population.sort(compare);
		
  AMap best = *(population.begin());
  std::cout << best.length() << "\n";
  // std::cout << population.get(population.size()-1).length ; // TODO
  best.print();
  // this.printPath(population.get(0)); // TODO
  return best;
}

AMap MapEvolution::getBestMapSoFar() {
  return population.front();
}

bool MapEvolution::evolutionFinished() {
  return MAX_GENERATIONS <= GENERATIONS_PASSED;
}

int main(int argc, char* argv[]) {
  srand(time(0));
  MapEvolution app;
  app.evolveMap();
  return 0;
}

