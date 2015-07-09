#ifndef A_MAP_H
#define A_MAP_H

#include <vector>

class AMap {
 public:
  typedef std::vector<bool> row_rep;
  typedef std::vector<row_rep> map_rep;

  // You often want a few ways of constructing solutions for an evolutionary program.
  // I use a default one (which generates a random map) and a parameterised one
  // which is used during crossover, to directly inherit from a parent.

  // Constructor
  AMap();

  // Constructor
  AMap(map_rep master);
  
  //CROSSOVER METHODS OF VARIOUS KINDS
  
  // Point Swap creates a child based on the map contained in <code>this</code>.
  // It then uses a fixed probability, P, to decide whether a tile in the child should 
  // instead be inherited from <code>otherParent</code>.
  AMap* crossoverViaPointSwap(AMap* otherParent);
  
  // One-Point crossover chooses an arbitrary point in the map. For all
  // elements occuring before this point (reading left-right, top-down),
  // take the map entry from <code>this</code> and add to child. For all
  // elements after this point, take the map entry from <code>otherParent</code>. 
  AMap* crossoverOnePoint(AMap* otherParent);
  
  // Row-column crossover, something I've used myself in the past but not recommended
  // in general. 0.3 chance of taking a column or row from <code>otherParent</code>
  // rather than <code>this</code>.
  AMap* crossoverViaRowColumnSwap(AMap* otherParent);
  
  // New for 2013! Mutation!
  // Evolution isn't perfect. One big problem it has is that crossover only
  // gets you so far - it can't make small adjustments very well. Mutation
  // helps with this - it makes smaller changes to a single solution to try and
  // slowly improve them. Here, we just randomly flip ten tiles in the map. 
  AMap* mutate();
  
  // Accessor
  int length();

  // Debug printer
  void print();

 private:
  map_rep map;
  bool length_computed;
  int length_cache;

};

#endif
