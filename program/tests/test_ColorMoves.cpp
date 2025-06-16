#include <iostream>
#include "../src/parser/ColorMoves.h"

using namespace std;

int main(){
	cout << "Hello world from test_ColorMoves.cpp" << endl;

	ColorMoves color_moves = ColorMoves();
	cout << "created color moves array." << endl;
	color_moves.print_moves();
	

	return 0;
}