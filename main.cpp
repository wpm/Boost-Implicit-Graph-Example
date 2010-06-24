#include <iostream>
#include "implicit.hpp"
#include <boost/graph/graph_concepts.hpp>


int main (int argc, char const *argv[]) {
	using namespace boost;
	function_requires< AdjacencyGraphConcept<ImplicitRingGraph> >();
	function_requires< ReadablePropertyMapConcept<EdgeWeightMap, Edge> >();
	// function_requires< 
	// 	PropertyGraphConcept<ImplicitRingGraph,
	// 						 Edge,
	// 						 edge_weight_t>
	// >();

	ImplicitRingGraph g(5);

	// Print all the vertices and their neighbors.
	for(size_t i = 0; i < 5; i++) {
		std::cout << i << ": ";
		AdjacencyIterator ai, ai_end;
		for (tie(ai, ai_end) = adjacent_vertices(i, g); ai != ai_end; ai++) {
			std::cout << *ai << " ";
		}
		std::cout << std::endl;
	}

	EdgeWeightMap m = get(boost::edge_weight, g);
	boost::graph_traits<ImplicitRingGraph>::edge_descriptor e(0, 1);
	EdgeWeight w = get(boost::edge_weight, g, e);

	return 0;
}