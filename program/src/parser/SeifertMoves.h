#ifndef SEIFERT_MOVES_H
#define SEIFERT_MOVES_H

#include <array>
#include <unordered_map>

using namespace std;


struct ArrayHash2 {
    template <typename T>
    size_t operator()(const T& array) const {
        size_t hash = 0;
        for (int i = 0; i < 2; ++i) {
            hash ^= array[i] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }};


class SeifertMoves{

	unordered_map<array<int,2>, int, ArrayHash2> moves; //map (sign, xi) --> next_xi

public:
	SeifertMoves();
	
	//returns next position xi to move to while traversing seifert circle (-1 if we should move to next crossing) (xi \in {0,1,2,3})
	//note: obsłuzyć przypadek zdegenerowany (pętla; pętle pod rząd) (dopisać później)
	int give_move(int cx_sign, int position);

};


#endif