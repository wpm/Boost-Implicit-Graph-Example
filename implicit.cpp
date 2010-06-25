#include "implicit.hpp"


// AdjacencyGraph model
std::pair<AdjacencyIterator, AdjacencyIterator>
adjacent_vertices(Vertex v, ImplicitRingGraph g) {
	return std::pair<AdjacencyIterator, AdjacencyIterator>(
		AdjacencyIterator(0, v, g),
		AdjacencyIterator(2, v, g)
	);
}


// PropertyMap model
EdgeWeightMap::reference get(EdgeWeightMap pmap, EdgeWeightMap::key_type key) {
	return pmap[key];
}


// PropertyGraph model
EdgeWeightMap get(boost::edge_weight_t, const ImplicitRingGraph& g) {
	return EdgeWeightMap();
}


EdgeWeight get(boost::edge_weight_t tag, const ImplicitRingGraph& g, Edge& e) {
	return get(tag, g)[e];
}
