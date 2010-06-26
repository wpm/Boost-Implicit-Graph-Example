
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "implicit.hpp"


// IncidenceGraph valid expressions
boost::graph_traits<implicit_ring_graph>::vertex_descriptor
source(
  boost::graph_traits<implicit_ring_graph>::edge_descriptor e,
  implicit_ring_graph g) {
  // The first vertex in the edge is the source.
  return e.first;
}

boost::graph_traits<implicit_ring_graph>::vertex_descriptor
target(
  boost::graph_traits<implicit_ring_graph>::edge_descriptor e,
  implicit_ring_graph g) {
  // The second vertex in the edge is the target.
  return e.second;
}

std::pair<out_iter, out_iter>
out_edges(boost::graph_traits<implicit_ring_graph>::vertex_descriptor u,
          implicit_ring_graph g) {
  return std::pair<out_iter, out_iter>(
    out_iter(0, u, g),    // The first iterator position
    out_iter(2, u, g) );  // The last iterator position
}

boost::graph_traits<implicit_ring_graph>::degree_size_type
out_degree(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
          implicit_ring_graph) {
  // All vertices in a ring graph have two neighbors.
  return 2;
}


// PropertyMap valid expressions
edge_pmap::reference get(edge_pmap pmap, edge_pmap::key_type key) {
  return pmap[key];
}


// ReadablePropertyGraph valid expressions
edge_pmap get(boost::edge_weight_t, const implicit_ring_graph& g) {
  return edge_pmap();
}

boost::property_traits<edge_pmap>::reference
get(boost::edge_weight_t tag,
    const implicit_ring_graph& g,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor& e) {
  return get(tag, g)[e];
}
