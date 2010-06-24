#include "implicit.hpp"


typedef ImplicitRingGraph::adjacency_iterator Iter;

std::pair<Iter, Iter>
adjacent_vertices(ImplicitRingGraph::vertex_descriptor v, ImplicitRingGraph g) {
	return std::pair<Iter, Iter>(AdjacentIterator(0, v, g),
								 AdjacentIterator(2, v, g));
}
