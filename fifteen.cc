#include <sstream>
#include <vector>

#include "traversal.h"

/// Résout le jeu du Taquin.
struct FifteenState {
	FifteenState(std::istream &);
	inline void traverse(std::queue<FifteenState> &);
	void print(std::ostream &) const;
private:
	enum Direction { UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2 };
private:
	inline void assume(Direction);
	inline void move(Direction);
	static inline long & get(size_t, size_t);
private:
	static size_t width, height, initial_x, initial_y, x, y;
	static std::vector<long> initial_state, current_state;
	std::vector<char> path;
};

/// La grille est lue dans un fichier qui commence avec la taille de la grille et la valeur de la case vide, et qui continue avec les valeurs de toutes les cases.
/// La case vide est représentée par un caractère non numérique (par exemple « _ »).
FifteenState::FifteenState(std::istream & in) {
	long empty;
	in >> width >> height >> empty;
	initial_state.resize(width * height);
	for(size_t i = 0; i < initial_state.size(); ++i) {
		std::string str_value;
		in >> str_value;
		std::istringstream str_in(str_value);
		str_in >> initial_state[i];
		if(str_in.fail()) {
			initial_state[i] = empty;
			initial_x = i % width;
			initial_y = i / width;
		}
	}
}

void FifteenState::traverse(std::queue<FifteenState> & queue) {

	///- Recalcule l'état courant à chaque itération pour économiser la mémoire.
	x = initial_x;
	y = initial_y;
	current_state = initial_state;
	for(size_t i = 0; i < path.size(); ++i)
		move((Direction) path[i]);

	///- Essaie toutes les configurations possibles.
	for(long i = -2; i <= 2; ++i) {
		if(!i
			|| (!path.empty() && i == -path.back())
			|| (i == UP && y <= 0)
			|| (i == DOWN && y >= height - 1)
			|| (i == RIGHT && x >= width - 1)
			|| (i == LEFT && x <= 0)
			) continue;
		queue.push(*this);
		queue.back().assume((Direction) i);
	}
}

void FifteenState::assume(Direction direction) {

	///- Met à jour le chemin parcouru.
	path.push_back(direction);
	move(direction);

	///- Si la grille n'est pas triée, essaie la configuration suivante.
	for(size_t i = 1; i < current_state.size(); ++i)
		if(current_state[i - 1] > current_state[i]) {
			move((Direction) -direction);
			return;
		}

	///- Si la grille est triée, envoie le résultat.
	throw *this;
}

void FifteenState::move(Direction direction) {
	switch(direction) {
		case UP:
			std::swap(get(x, y), get(x, y - 1));
			--y;
			break;
		case DOWN:
			std::swap(get(x, y), get(x, y + 1));
			++y;
			break;
		case RIGHT:
			std::swap(get(x, y), get(x + 1, y));
			++x;
			break;
		case LEFT:
			std::swap(get(x, y), get(x - 1, y));
			--x;
			break;
	}
}

long & FifteenState::get(size_t x, size_t y) {
	return current_state[x + y * width];
}

void FifteenState::print(std::ostream & out) const {
	for(std::vector<char>::const_iterator p = path.begin(); p != path.end(); ++p)
		switch((Direction) *p) {
			case UP: out << "↓"; break;
			case DOWN: out << "↑"; break;
			case RIGHT: out << "←"; break;
			case LEFT: out << "→"; break;
		}
	out << std::endl;
}

size_t FifteenState::width, FifteenState::height, FifteenState::initial_x, FifteenState::initial_y, FifteenState::x, FifteenState::y;
std::vector<long> FifteenState::initial_state, FifteenState::current_state;
int main(void) { solve<FifteenState>(); }

