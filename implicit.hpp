
//          Copyright W.P. McNeill 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/graph/properties.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <utility>


/*
This file defines a simple, read-only, implicit ring graph using the boost
graph library.

A ring graph is an undirected graph of size n whose vertices indexed are by
integer and arranged sequentially so that each vertex i is adjacent
to i-1 for i>0 and i+1 for i<n-1.  Vertex 0 is also adjacent to vertex n-1.

For example, a ring graph of size n=5 looks like this:

                    0
                  /   \
                4      1
                |      |
                3 ---- 2

Additionally, each edge has a read-only floating point weight associated with
it.  Here all edges have a weight of 1.

The graph is defined inside the implicit_ring namespace.  Various aspects of
the graph are implemented by the following structures:

  implicit_ring::graph
    Defines types for the concepts this graph models

  implicit_ring::ring_incident_edge_iterator
    Implements the ring topology

  implicit_ring::edge_weight_map
    Defines a property map between edges and weights

  boost::property_map<implicit_ring::graph, boost::edge_weight_t>
    Associates the edges of the ring graph with the edge weight map

Along with the various valid expression functions, these define a model of a
Boost Graph Library graph concept.

This object models the following concepts:
  Graph
  IncidenceGraph
  BidirectionalGraph
  VertexListGraph
  EdgeListGraph
  ReadablePropertyGraph
*/

// Forward declarations
namespace implicit_ring {
  class graph;
  class ring_incident_edge_iterator;
  class ring_adjacency_iterator;
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
  // Tag values that specify the traversal type in graph::traversal_category.
  struct ring_traversal_catetory:
    virtual public boost::bidirectional_graph_tag,
    virtual public boost::adjacency_graph_tag,
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
    typedef ring_incident_edge_iterator out_edge_iterator;
    typedef size_t degree_size_type;

    // BidirectionalGraph associated types
    // Note that undirected graphs make no distinction between in- and out-
    // edges.
    typedef ring_incident_edge_iterator in_edge_iterator;

    // AdjacencyGraph associated types
    typedef ring_adjacency_iterator adjacency_iterator;

    // VertexListGraph associated types
    typedef boost::counting_iterator<vertex_descriptor> vertex_iterator;
    typedef size_t vertices_size_type;

    // EdgeListGraph associated types
    typedef ring_edge_iterator edge_iterator;
    typedef size_t edges_size_type;

    // This type is not part of a graph concept, but is used to return the
    // default vertex index map used by the Dijkstra search algorithm.
    typedef vertex_descriptor vertex_property_type;

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
  typedef boost::graph_traits<graph>::in_edge_iterator in_edge_iterator;
  typedef boost::graph_traits<graph>::adjacency_iterator adjacency_iterator;
  typedef boost::graph_traits<graph>::degree_size_type degree_size_type;
  typedef boost::graph_traits<graph>::vertex_iterator vertex_iterator;
  typedef boost::graph_traits<graph>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<graph>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph>::edges_size_type edges_size_type;


  // Tag values passed to an iterator constructor to specify whether it should
  // be set to the start or the end of its range.
  struct iterator_position {};
  struct iterator_start:virtual public iterator_position {};
  struct iterator_end:virtual public iterator_position {};

  /*
  Each vertex has two neighbors: the one that comes after it in the ring and
  the one that comes before.  The NEXT and PREV values correspond to these two
  neighbors, while END is a sentinel value.  These are used as offsets into the
  ring_offset array in ring_incident_edge_iterator::dereference.

  A postfix increment operator is defined for use in
  ring_incident_edge_iterator::increment.
  */
  typedef enum {NEXT, PREV, END} out_edge_iterator_position;

  inline out_edge_iterator_position
  operator++(out_edge_iterator_position &rs, int) {
    return rs = (out_edge_iterator_position)(rs + 1);
  }

  /*
  Iterator over edges incident on a vertex in a ring graph.

  For vertex i, this returns edge (i, i+1) and then edge (i, i-1), wrapping
  around the end of the ring as needed.
  */
  class ring_incident_edge_iterator:public boost::iterator_facade <
      ring_incident_edge_iterator,
      edge_descriptor,
      boost::forward_traversal_tag,
      edge_descriptor > {
  public:
    ring_incident_edge_iterator():m_n(0),m_u(0),m_p(NEXT) {};
    explicit ring_incident_edge_iterator(const graph& g,
                                    vertex_descriptor u,
                                    iterator_start):
                                    m_n(g.n()),m_u(u),m_p(NEXT) {};
    explicit ring_incident_edge_iterator(const graph& g,
                                    vertex_descriptor u,
                                    iterator_end):
                                    m_n(g.n()),m_u(u),m_p(END) {};

  private:
    friend class boost::iterator_core_access;

    void increment() {m_p++;}

    bool equal(const ring_incident_edge_iterator& other) const {
      return this->m_p == other.m_p;
    }

    edge_descriptor dereference() const {
      static const int ring_offset[] = {1, -1};
      vertex_descriptor v;

      if (m_p == PREV && m_u == 0)
        v = m_n-1; // Wrap around to the largest vertex
      else
        v = (m_u+ring_offset[m_p]) % m_n;
      return edge_descriptor(m_u, v);
    }

    size_t m_n; // Size of the graph
    vertex_descriptor m_u; // Vertex whose out edges are iterated
    out_edge_iterator_position m_p; // Current offset into ring_offset array
  };


  // IncidenceGraph valid expressions
  vertex_descriptor source(edge_descriptor, const graph&);

  inline vertex_descriptor
  source(edge_descriptor e, const graph& g) {
    // The first vertex in the edge pair is the source.
    return e.first;
  }


  vertex_descriptor target(edge_descriptor, graph&);

  inline vertex_descriptor
  target(edge_descriptor e, const graph& g) {
   // The second vertex in the edge pair is the target.
   return e.second;
  }

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(vertex_descriptor, const graph&);

  inline std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(vertex_descriptor u, const graph& g) {
    return std::pair<out_edge_iterator, out_edge_iterator>(
      out_edge_iterator(g, u, iterator_start()),// The first iterator position
      out_edge_iterator(g, u, iterator_end()) );// The last iterator position
  }


  degree_size_type out_degree(vertex_descriptor, const graph&);

  inline degree_size_type
  out_degree(vertex_descriptor, const graph&) {
    // All vertices in a ring graph have two neighbors.
    return 2;
  }


  // BidirectionalGraph valid expressions
  std::pair<in_edge_iterator, in_edge_iterator>
  in_edges(vertex_descriptor, const graph&);

  inline std::pair<in_edge_iterator, in_edge_iterator>
  in_edges(vertex_descriptor u, const graph& g) {
    // The in-edges and out-edges are the same in an undirected graph.
    return out_edges(u, g);
  }

  degree_size_type in_degree(vertex_descriptor, const graph&);

  inline degree_size_type in_degree(vertex_descriptor u, const graph& g) {
    // The in-degree and out-degree are both equal to the number of incident
    // edges in an undirected graph.
    return out_degree(u, g);
  }

  degree_size_type degree(vertex_descriptor, const graph&);

  inline degree_size_type degree(vertex_descriptor u, const graph& g) {
    // The in-degree and out-degree are both equal to the number of incident
    // edges in an undirected graph.
    return out_degree(u, g);
  }


  /*
  Iterator over vertices adjacent to a given vertex.

  This iterates over the target vertices of all the incident edges.
  */
  class ring_adjacency_iterator:public boost::iterator_adaptor<
          ring_adjacency_iterator,
          out_edge_iterator,
          vertex_descriptor,
          boost::use_default,
          vertex_descriptor> {
  public:
    ring_adjacency_iterator() {};
    ring_adjacency_iterator(const out_edge_iterator& ei, const graph* g):
      ring_adjacency_iterator::iterator_adaptor_(ei),m_g(m_g) {};

  private:
    friend class boost::iterator_core_access;

    vertex_descriptor dereference() const {
      return target(*this->base(), *m_g);
    }
    
    const graph* m_g;
  };


  std::pair<adjacency_iterator, adjacency_iterator>
  adjacent_vertices(vertex_descriptor, const graph&);
  
  inline std::pair<adjacency_iterator, adjacency_iterator>
  adjacent_vertices(vertex_descriptor u, const graph& g) {
    return std::pair<adjacency_iterator, adjacency_iterator>(
      adjacency_iterator(out_edge_iterator(g, u, iterator_start()), &g),
      adjacency_iterator(out_edge_iterator(g, u, iterator_end()), &g));
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
    ring_edge_iterator():m_g(NULL),m_vi(0) {};
    explicit ring_edge_iterator(const graph& g, iterator_start):
      m_g((graph *)&g),m_vi(vertices(g).first) {};
    explicit ring_edge_iterator(const graph& g, iterator_end):
      m_g((graph *)&g),m_vi(vertices(g).second) {};

    ring_edge_iterator& operator=(const ring_edge_iterator& other) {
      if (this != &other) {
        m_g = other.m_g;
        m_vi = other.m_vi;
      }
      return *this;
    }

  private:
    friend class boost::iterator_core_access;

    void increment() { m_vi++;}

    bool equal(const ring_edge_iterator& other) const {
      return this->m_vi == other.m_vi;
    }

    edge_descriptor dereference() const {
      // The first element in the incident edge list of the current vertex.
      return *(out_edges(*m_vi, *m_g).first);
    }

    graph *m_g; // The graph being iterated over
    vertex_iterator m_vi; // Current vertex
  };


  // EdgeListGraph valid expressions
  std::pair<edge_iterator, edge_iterator> edges(const graph&);
  
  inline std::pair<edge_iterator, edge_iterator> edges(const graph& g) {
    return std::pair<edge_iterator, edge_iterator>(
      ring_edge_iterator(g, iterator_start()),
      ring_edge_iterator(g, iterator_end()) );
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


  // This expression is not part of a graph concept, but is used to return the
  // default vertex index map used by the Dijkstra search algorithm.
  boost::identity_property_map get(boost::vertex_index_t, const graph&);

  boost::identity_property_map get(boost::vertex_index_t, const graph&) {
    // The vertex descriptors are already unsigned integer indices, so just
    // return an identity map.
    return boost::identity_property_map();
  }
}
