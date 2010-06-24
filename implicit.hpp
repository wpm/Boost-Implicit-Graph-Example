#include <boost/graph/properties.hpp>
#include <utility> // For std::pair
#include <boost/operators.hpp> // For forward_iterator_helper


struct RingAdjacencyIterator;


struct ImplicitRingGraph {
	ImplicitRingGraph(size_t n):n(n) {};

	// Graph concept
	typedef size_t vertex_descriptor;
	typedef boost::undirected_tag directed_category;
	typedef boost::disallow_parallel_edge_tag edge_parallel_category;
	typedef boost::adjacency_graph_tag traversal_category;

	// PropertyGraph concept
	typedef boost::edge_weight_t edge_property_type;

	typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
	typedef size_t vertices_size_type;
	typedef size_t edges_size_type;
	typedef size_t degree_size_type;
	
	typedef RingAdjacencyIterator adjacency_iterator;
	typedef void out_edge_iterator;
	typedef void in_edge_iterator;
	typedef void vertex_iterator;
	typedef void edge_iterator;

	size_t n;
};

typedef boost::graph_traits<ImplicitRingGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<ImplicitRingGraph>::edge_descriptor Edge;
typedef boost::graph_traits<ImplicitRingGraph>::adjacency_iterator AdjacencyIterator;


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
	Vertex v;
	size_t n; // The size of the graph
};


struct EdgeWeightMap {
	typedef float value_type;
	typedef float reference;
	typedef Edge key_type;
	typedef boost::readable_property_map_tag category;
	
	reference operator[](key_type e) const {
		return 1;
	}
};

typedef boost::property_traits<EdgeWeightMap>::reference EdgeWeight;


// AdjacencyGraph concept
std::pair<AdjacencyIterator, AdjacencyIterator>
adjacent_vertices(Vertex, ImplicitRingGraph);

// PropertyMap concept
EdgeWeightMap::reference get(EdgeWeightMap, EdgeWeightMap::key_type);

// PropertyGraph concept
EdgeWeightMap get(boost::edge_weight_t, ImplicitRingGraph&);

EdgeWeight get(boost::edge_weight_t, ImplicitRingGraph&, Edge);

