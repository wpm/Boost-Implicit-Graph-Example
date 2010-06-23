#include <boost/graph/graph_concepts.hpp>
#include "implicit.hpp"


int main (int argc, char const *argv[]) {
	boost::function_requires< boost::GraphConcept<ImplicitRingGraph> >();

	ImplicitRingGraph g(3);
	return 0;
}