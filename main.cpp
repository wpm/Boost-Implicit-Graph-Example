
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "implicit.hpp"
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
  using namespace boost;
  function_requires< AdjacencyGraphConcept<implicit_ring_graph> >();
  function_requires< ReadablePropertyMapConcept<edge_weight_map, edge> >();
  function_requires<
    ReadablePropertyGraphConcept<
                implicit_ring_graph,
                edge,
                edge_weight_t> >();

  implicit_ring_graph g(5);

  // Print all the vertices and their neighbors.
  for(size_t i = 0; i < 5; i++) {
    std::cout << i << ": ";
    adjacency_iterator ai, ai_end;
    for (tie(ai, ai_end) = adjacent_vertices(i, g); ai != ai_end; ai++) {
      std::cout << *ai << " ";
    }
    std::cout << std::endl;
  }

  // Read an edge weight from the mapping.
  edge_weight_map m = get(boost::edge_weight, g);
  boost::graph_traits<implicit_ring_graph>::edge_descriptor e(0, 1);
  weight w = get(boost::edge_weight, g, e);
  std::cout << w << std::endl;

  return 0;
}