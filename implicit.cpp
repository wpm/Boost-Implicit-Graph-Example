
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "implicit.hpp"


// AdjacencyGraph model
std::pair<adjacency_iterator, adjacency_iterator>
adjacent_vertices(vertex v, implicit_ring_graph g) {
  return std::pair<adjacency_iterator, adjacency_iterator>(
    adjacency_iterator(0, v, g),
    adjacency_iterator(2, v, g)
  );
}


// PropertyMap model
edge_weight_map::reference get(edge_weight_map pmap, edge_weight_map::key_type key) {
  return pmap[key];
}


// PropertyGraph model
edge_weight_map get(boost::edge_weight_t, const implicit_ring_graph& g) {
  return edge_weight_map();
}


weight get(boost::edge_weight_t tag, const implicit_ring_graph& g, edge& e) {
  return get(tag, g)[e];
}

