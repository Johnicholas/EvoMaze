#include "AMap.h"

// for drand48
#include <stdlib.h>

// for std::cout
#include <iostream>

// Feel free to play with these.
static const int map_width = 12;
static const int map_height = 12;
static const double initial_density = 0.75;
static const double crossover_rate = 0.1;
static const int flip_amount = 10;
// These can also be fun to play with. Try moving the end co-ordinates into 
// the middle of the map (or even next to the start!)
static const int start_row = 0;
static const int start_col = 0;
static const int end_row = map_height - 1;
static const int end_col = map_width - 1;
  
// Calculates the length from start_x/y to end_x/y.
int AMap::length() {
  if (length_computed) {
    return length_cache;
  }

  int d[map_height][map_width];

  // initialize
  for (int i = 0; i < map_height; i += 1) {
    for (int j = 0; j < map_width; j += 1) {
      d[i][j] = 999; // TODO: infinity
    }
  }

  d[start_row][start_col] = 0;

  // until quiescence
  bool done = false;
  while (not done) {
    done = true;
    for (int i = 0; i < map_height; i += 1) {
      for (int j = 0; j < map_width; j += 1) {
        if (map[i][j]) {
          // north
          if (i-1 >= 0 && d[i][j] > d[i-1][j]+1) {
            d[i][j] = d[i-1][j]+1;
            done = false;
          }
          // east
          if (j-1 >= 0 && d[i][j] > d[i][j-1]+1) {
            d[i][j] = d[i][j-1]+1;
            done = false;
          }
          // west
          if (j+1 < map_width && d[i][j] > d[i][j+1]+1) {
            d[i][j] = d[i][j+1]+1;
            done = false;
          }
          // south
          if (i+1 < map_height && d[i][j] > d[i+1][j]+1) {
            d[i][j] = d[i+1][j]+1;
            done = false;
          }
        }
      }
    }
  }

  length_computed = true;
  length_cache = d[end_row][end_col] == 999 ? 0 : d[end_row][end_col];
  return length_cache;
}

// A note on random map generation:
// Initially, I thought that we'd have a 50/50 odds of making a tile blocked or free. However,
// like many evolutionary algorithms, the first guess is not the best one. Instead, we make random
// maps more empty than full, by weighting against adding in blocked tiles. The reason we do this is
// to avoid having lots of random maps which are completely impassable, with no route at all to the end.
static bool newRandomMapTile() {
  return drand48() < initial_density;
}

// Just a loop helper that appeals to newRandomMapTile()
static AMap::map_rep newRandomMap() {
  AMap::map_rep map;
  for (int i = 0; i < map_height; i += 1) {
    AMap::row_rep row;
    for (int j = 0; j < map_width; j += 1) {
      if (i == start_row && j == start_col) {
        row.push_back(true);
      } else if (i == end_row && j == end_col) {
        row.push_back(true);
      } else {
        row.push_back(newRandomMapTile());
      }
    }
    map.push_back(row);
  }
  return map;
}
	
// Constructor
AMap::AMap() : map(newRandomMap()), length_computed(false) {
}

// Constructor
AMap::AMap(map_rep master) : map(master), length_computed(false) {
}

AMap AMap::crossoverViaPointSwap(AMap otherParent) {
  map_rep out;
  for (int i = 0; i < map_height; i += 1) {
    row_rep row;
    for (int j = 0; j < map_width; j += 1) {
      if (drand48() < crossover_rate) {
        row.push_back(map[i][j]);
      } else {
        row.push_back(otherParent.map[i][j]);
      }
    }
    out.push_back(row);
  }
  return AMap(out);
}
	
AMap AMap::crossoverOnePoint(AMap otherParent) {
  map_rep out;
  int swap = rand() % (map_height * map_width);
  for (int i = 0; i < map_height; i += 1) {
    row_rep row;
    for (int j = 0; j < map_width; j += 1) {
      if (swap > 0) {
        swap -= 1;
        row.push_back(map[i][j]);
      } else {
        row.push_back(otherParent.map[i][j]);
      }
    }
    out.push_back(row);
  }
  return AMap(out);
}

// New for 2013! Mutation!
// Evolution isn't perfect. One big problem it has is that crossover only
// gets you so far - it can't make small adjustments very well. Mutation
// helps with this - it makes smaller changes to a single solution to try and
// slowly improve them. Here, we just randomly flip ten tiles in the map. 
AMap AMap::mutate() {
  map_rep out = map;
  for (int n = 0; n < flip_amount; n += 1) {
    int i = rand() % map_height;
    int j = rand() % map_width;
    out[i][j] = !out[i][j];
  }
  return AMap(out);
}
	
void AMap::print() {
  std::cout << "---\n";
  for (int i = 0; i < map_height; i += 1) {
    for (int j = 0; j < map_width; j += 1) {
      std::cout << (map[i][j] ? "." : "#");
    }
    std::cout << "\n";
  }
  std::cout << "---\n";
}


