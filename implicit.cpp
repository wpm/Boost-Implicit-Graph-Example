#include "implicit.hpp"


// AdjacencyGraph concept
std::pair<AdjacencyIterator, AdjacencyIterator>
adjacent_vertices(Vertex v, ImplicitRingGraph g) {
	return std::pair<AdjacencyIterator, AdjacencyIterator>(
		AdjacencyIterator(0, v, g),
		AdjacencyIterator(2, v, g)
	);
}


// PropertyMap concept
EdgeWeightMap::reference get(EdgeWeightMap pmap, EdgeWeightMap::key_type key) {
	return pmap[key];
}


// PropertyGraph concept
EdgeWeightMap get(boost::edge_weight_t, ImplicitRingGraph& g) {
	return EdgeWeightMap();
}


EdgeWeight get(boost::edge_weight_t tag, ImplicitRingGraph& g, Edge e) {
	return get(tag, g)[e];
}

