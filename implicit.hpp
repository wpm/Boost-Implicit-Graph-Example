#include <boost/graph/properties.hpp>
#include <utility> // For std::pair
#include <boost/operators.hpp> // For forward_iterator_helper


struct AdjacencyIterator;


struct ImplicitRingGraph {
	ImplicitRingGraph(size_t n):n(n) {};

	// Graph concept
	typedef size_t vertex_descriptor;
	typedef boost::undirected_tag directed_category;
	typedef boost::disallow_parallel_edge_tag edge_parallel_category;
	typedef boost::adjacency_graph_tag traversal_category;

	typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
	typedef size_t vertices_size_type;
	typedef size_t edges_size_type;
	typedef size_t degree_size_type;
	
	typedef AdjacencyIterator adjacency_iterator;
	typedef void out_edge_iterator;
	typedef void in_edge_iterator;
	typedef void vertex_iterator;
	typedef void edge_iterator;

	size_t n;
};


struct AdjacencyIterator:public boost::forward_iterator_helper <
		AdjacencyIterator,
		ImplicitRingGraph::vertex_descriptor,
		std::ptrdiff_t,
		ImplicitRingGraph::vertex_descriptor*,
		ImplicitRingGraph::vertex_descriptor> {
	AdjacencyIterator() {}
	AdjacencyIterator(size_t i,
		ImplicitRingGraph::vertex_descriptor v,
		ImplicitRingGraph& g):i(i),v(v),n(g.n) {}
	
	ImplicitRingGraph::vertex_descriptor operator*() const {
		static const int ring_offset[] = {-1, 1};
		if (i == 0 && v == 0)
			return n-1; // Wrap around to the largest vertex
		else
			return (v+ring_offset[i]) % n;
	}
	void operator++() {i++;}
	bool operator==(const AdjacencyIterator& other) const {
		return i == other.i;
	}
	
	size_t i; // Index into ring_offset, ranges over {0,1}
	ImplicitRingGraph::vertex_descriptor v;
	size_t n; // The size of the graph
};


std::pair<ImplicitRingGraph::adjacency_iterator, ImplicitRingGraph::adjacency_iterator>
adjacent_vertices(ImplicitRingGraph::vertex_descriptor v, ImplicitRingGraph g);
