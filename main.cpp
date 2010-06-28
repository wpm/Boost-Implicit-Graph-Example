
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "implicit.hpp"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
  using namespace implicit_ring;

  // Check the concepts that graph models.
  boost::function_requires< boost::IncidenceGraphConcept<graph> >();
  boost::function_requires< boost::VertexListGraphConcept<graph> >();
  // boost::function_requires< boost::EdgeListGraphConcept<graph> >();

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
  std::cout << "Vertices, edges, and weights" << std::endl;
  vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++) {
    vertex_descriptor u = *vi;
    std::cout << "Vertex " << u << ": ";
    out_edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ei++) {
      edge_descriptor e = *ei;
      std::cout << "<" << e.first << ", " << e.second << ">"
                << "(" << get(boost::edge_weight, g, e) << ")" << "  ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  // Do a Dijkstra search from vertex 0.
  //
  // Vertex 0: distance 0, parent 0
  // Vertex 1: distance 1, parent 0
  // Vertex 2: distance 2, parent 1
  // Vertex 3: distance 2, parent 4
  // Vertex 4: distance 1, parent 0
  vertex_descriptor source = 0;
  std::vector<vertex_descriptor> pred(num_vertices(g));
  std::vector<edge_weight_map_reference> dist(num_vertices(g));

  boost::dijkstra_shortest_paths(g, source,
          boost::predecessor_map(&pred[0]).
          distance_map(&dist[0]).
          // The identity property map tells the Dijkstra algorithm that
          // vertex descriptors may be used as array indexes.
          vertex_index_map(boost::identity_property_map()) );

  std::cout << "Dijkstra search from vertex " << source << std::endl;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    vertex_descriptor u = *vi;
    std::cout << "Vertex " << u << ": "
              << "distance " << dist[u] << ", "
              << "parent "<< pred[*vi]
              << std::endl;
  }

  return 0;
}
