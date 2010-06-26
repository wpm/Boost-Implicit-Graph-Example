
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/graph/properties.hpp>
#include <utility> // For std::pair
#include <boost/operators.hpp> // For forward_iterator_helper


// Forward declaration
struct ring_adjacency_iterator;

/*
Undirected graph of vertices arranged in a ring shape.

Vertices are indexed by integer, and edges connect vertices with consecutive
indices.  The n-th vertex is adjacent to the 0-th vertex.  For example, a
ring graph of size 3 looks like this:

                    0
                  /   \
                1 ----- 2
*/
struct implicit_ring_graph {
  implicit_ring_graph(size_t n):n(n) {};

  // Graph model
  typedef size_t vertex_descriptor;
  typedef boost::undirected_tag directed_category;
  typedef boost::disallow_parallel_edge_tag edge_parallel_category;
  typedef boost::adjacency_graph_tag traversal_category;

  // AdjacencyGraph model
  typedef ring_adjacency_iterator adjacency_iterator;

  // Additional types required by the concept-checking code.
  typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
  typedef size_t vertices_size_type;
  typedef size_t edges_size_type;
  typedef size_t degree_size_type;

  typedef void out_edge_iterator;
  typedef void in_edge_iterator;
  typedef void vertex_iterator;
  typedef void edge_iterator;

  // The number of vertices in the graph.
  size_t n;
};


// AdjacencyGraph model

/*
Iterator over adjacent vertices in a ring graph.

For vertex i, this returns vertex i-1 and then vertex i+1, wrapping around
the end of the ring as needed.
*/
struct ring_adjacency_iterator:public boost::forward_iterator_helper <
    ring_adjacency_iterator,
    boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
    std::ptrdiff_t,
    boost::graph_traits<implicit_ring_graph>::vertex_descriptor*,
    boost::graph_traits<implicit_ring_graph>::vertex_descriptor> {
  typedef boost::graph_traits<implicit_ring_graph>::vertex_descriptor vertex;

  ring_adjacency_iterator() {}
  ring_adjacency_iterator(size_t i,
                          vertex v,
                          implicit_ring_graph& g):i(i),v(v),n(g.n) {}

  vertex operator*() const {
    static const int ring_offset[] = {-1, 1};
    if (i == 0 && v == 0)
      return n-1; // Wrap around to the largest vertex
    else
      return (v+ring_offset[i]) % n;
  }
  void operator++() {i++;}
  bool operator==(const ring_adjacency_iterator& other) const {
    return i == other.i;
  }

  size_t i; // Index into ring_offset, ranges over {0,1}
  vertex v; // Vertex whose neighbors are iterated
  size_t n; // Size of the graph
};

std::pair<
  boost::graph_traits<implicit_ring_graph>::adjacency_iterator,
  boost::graph_traits<implicit_ring_graph>::adjacency_iterator >
adjacent_vertices(
  boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
  implicit_ring_graph);



// PropertyMap model

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


// ReadablePropertyGraph model
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

// PropertyMap model
edge_pmap::reference get(edge_pmap, edge_pmap::key_type);


// ReadablePropertyGraph model
edge_pmap get(boost::edge_weight_t, const implicit_ring_graph&);

boost::property_traits<edge_pmap>::reference
get(boost::edge_weight_t,
    const implicit_ring_graph&,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor&);

