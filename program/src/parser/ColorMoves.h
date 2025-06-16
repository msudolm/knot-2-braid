#ifndef COLOR_MOVES_H
#define COLOR_MOVES_H

//#include <iostream>
#include <string>
#include <unordered_map>
#include <array>


using namespace std;




struct ArrayHash {
    template <typename T>
    size_t operator()(const T& array) const {
        size_t hash = 0;
        for (int i = 0; i < 4; ++i) {
            hash ^= array[i] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }};

//class storing table of moves to apply when coloring faces of the pd-diagram
class ColorMoves{
public:
	unordered_map<array<int,4>, array<int,3>, ArrayHash> moves;

	ColorMoves(); //note: define later
	//~ColorMoves(); //note:define later

    array<int,3> get_move_by_state(array<int,4> state) const {return moves.at(state);};

	bool is_empty() const{return moves.empty();};
	void print_moves() const;
    void print_move(const array<int,4> state) const;
    void print_state(const array<int,4> state)const;

};









#endif