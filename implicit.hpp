
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/graph/properties.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <utility> // For std::pair


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
  implicit_ring_graph(size_t n):m_n(n) {};

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
  size_t m_n;
};


/*
Each vertex has two neighbors: the one that comes before it in the ring and
the one that comes after.  The PREV and NEXT values correspond to these two
neighbors, while END is a sentinel value.  These are used as offsets into the
ring_offset array in ring_out_edge_iterator::dereference.

A postfix increment operator is defined for use in
ring_out_edge_iterator::increment.
*/
typedef enum {PREV, NEXT, END} ring_out_edge_iterator_position;

inline ring_out_edge_iterator_position
operator++(ring_out_edge_iterator_position &rs, int) {
  return rs = (ring_out_edge_iterator_position)(rs + 1);
}

/*
Iterator over outgoing edges in a ring graph.

Note that in an undirected graph, all the incident edges are outgoing edges.

For vertex i, this returns edge (i, i-1) and then edge (i, i+1), wrapping
around the end of the ring as needed.
*/
class ring_out_edge_iterator:public boost::iterator_facade <
    ring_out_edge_iterator,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor,
    boost::forward_traversal_tag,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor > {
  typedef boost::graph_traits<implicit_ring_graph>::vertex_descriptor vertex;
  typedef boost::graph_traits<implicit_ring_graph>::edge_descriptor edge;

public:
  ring_out_edge_iterator():m_p(PREV),m_u(0),m_n(0) {};
  explicit ring_out_edge_iterator(ring_out_edge_iterator_position p,
                                  vertex u,
                                  implicit_ring_graph& g):
                                  m_p(p),m_u(u),m_n(g.m_n) {};

private:
  friend class boost::iterator_core_access;

  void increment() {m_p++;}

  bool equal(ring_out_edge_iterator const& other) const {
    return this->m_p == other.m_p;
  }

  edge dereference() const {
    static const int ring_offset[] = {-1, 1};
    vertex v;

    if (m_p == PREV && m_u == 0)
      v = m_n-1; // Wrap around to the largest vertex
    else
      v = (m_u+ring_offset[m_p]) % m_n;
    return edge(m_u, v);
  }

  ring_out_edge_iterator_position m_p;
  vertex m_u; // Vertex whose out edges are iterated
  size_t m_n; // Size of the graph
};


// IncidenceGraph valid expressions
boost::graph_traits<implicit_ring_graph>::vertex_descriptor
source(boost::graph_traits<implicit_ring_graph>::edge_descriptor,
       implicit_ring_graph);

inline boost::graph_traits<implicit_ring_graph>::vertex_descriptor
source(boost::graph_traits<implicit_ring_graph>::edge_descriptor e,
       implicit_ring_graph g) {
  // The first vertex in the edge is the source.
  return e.first;
}


boost::graph_traits<implicit_ring_graph>::vertex_descriptor
target(boost::graph_traits<implicit_ring_graph>::edge_descriptor,
       implicit_ring_graph);

inline boost::graph_traits<implicit_ring_graph>::vertex_descriptor
target(boost::graph_traits<implicit_ring_graph>::edge_descriptor e,
       implicit_ring_graph g) {
 // The second vertex in the edge is the target.
 return e.second;
}

// out_iter is an alias for ring_out_edge_iterator.  Use out_iter because it comes
// from the graph_traits parameterization of implicit_ring_graph.
typedef boost::graph_traits<implicit_ring_graph>::out_edge_iterator out_iter;

std::pair<out_iter, out_iter>
out_edges(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
          implicit_ring_graph);

inline std::pair<out_iter, out_iter>
out_edges(boost::graph_traits<implicit_ring_graph>::vertex_descriptor u,
          implicit_ring_graph g) {
  return std::pair<out_iter, out_iter>(
    out_iter(PREV, u, g),   // The first iterator position
    out_iter(END, u, g) );  // The last iterator position
}


boost::graph_traits<implicit_ring_graph>::degree_size_type
out_degree(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
           implicit_ring_graph);

inline boost::graph_traits<implicit_ring_graph>::degree_size_type
out_degree(boost::graph_traits<implicit_ring_graph>::vertex_descriptor,
           implicit_ring_graph) {
  // All vertices in a ring graph have two neighbors.
  return 2;
}


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

inline edge_pmap::reference get(edge_pmap pmap, edge_pmap::key_type key) {
  return pmap[key];
}


// ReadablePropertyGraph valid expressions
edge_pmap get(boost::edge_weight_t, const implicit_ring_graph&);

inline edge_pmap get(boost::edge_weight_t, const implicit_ring_graph& g) {
  return edge_pmap();
}

boost::property_traits<edge_pmap>::reference
get(boost::edge_weight_t,
    const implicit_ring_graph&,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor&);

inline boost::property_traits<edge_pmap>::reference
get(boost::edge_weight_t tag,
    const implicit_ring_graph& g,
    boost::graph_traits<implicit_ring_graph>::edge_descriptor& e) {
  return get(tag, g)[e];
}

