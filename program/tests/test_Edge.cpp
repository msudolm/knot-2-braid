////g++ -std=c++17   ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp  ./tests/test_Edge.cpp

#include "../src/building_blocks/Edge.h"

using namespace std;




int main(){
	cout << endl << "Hello world from test_Edge.cpp" << endl << endl;

	Edge e1 = Edge(10, 10);
	e1.print_edge();

	e1 = Edge(10);
	e1.print_edge();

	Edge e2;
	e2.print_edge();


	return 0;



}