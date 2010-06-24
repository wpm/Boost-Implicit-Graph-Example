#include <boost/graph/properties.hpp>
#include <utility> // For std::pair
#include <boost/operators.hpp> // For forward_iterator_helper


// Forward declaration
struct RingAdjacencyIterator;

/*
Undirected graph of vertices arranged in a ring shape.

Vertices are indexed by integer, and edges connect vertices with consecutive
indices.  The n-th vertex is adjacent to the 0-th vertex.  For example, a
ring graph of size 3 looks like this:

								    0
								  /   \
								1 ----- 2

Models: Graph, AdjacencyGraph, PropertyGraph
*/
struct ImplicitRingGraph {
	ImplicitRingGraph(size_t n):n(n) {};

	// Graph concept
	typedef size_t vertex_descriptor;
	typedef boost::undirected_tag directed_category;
	typedef boost::disallow_parallel_edge_tag edge_parallel_category;
	typedef boost::adjacency_graph_tag traversal_category;

	// AdjacencyGraph concept
	typedef RingAdjacencyIterator adjacency_iterator;

	// PropertyGraph concept
	typedef boost::edge_weight_t edge_property_type;

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

// Short names from graph traits types
typedef boost::graph_traits<ImplicitRingGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<ImplicitRingGraph>::edge_descriptor Edge;
typedef boost::graph_traits<ImplicitRingGraph>::adjacency_iterator AdjacencyIterator;


/*
Iterator over adjacent vertices in a ring graph.

For vertex i, this returns vertex i-1 and then vertex i+1, wrapping around
the end of the ring as needed.
*/
struct RingAdjacencyIterator:public boost::forward_iterator_helper <
		RingAdjacencyIterator,
		Vertex,
		std::ptrdiff_t,
		Vertex*,
		Vertex> {
	RingAdjacencyIterator() {}
	RingAdjacencyIterator(size_t i,
						  Vertex v,
						  ImplicitRingGraph& g):i(i),v(v),n(g.n) {}
	
	Vertex operator*() const {
		static const int ring_offset[] = {-1, 1};
		if (i == 0 && v == 0)
			return n-1; // Wrap around to the largest vertex
		else
			return (v+ring_offset[i]) % n;
	}
	void operator++() {i++;}
	bool operator==(const RingAdjacencyIterator& other) const {
		return i == other.i;
	}
	
	size_t i; // Index into ring_offset, ranges over {0,1}
	Vertex v; // Vertex whose neighbors are iterated
	size_t n; // Size of the graph
};


/*
Map from edges to weights

Edges are pairs of vertex indices and weights are float values.

Models: ReadablePropertyMap
*/
struct EdgeWeightMap {
	typedef float value_type;
	typedef float reference;
	typedef Edge key_type;
	typedef boost::readable_property_map_tag category;
	
	reference operator[](key_type e) const {
		return 1;
	}
};

// Short names from property traits types
typedef boost::property_traits<EdgeWeightMap>::reference EdgeWeight;


// AdjacencyGraph concept
std::pair<AdjacencyIterator, AdjacencyIterator>
adjacent_vertices(Vertex, ImplicitRingGraph);

// PropertyMap concept
EdgeWeightMap::reference get(EdgeWeightMap, EdgeWeightMap::key_type);

// PropertyGraph concept
EdgeWeightMap get(boost::edge_weight_t, ImplicitRingGraph&);

EdgeWeight get(boost::edge_weight_t, ImplicitRingGraph&, Edge);

