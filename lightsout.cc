#include <vector>

#include "traversal.h"

/// Résout le puzzle Lights Out.
/// Avec 1 Go de RAM j'ai pu résoudre des problèmes de taille 5x5.
struct LightsoutState {
	LightsoutState(std::istream &);
	inline void traverse(std::queue<LightsoutState> &);
	void print(std::ostream &) const;
private:
	inline void assume(size_t);
	static inline void toggle(size_t);
private:
	static size_t width, height, size;
	static std::vector<bool> initial_state, current_state;
	std::vector<bool> toggled;
	unsigned char current_index;
};

/// La grille est lue dans un fichier qui commence avec la taille de la grille et qui continue avec les valeurs de toutes les cases.
/// Les lumières éteintes sont représentées par un 0 et les lumières alumées (et qu'il faut éteindre) par un 1.
LightsoutState::LightsoutState(std::istream & in):
current_index(0) {
	in >> width >> height;
	size = width * height;
	toggled.resize(size);
	initial_state.resize(size);
	for(size_t i = 0; i < size; ++i) {
		bool value;
		in >> value;
		initial_state[i] = value;
		toggled[i] = false;
	}
}

void LightsoutState::traverse(std::queue<LightsoutState> & queue) {
	for(size_t i = current_index; i < size; ++i) {
		queue.push(*this);
		queue.back().assume(i);
	}
}

void LightsoutState::assume(size_t index) {

	///- Marque la case comme ayant été modifiée.
	toggled[index] = true;

	///- À la prochaine itération on s'intéressera aux cases suivantes.
	current_index = (unsigned char)(index + 1);

	///- Change la valeur de toutes les cases concernées.
	///\note Je recalcule l'état courant à chaque itération pour économiser de la mémoire.
	current_state = initial_state;
	for(size_t i = 0; i < current_index; ++i)
		if(toggled[i]) toggle(i);

	///- Continue si au moins une case est encore active.
	for(size_t i = 0; i < size; ++i)
		if(current_state[i]) return;

	///- Si toutes les cases sont inactives, envoie le résultat.
	throw *this;
}

void LightsoutState::toggle(size_t index) {
	current_state[index] = !current_state[index];
	if(index / width > 0) current_state[index - width] = !current_state[index - width];
	if(index / width < height - 1) current_state[index + width] = !current_state[index + width];
	if(index % width > 0) current_state[index - 1] = !current_state[index - 1];
	if(index % width < width - 1) current_state[index + 1] = !current_state[index + 1];
}

void LightsoutState::print(std::ostream & out) const {
	for(size_t y = 0; y < height; ++y) {
		for(size_t x = 0; x < width; ++x)
			out << ' ' << toggled[x + y * width];
		out << std::endl;
	}
}

size_t LightsoutState::width, LightsoutState::height, LightsoutState::size;
std::vector<bool> LightsoutState::initial_state, LightsoutState::current_state;
int main(void) { solve<LightsoutState>(); }

