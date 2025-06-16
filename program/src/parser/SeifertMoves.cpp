#include "SeifertMoves.h"


using namespace std;



SeifertMoves::SeifertMoves(){
	moves = {
		//in negative crossing
		{{-1,0}, 3},
		{{-1,1}, 2},
		{{-1,2}, -1},
		{{-1,3}, -1},

		//in positive crossing
		{{1,0}, 1},
		{{1,1}, -1},
		{{1,2}, -1},
		{{1,3}, 2},
	};
};


//returns next position xi to move to while traversing seifert circle (-1 if we should move to next crossing) (xi \in {0,1,2,3})
//note: obsłuzyć przypadek zdegenerowany (pętla; pętle pod rząd) (dopisać później)
int SeifertMoves::give_move(int cx_sign, int position){
	array<int,2> key = {cx_sign, position};
	return moves[key];
};