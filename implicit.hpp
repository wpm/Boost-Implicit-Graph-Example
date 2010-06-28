
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/graph/properties.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <utility>


/*
The file defines a ring-shaped graph usng the boost graph library.

The ring graph is an undirected graph of size n whose vertices indexed are by
integer and arranged sequentially so that each vertex i is adjacent
to i-1 for i>0 and i+1 for i<n-1.  Vertex 0 is also adjacent to vertex n-1.

For example, a ring graph of size n=5 looks like this:

                    0
                  /   \
                4      1
                |      |
                3 ---- 2

Each edge has a read-only floating point weight associated with it.

The graph is defined inside the implicit_ring namespace.  Various aspects of
the graph are implemented by the following structures:

  implicit_ring::graph
    Defines types for the Graph and IncidenceGraph concepts

  implicit_ring::ring_out_edge_iterator
    Implements the ring topology

  implicit_ring::edge_weight_map
    Defines a property map between edges and weights

  boost::property_map<implicit_ring::graph, boost::edge_weight_t>
    Associates the edges of the ring graph with the edge weight map

Along with the various valid expression functions, these define a model of a
Boost Graph Library graph concept.
*/

// Forward declarations
namespace implicit_ring {
  class graph;
  class ring_out_edge_iterator;
  class ring_edge_iterator;
  struct edge_weight_map;
}

// ReadablePropertyGraph associated types
namespace boost {
  // This has to be declared outside the implicit_ring namespace block so
  // that the namespaces do not nest.
  template<>
  struct property_map<implicit_ring::graph,
                      edge_weight_t> {
    typedef implicit_ring::edge_weight_map type;
    typedef implicit_ring::edge_weight_map const_type;
  };
}

namespace implicit_ring {
  struct ring_traversal_catetory:virtual public boost::incidence_graph_tag,
                                 virtual public boost::vertex_list_graph_tag,
                                 virtual public boost::edge_list_graph_tag
                                 {};
  /*
  Undirected graph of vertices arranged in a ring shape.

  Vertices are indexed by integer, and edges connect vertices with consecutive
  indices.  Vertex 0 is also adjacent to the vertex n-1.
  */
  class graph {
  public:
    // Graph associated types
    typedef size_t vertex_descriptor;
    typedef boost::undirected_tag directed_category;
    typedef boost::disallow_parallel_edge_tag edge_parallel_category;
    typedef ring_traversal_catetory traversal_category;

    // IncidenceGraph associated types
    typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
    typedef ring_out_edge_iterator out_edge_iterator;
    typedef size_t degree_size_type;

    // VertexListGraph associated types
    typedef boost::counting_iterator<vertex_descriptor> vertex_iterator;
    typedef size_t vertices_size_type;

    // EdgeListGraph associated types
    typedef ring_edge_iterator edge_iterator;
    typedef size_t edges_size_type;

    // The following additional types are not required by any of the concepts
    // modeled here.  They are still declared here because graph_traits expects
    // them to be in the graph class.
    typedef void in_edge_iterator;
    typedef void adjacency_iterator;

    graph(size_t n):m_n(n) {};

    size_t n() const {return m_n;}

  private:
    // The number of vertices in the graph.
    size_t m_n;
  };

  // Use these graph_traits parameterizations to refer to the associated
  // graph types.
  typedef boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph>::edge_descriptor edge_descriptor;
  typedef boost::graph_traits<graph>::out_edge_iterator out_edge_iterator;
  typedef boost::graph_traits<graph>::degree_size_type degree_size_type;
  typedef boost::graph_traits<graph>::vertex_iterator vertex_iterator;
  typedef boost::graph_traits<graph>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<graph>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph>::edges_size_type edges_size_type;


  /*
  Each vertex has two neighbors: the one that comes before it in the ring and
  the one that comes after.  The PREV and NEXT values correspond to these two
  neighbors, while END is a sentinel value.  These are used as offsets into the
  ring_offset array in ring_out_edge_iterator::dereference.

  A postfix increment operator is defined for use in
  ring_out_edge_iterator::increment.
  */
  typedef enum {PREV, NEXT, END} out_edge_iterator_position;

  inline out_edge_iterator_position
  operator++(out_edge_iterator_position &rs, int) {
    return rs = (out_edge_iterator_position)(rs + 1);
  }

  /*
  Iterator over outgoing edges in a ring graph.

  Note that in an undirected graph, all the incident edges are outgoing edges.

  For vertex i, this returns edge (i, i-1) and then edge (i, i+1), wrapping
  around the end of the ring as needed.
  */
  class ring_out_edge_iterator:public boost::iterator_facade <
      ring_out_edge_iterator,
      edge_descriptor,
      boost::forward_traversal_tag,
      edge_descriptor > {
  public:
    ring_out_edge_iterator():m_p(PREV),m_u(0),m_n(0) {};
    explicit ring_out_edge_iterator(out_edge_iterator_position p,
                                    vertex_descriptor u,
                                    const graph& g):
                                    m_p(p),
                                    m_u(u),
                                    m_n( g.n() ) {};

  private:
    friend class boost::iterator_core_access;

    void increment() {m_p++;}

    bool equal(ring_out_edge_iterator const& other) const {
      return this->m_p == other.m_p;
    }

    edge_descriptor dereference() const {
      static const int ring_offset[] = {-1, 1};
      vertex_descriptor v;

      if (m_p == PREV && m_u == 0)
        v = m_n-1; // Wrap around to the largest vertex
      else
        v = (m_u+ring_offset[m_p]) % m_n;
      return edge_descriptor(m_u, v);
    }

    out_edge_iterator_position m_p;
    vertex_descriptor m_u; // Vertex whose out edges are iterated
    size_t m_n; // Size of the graph
  };


  // IncidenceGraph valid expressions
  vertex_descriptor source(edge_descriptor, const graph&);

  inline vertex_descriptor
  source(edge_descriptor e, const graph& g) {
    // The first vertex in the edge is the source.
    return e.first;
  }


  vertex_descriptor target(edge_descriptor, graph&);

  inline vertex_descriptor
  target(edge_descriptor e, const graph& g) {
   // The second vertex in the edge is the target.
   return e.second;
  }

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(vertex_descriptor, const graph&);

  inline std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(vertex_descriptor u, const graph& g) {
    return std::pair<out_edge_iterator, out_edge_iterator>(
      out_edge_iterator(PREV, u, g),   // The first iterator position
      out_edge_iterator(END, u, g) );  // The last iterator position
  }


  degree_size_type out_degree(vertex_descriptor, const graph&);

  inline degree_size_type
  out_degree(vertex_descriptor, const graph&) {
    // All vertices in a ring graph have two neighbors.
    return 2;
  }

  // VertexListGraph valid expressions
  vertices_size_type num_vertices(const graph&);

  inline vertices_size_type num_vertices(const graph& g) {
    return g.n();
  };

  std::pair<vertex_iterator, vertex_iterator> vertices(const graph&);
  inline std::pair<vertex_iterator, vertex_iterator>
  vertices(const graph& g) {
    return std::pair<vertex_iterator, vertex_iterator>(
      vertex_iterator(0),                 // The first iterator position
      vertex_iterator(num_vertices(g)) ); // The last iterator position
  }

  // Tag values passed to the constructor of ring_edge_iterator.
  struct edge_iterator_position {};
  struct edge_iterator_start:virtual public edge_iterator_position {};
  struct edge_iterator_end:virtual public edge_iterator_position {};

  /*
  Iterator over edges in a ring graph.
  
  This object iterates over all the vertices in the graph, then for each
  vertex returns its first outgoing edge.
  */
  class ring_edge_iterator:public boost::iterator_facade <
      ring_edge_iterator,
      edge_descriptor,
      boost::forward_traversal_tag,
      edge_descriptor > {
  public:
    // ring_edge_iterator():m_g(&graph(0)),m_vi(0) {};
    explicit ring_edge_iterator(const graph& g, edge_iterator_start):
      m_g((graph&)g) {
      vertex_iterator vi, vi_end;
      tie(vi, vi_end) = vertices(g);
      m_vi = vi;
    };
    explicit ring_edge_iterator(const graph& g, edge_iterator_end):
      m_g((graph&)g) {
      vertex_iterator vi, vi_end;
      tie(vi, vi_end) = vertices(g);
      m_vi = vi_end;
    };

    ring_edge_iterator& operator=(ring_edge_iterator const& other) {
      if (this != &other) {
        m_g = other.m_g;
        m_vi = other.m_vi;
      }
      return *this;
    }

  private:
    friend class boost::iterator_core_access;

    void increment() { m_vi++;}

    bool equal(ring_edge_iterator const& other) const {
      return this->m_vi == other.m_vi;
    }

    edge_descriptor dereference() const {
      out_edge_iterator ei, ei_end;
      tie(ei, ei_end) = out_edges(*m_vi, m_g);
      return *ei;
    }

    // The graph being iterated over
    graph& m_g;
    // Current vertex
    vertex_iterator m_vi;
  };


  // EdgeListGraph valid expressions
  std::pair<edge_iterator, edge_iterator> edges(const graph&);
  
  inline std::pair<edge_iterator, edge_iterator> edges(const graph& g) {
    return std::pair<edge_iterator, edge_iterator>(
      ring_edge_iterator(g, edge_iterator_start()),
      ring_edge_iterator(g, edge_iterator_end()) );
  }
  
  edges_size_type num_edges(const graph&);

  inline edges_size_type num_edges(const graph& g) {
    // There are as many edges as there are vertices.
    return g.n();
  }

  /*
  Map from edges to floating point weight values
  */
  struct edge_weight_map {
    typedef float value_type;
    typedef float reference;
    typedef edge_descriptor key_type;
    typedef boost::readable_property_map_tag category;

    reference operator[](key_type e) const {
      // All edges have a weight of one.
      return 1;
    }
  };

  // Use these propety_map and property_traits parameterizations to refer to
  // the associated property map types.
  typedef boost::property_map<graph,
                              boost::edge_weight_t>::const_type
          const_edge_weight_map;
  typedef boost::property_traits<const_edge_weight_map>::reference
          edge_weight_map_reference;
  typedef boost::property_traits<const_edge_weight_map>::key_type
          edge_weight_map_key;

  // PropertyMap valid expressions
  edge_weight_map_reference get(const_edge_weight_map, edge_weight_map_key);

  inline edge_weight_map_reference
  get(const_edge_weight_map pmap, edge_weight_map_key e) {
    return pmap[e];
  }


  // ReadablePropertyGraph valid expressions
  const_edge_weight_map get(boost::edge_weight_t, const graph&);

  inline const_edge_weight_map
  get(boost::edge_weight_t, const graph& g) {
    return const_edge_weight_map();
  }

  edge_weight_map_reference get(boost::edge_weight_t,
                                const graph&,
                                edge_weight_map_key);

  inline edge_weight_map_reference get(boost::edge_weight_t tag,
                                       const graph& g,
                                       edge_weight_map_key e) {
    return get(tag, g)[e];
  }
  
}
