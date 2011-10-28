/*

Copyright 2010 Alexis Wilhelm
This program is Free Software under the Elvis Presley Licence:
You can do anything, but lay off of my blue suede shoes.

*/

#include <iostream>
#include <queue>

#define DBG_PRINT(x) {\
	std::cerr << __FILE__ ":" << __LINE__ << ": " #x " = " << (x) << std::endl;\
}

/// Résout des problèmes avec une recherche exhaustive.
/// Des états successifs seront dérivés de l'état initial pour former un arbre parcouru horizontalement.
/// La première solution trouvée est retournée sous forme d'exception et affichée.
/// La structure State doit proposer les méthodes:
/// - \code void State::traverse(std::queue<State> const&) throw(State) \endcode
/// - \code void State::print(std::ostream &) throw() \endcode
template<typename State>
void solve(State const& initial_state = State(std::cin)) {
	try {
		std::queue<State> queue;
		queue.push(initial_state);
		while(!queue.empty()) {
			queue.front().traverse(queue);
			queue.pop();
		}
	} catch(State result) {
		result.print(std::cout);
	}
}
