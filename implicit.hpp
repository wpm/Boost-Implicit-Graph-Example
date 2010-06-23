#include <boost/graph/properties.hpp>
#include <utility>


struct ImplicitRingGraph {
	ImplicitRingGraph(size_t n):n(n) {};

	// Graph concept
	typedef size_t vertex_descriptor;
	typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
	typedef boost::directed_tag directed_category;
	typedef boost::disallow_parallel_edge_tag edge_parallel_category;
	typedef boost::adjacency_graph_tag traversal_category;

private:
	size_t n;
};