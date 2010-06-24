#include "implicit.hpp"


typedef ImplicitRingGraph::adjacency_iterator Iter;

std::pair<Iter, Iter>
adjacent_vertices(ImplicitRingGraph::vertex_descriptor v, ImplicitRingGraph g) {
	return std::pair<Iter, Iter>(AdjacencyIterator(0, v, g),
								 AdjacencyIterator(2, v, g));
}
