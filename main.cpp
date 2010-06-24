#include <iostream>
#include "implicit.hpp"
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
	using namespace boost;
	function_requires< AdjacencyGraphConcept<ImplicitRingGraph> >();

	ImplicitRingGraph g(5);

	// Print all the vertices and their neighbors.
	for(size_t i = 0; i < 5; i++) {
		std::cout << i << ": ";
		ImplicitRingGraph::adjacency_iterator ai, ai_end;
		for (tie(ai, ai_end) = adjacent_vertices(i, g); ai != ai_end; ai++) {
			std::cout << *ai << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}