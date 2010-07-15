
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "implicit.hpp"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
  // Check the concepts that graph models.  This is included to demonstrate
  // how concept checking works, but is not required for a working program
  // since Boost algorithms do their own concept checking.
  boost::function_requires< boost::BidirectionalGraphConcept<graph> >();
  boost::function_requires< boost::AdjacencyGraphConcept<graph> >();
  boost::function_requires< boost::VertexListGraphConcept<graph> >();
  boost::function_requires< boost::EdgeListGraphConcept<graph> >();
  boost::function_requires< boost::AdjacencyMatrixConcept<graph> >();
  boost::function_requires<
    boost::ReadablePropertyMapConcept<const_edge_weight_map,
                                      edge_descriptor> >();
  boost::function_requires<
    boost::ReadablePropertyGraphConcept<graph,
                                        edge_descriptor,
                                        boost::edge_weight_t> >();

  // Specify the size of the graph on the command line, or use a default size
  // of 5.
  size_t n = argc == 2 ? atoi(argv[1]) : 5;

  // Create a small ring graph.
  graph g(n);
  const_edge_weight_map m = get(boost::edge_weight, g);

  // Print the outgoing edges of all the vertices.  For n=5 this will print:
  //
  // Vertices, outgoing edges, and adjacent vertices
  // Vertex 0: <0, 1>  <0, 4>   Adjacent vertices 1 4
  // Vertex 1: <1, 2>  <0, 1>   Adjacent vertices 2 1
  // Vertex 2: <2, 3>  <1, 2>   Adjacent vertices 3 2
  // Vertex 3: <3, 4>  <2, 3>   Adjacent vertices 4 3
  // Vertex 4: <0, 4>  <3, 4>   Adjacent vertices 4 4
  // 5 vertices
  std::cout << "Vertices, outgoing edges, and adjacent vertices" << std::endl;
  vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++) {
    vertex_descriptor u = *vi;
    std::cout << "Vertex " << u << ": ";
    // Adjacenct edges
    out_edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ei++) {
      edge_descriptor e = *ei;
      std::cout << "<" << e.first << ", " << e.second << ">" << "  ";
    }
    std::cout << " Adjacent vertices ";
    // Adjacent vertices
    adjacency_iterator ai, ai_end;
    for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ai++) {
      std::cout << *ai << " ";
    }
    std::cout << std::endl;
  }
  std::cout << num_vertices(g) << " vertices" << std::endl << std::endl;

  // Print all the edges in the graph along with their weights.  For n=5 this
  // will print:
  //
  // Edges and weights
  // <0, 1> weight 1
  // <1, 2> weight 1
  // <2, 3> weight 1
  // <3, 4> weight 1
  // <0, 4> weight 1
  // 5 edges
  std::cout << "Edges and weights" << std::endl;
  edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ei++) {
    edge_descriptor e = *ei;
    std::cout << "<" << e.first << ", " << e.second << ">"
              << " weight " << get(boost::edge_weight, g, e) << std::endl;
  }
  std::cout << num_edges(g) << " edges"  << std::endl;

  if (n>0) {
    std::cout << std::endl;
    // Do a Dijkstra search from vertex 0.  For n=5 this will print:
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
            boost::predecessor_map(&pred[0]).distance_map(&dist[0]) );

    std::cout << "Dijkstra search from vertex " << source << std::endl;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
      vertex_descriptor u = *vi;
      std::cout << "Vertex " << u << ": "
                << "distance " << dist[u] << ", "
                << "parent "<< pred[*vi]
                << std::endl;
    }
  }

  return 0;
}
