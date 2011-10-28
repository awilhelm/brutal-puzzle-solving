#include <cmath>
#include <iomanip>
#include <set>
#include <sstream>
#include <vector>

#include "traversal.h"

/// Résout un sudoku.
/// J'ai pu résoudre des grilles de niveau « très difficile » en un dixième de seconde !
struct SudokuState {
	SudokuState(std::istream &);
	inline void traverse(std::queue<SudokuState> &);
	void print(std::ostream &);
private:
	typedef std::set<unsigned char> Cell;
	typedef std::vector<Cell> Grid;
	typedef Cell & Index(size_t, size_t);
private:
	inline void simplify(std::vector<bool> &, Index const SudokuState::*);
	inline void assume(size_t, Cell::value_type);
	inline Index get_by_row, get_by_column, get_by_square;
private:
	static size_t dimension, size, square_size;
	static std::vector<bool> rows, columns, squares;
	Grid grid;
};

/// La grille est lue dans un fichier qui commence avec la largeur de la grille (9 en général) et qui continue avec les valeurs de toutes les cases.
/// Pour les cases dont la valeur est connue, cette valeur doit être renseignée dans une bonne base numérique (telle que 0 ≤ valeur < largeur).
/// Pour les cases dont la valeur n'est pas connue, on peut utiliser n'importe quels caractères non numériques (par exemple X, ? ou _).
SudokuState::SudokuState(std::istream & in) {

	///- Calcule les dimensions de la grille.
	in >> dimension;
	size = dimension * dimension;
	square_size = (size_t) std::sqrt(dimension);

	///- Marque toutes les lignes/colonnes/carrés comme devant être vérifiés.
	rows.resize(dimension);
	columns.resize(dimension);
	squares.resize(dimension);
	for(size_t i = 0; i < dimension; ++i)
		rows[i] = columns[i] = squares[i] = true;

	///- Lit la grille depuis le fichier (les cases sans valeur sont initialisées avec toutes les valeurs possibles).
	grid.resize(size);
	for(Grid::iterator cell = grid.begin(); cell != grid.end(); ++cell) {
		std::string str_value; in >> str_value;
		std::istringstream str_in(str_value);
		long int_value; str_in >> int_value;
		if(!str_in.fail()) cell->insert((unsigned char) int_value);
		else for(unsigned char i = 0; i < dimension; ++i) cell->insert(i);
	}
}

void SudokuState::traverse(std::queue<SudokuState> & queue) {

	///- Élimine les hypothèses farfelues en parcourrant les lignes/colonnes/carrés.
	for(size_t i = 0; i < dimension; ++i) {
		simplify(rows, &SudokuState::get_by_row);
		simplify(columns, &SudokuState::get_by_column);
		simplify(squares, &SudokuState::get_by_square);
	}

	///- Vérifie que cette grille est encore valide.
	for(Grid::const_iterator cell = grid.begin(); cell != grid.end(); ++cell)
		if(cell->empty()) return;

	///- Émet des hypothèses sur les valeurs possibles des cases encore indéterminées.
	bool done = true;
	for(size_t i = 0; i < size; ++i)
		if(grid[i].size() > 1) {
			done = false;
			for(Cell::iterator p = grid[i].begin(); p != grid[i].end(); ++p) {
				queue.push(*this);
				queue.back().assume(i, *p);
			}
		}

	if(done) throw *this;
}

void SudokuState::simplify(std::vector<bool> & changed, Index SudokuState::* get) {
	for(size_t i = 0; i < dimension; ++i) if(changed[i]) {
		changed[i] = false;
		Cell given;

		///- Établit la liste des valeurs dont on sait qu'elles sont déjà utilisées dans cette ligne/colonne/carré.
		for(size_t j = 0; j < dimension; ++j) {
			Cell & x = (this->*get)(i, j);
			if(x.size() == 1) given.insert(x.begin(), x.end());
		}

		///- Retire ces valeurs de la liste des hypothèses pour chaque case encore indéterminée de cette ligne/colonne/carré.
		for(size_t j = 0; j < dimension; ++j) {
			Cell & x = (this->*get)(i, j);
			if(x.size() > 1) for(Cell::const_iterator p = given.begin(); p != given.end(); ++p) {
				x.erase(*p);
				changed[i] = true;
			}
		}
	}
}

void SudokuState::assume(size_t i, Cell::value_type x) {
	grid[i].clear();
	grid[i].insert(x);
}

SudokuState::Cell & SudokuState::get_by_row(size_t i, size_t j) {
	return grid[i * dimension + j];
}

SudokuState::Cell & SudokuState::get_by_column(size_t i, size_t j) {
	return grid[i + j * dimension];
}

SudokuState::Cell & SudokuState::get_by_square(size_t i, size_t j) {
	return get_by_row(
		(i / square_size) * square_size + (j / square_size),
		(i % square_size) * square_size + (j % square_size));
}

void SudokuState::print(std::ostream & out) {
	int width = 2 + (int) std::log10(dimension);
	for(size_t i = 0; i < dimension; ++i) {
		for(size_t j = 0; j < dimension; ++j)
			out << std::setw(width) << (long) *get_by_row(i, j).begin();
		out << std::endl;
	}
}

size_t SudokuState::dimension, SudokuState::size, SudokuState::square_size;
std::vector<bool> SudokuState::rows, SudokuState::columns, SudokuState::squares;
int main(void) { solve<SudokuState>(); }

