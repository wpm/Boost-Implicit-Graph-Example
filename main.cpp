
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "implicit.hpp"
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
  using namespace implicit_ring;

  // Check the concepts that graph models.
  boost::function_requires< boost::IncidenceGraphConcept<graph> >();
  boost::function_requires< boost::VertexListGraphConcept<graph> >();

  boost::function_requires<
    boost::ReadablePropertyMapConcept<const_edge_weight_map,
                                      edge_descriptor> >();
  boost::function_requires<
    boost::ReadablePropertyGraphConcept<graph,
                                        edge_descriptor,
                                        boost::edge_weight_t> >();

  // Create a small ring graph.
  graph g(5);
  const_edge_weight_map m = get(boost::edge_weight, g);

  // Print the outgoing edges of all vertices along with their weights.
  //  
  // Vertex 0: <0, 4>(1)  <0, 1>(1)
  // Vertex 1: <1, 0>(1)  <1, 2>(1)
  // Vertex 2: <2, 1>(1)  <2, 3>(1)
  // Vertex 3: <3, 2>(1)  <3, 4>(1)
  // Vertex 4: <4, 3>(1)  <4, 0>(1)
  vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++) {
    vertex_descriptor u = *vi;
    std::cout << "Vertex " << u << ": ";
    out_edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ei++) {
      edge_descriptor e = *ei;
      std::cout << "<" << e.first << ", " << e.second << ">";
      std::cout << "(" << get(boost::edge_weight, g, e) << ")" << "  ";
    }
    std::cout << std::endl;
  }

  return 0;
}
