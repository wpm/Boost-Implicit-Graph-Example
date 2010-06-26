
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "implicit.hpp"
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
  using namespace implicit_ring;

  // Check the concepts that implicit_ring_graph models.
  boost::function_requires<
    boost::IncidenceGraphConcept<implicit_ring_graph> >();

  boost::function_requires<
    boost::ReadablePropertyMapConcept<const_edge_weight_map,
                                      edge_descriptor> >();
  boost::function_requires<
    boost::ReadablePropertyGraphConcept<implicit_ring_graph,
                                        edge_descriptor,
                                        boost::edge_weight_t> >();

  // Create a small ring graph.
  implicit_ring_graph g(5);
  const_edge_weight_map m = get(boost::edge_weight, g);

  // Print the outgoing edges of all vertices along with their weights.
  //  
  // Vertex 0: <0, 4>(1)  <0, 1>(1)
  // Vertex 1: <1, 0>(1)  <1, 2>(1)
  // Vertex 2: <2, 1>(1)  <2, 3>(1)
  // Vertex 3: <3, 2>(1)  <3, 4>(1)
  // Vertex 4: <4, 3>(1)  <4, 0>(1)
  for(size_t u = 0; u < 5; u++) {
    std::cout << "Vertex " << u << ": ";
    out_edge_iterator oi, oi_end;
    for (tie(oi, oi_end) = out_edges(u, g); oi != oi_end; oi++) {
      edge_descriptor e = *oi;
      std::cout << "<" << e.first << ", " << e.second << ">";
      std::cout << "(" << get(boost::edge_weight, g, e) << ")" << "  ";
    }
    std::cout << std::endl;
  }

  return 0;
}
