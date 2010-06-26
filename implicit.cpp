
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "implicit.hpp"


// AdjacencyGraph model
typedef boost::graph_traits<implicit_ring_graph>::adjacency_iterator
  adjacency_iter;
typedef boost::graph_traits<implicit_ring_graph>::vertex_descriptor vertex;

std::pair<adjacency_iter, adjacency_iter>
adjacent_vertices(vertex v, implicit_ring_graph g) {
  return std::pair<adjacency_iter, adjacency_iter>(
    adjacency_iter(0, v, g),  // The first iterator position
    adjacency_iter(2, v, g)   // The last iterator position
  );
}


// PropertyMap model
edge_pmap::reference get(edge_pmap pmap, edge_pmap::key_type key) {
  return pmap[key];
}


// ReadablePropertyGraph model
edge_weight_map get(boost::edge_weight_t, const implicit_ring_graph& g) {
  return edge_weight_map();
}

boost::property_traits<edge_weight_map>::reference
get(boost::edge_weight_t tag,
    const implicit_ring_graph& g,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor& e) {
  return get(tag, g)[e];
}
