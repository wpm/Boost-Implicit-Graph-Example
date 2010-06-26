
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/graph/properties.hpp>
#include <utility> // For std::pair
#include <boost/operators.hpp> // For forward_iterator_helper


// Forward declaration
struct ring_out_edge_iterator;

/*
Undirected graph of vertices arranged in a ring shape.

Vertices are indexed by integer, and edges connect vertices with consecutive
indices.  The n-th vertex is adjacent to the 0-th vertex.  For example, a
ring graph of size 5 looks like this:

                    0
                  /   \
                4      1
                |      |
                3 ---- 2
*/
struct implicit_ring_graph {
  implicit_ring_graph(size_t n):n(n) {};

  // Graph associated types
  typedef size_t vertex_descriptor;
  typedef boost::undirected_tag directed_category;
  typedef boost::disallow_parallel_edge_tag edge_parallel_category;
  typedef boost::incidence_graph_tag traversal_category;

  // IncidenceGraph associated types
  typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
  typedef ring_out_edge_iterator out_edge_iterator;
  typedef size_t degree_size_type;

  // The following additional types are not required by any of the concepts
  // modeled here.  They are still declared here because graph_traits expects
  // them to be in the graph class.
  typedef void adjacency_iterator;
  typedef void vertices_size_type;
  typedef void edges_size_type;
  typedef void in_edge_iterator;
  typedef void vertex_iterator;
  typedef void edge_iterator;

  // The number of vertices in the graph.
  size_t n;
};


/*
Iterator over outgoing edges in a ring graph.

In an undirected graph such as this one, all the incident edges are outgoing
edges.

For vertex i, this returns edge (i, i-1) and then edge (i, i+1), wrapping
around the end of the ring as needed.
*/
struct ring_out_edge_iterator:public boost::forward_iterator_helper <
    ring_out_edge_iterator,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor,
    std::ptrdiff_t,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor*,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor> {
  typedef boost::graph_traits<implicit_ring_graph>::vertex_descriptor vertex;
  typedef boost::graph_traits<implicit_ring_graph>::edge_descriptor edge;

  ring_out_edge_iterator() {}
  ring_out_edge_iterator(size_t i,
                        vertex u,
                        implicit_ring_graph& g):i(i),u(u),n(g.n) {}

  edge operator*() const {
    static const int ring_offset[] = {-1, 1};
    vertex v;
    if (i == 0 && u == 0)
      v = n-1; // Wrap around to the largest vertex
    else
      v = (u+ring_offset[i]) % n;
    return edge(u, v);
  }

  void operator++() {i++;}

  bool operator==(const ring_out_edge_iterator& other) const {
    return i == other.i;
  }

  size_t i; // Index into ring_offset, ranges over {0,1}
  vertex u; // Vertex whose out edges are iterated
  size_t n; // Size of the graph
};

// IncidenceGraph valid expressions
boost::graph_traits<implicit_ring_graph>::vertex_descriptor
source(boost::graph_traits<implicit_ring_graph>::edge_descriptor,
       implicit_ring_graph);

boost::graph_traits<implicit_ring_graph>::vertex_descriptor
target(boost::graph_traits<implicit_ring_graph>::edge_descriptor,
       implicit_ring_graph);

// out_iter is an alias for ring_out_edge_iterator.  Use out_iter because it comes
// from the graph_traits parameterization of implicit_ring_graph.
typedef boost::graph_traits<implicit_ring_graph>::out_edge_iterator out_iter;

std::pair<out_iter, out_iter>
out_edges(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
          implicit_ring_graph);

boost::graph_traits<implicit_ring_graph>::degree_size_type
out_degree(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
          implicit_ring_graph);


/*
Map from edges to weights

Edges are pairs of vertex indices and weights are float values.
*/
struct edge_weight_map {
  typedef float value_type;
  typedef float reference;
  typedef boost::graph_traits<implicit_ring_graph>::edge_descriptor key_type;
  typedef boost::readable_property_map_tag category;

  reference operator[](key_type e) const {
    // All edges have a weight of one.
    return 1;
  }
};


// ReadablePropertyGraph associated types
namespace boost {
  template<>
  struct property_map<implicit_ring_graph, boost::edge_weight_t> {
    typedef edge_weight_map type;
    typedef edge_weight_map const_type;
  };
}

// edge_pmap is an alias for edge_weight_map.  Use edge_pmap because it comes
// from the property_map parameterization of implicit_ring_graph.
typedef boost::property_map<implicit_ring_graph,
                            boost::edge_weight_t>::const_type edge_pmap;

// PropertyMap valid expressions
edge_pmap::reference get(edge_pmap, edge_pmap::key_type);


// ReadablePropertyGraph valid expressions
edge_pmap get(boost::edge_weight_t, const implicit_ring_graph&);

boost::property_traits<edge_pmap>::reference
get(boost::edge_weight_t,
    const implicit_ring_graph&,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor&);

