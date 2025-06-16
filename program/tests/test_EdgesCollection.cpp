////g++ -std=c++17   ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp ./src/building_blocks/Edgescollection.cpp ./tests/test_EdgesCollection.cpp

#include "../src/building_blocks/EdgesCollection.h"

using namespace std;




int main(){
	cout << endl << "Hello world from test_Edges Collection.cpp" << endl << endl;

	EdgesCollection e_collection;
	// e_collection.print_edges_collection();
	Edge e1 = Edge();

	for(int i=0; i<10; i++){ e1 = *(e_collection.add_new_return_ptr()); }
	e1 = *(e_collection.add_new_return_ptr());


	e_collection.print_edges_collection();


	return 0;



}