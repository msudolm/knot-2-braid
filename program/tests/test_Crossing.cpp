#include <iostream>

#include "../src/building_blocks/Crossing.h"
// #include "../src/parser/PD_X.h"

using namespace std;
//g++ -std=c++17 src/parser/PD_X.cpp src/building_blocks/Crossing.cpp tests/test_Crossing.cpp

int main(){

	cout << "Hello world from 'test_Crossing'." << endl << endl;


	//############ TESTS CREATING CROSSINGS FROM PDX ####################//
	//array<int, 4> xs = {10,21,11,20};
	//PD_X pdx = PD_X(xs); 

	//##################################################################//



	// ########### TEST CREATING CROSSINGS DE NOVO ####################//
	Crossing c1 = Crossing(-1,1);
	Crossing c2 = Crossing(1,2);
	// Crossing c3 = Crossing(1,-10); //assertion error --> ok


	c1.print_crossing();
	c2.print_crossing();

	cout << "Left color of c1: " << c1.get_color(-1) << endl;
	cout << "Left color of c1: " << c1.get_color(-1,1) << endl;
	//cout << "Left color behind c1: " << c1.get_color(1,-1) << endl; //<-- to fix: dodać obsługę sytuacji, kiedy cx nie ma nikogo za soba (ew dodać podłączanie skrzyżowania do siebie(?) w konstruktorze)
	// ###############################################################




	//####### crossings for example diagram #########
//copy-pasted from old crossing.cpp
	// Crossing cx1 = Crossing(-1, "cx1", 1, 2); //blue=1, red=2, yellow=3, "purple"=4
	// Crossing cx2 = Crossing(1, "cx2", 1, 2);
	// Crossing cx3 = Crossing(-1, "cx3", 2, 3);
	// Crossing cx4 = Crossing(1, "cx4", 4,1);


	// // connecting crossings as in the diagram
	// //circle with cx1 and cx2 
	// cx1.next_crossing = &cx2;
	// cx1.prev_crossing = &cx2;
	// cx1.conjugate_crossing = &cx3;

	// cx2.next_crossing = &cx1;
	// cx2.prev_crossing = &cx1;
	// cx2.conjugate_crossing = &cx4;

	// //circle with cx3 connected with previous by cx1--cx3
	// cx3.conjugate_crossing = &cx1;//
	// cx3.prev_crossing = &cx3;
	// cx3.next_crossing = &cx3;

	// //circle with crosing cx4 conjugate to cx2 (inner for cx1;cx2 circle)
	// cx4.next_crossing = &cx4;
	// cx4.prev_crossing = &cx4;
	// cx4.conjugate_crossing = &cx2;

	// //test printing
	// cx1.print_crossing();
	// cx2.print_crossing();
	// cx3.print_crossing();
	// cx4.print_crossing();

	// // red from cx1 backward
	// cout << "\n\n-----------------------"<<endl;
	// cout << "cx2.traverse_color(1,-1)" << endl;
	// cx2.traverse_color(1,-1);
	//##########################################






	return 0;
}