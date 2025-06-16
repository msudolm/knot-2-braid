#include "../src/parser/Diagram.h"
#include <fstream>

using namespace std;

//g++ -std=c++17 ./src/parser/ColorMoves.cpp ./src/parser/PD_X.cpp ./src/parser/Diagram.cpp ./tests/test_Diagram.cpp 


int main(int argc, char* argv[]){
	cout << endl << "Hello world from test_Diagram.cpp" << endl << endl;

	//##################################################################
	//#####  checking command line attributes (pd-code file) ###########
	//check if exactly one additional argument (pd-code file) was passed
	if(argc != 2){
		cout << "Usage: " << argv[0] << "<pd-code-filename>" << endl;
		return 1; 
	}
	//##################################################################


	//get pd-code filename
	const char* filename = argv[1];
	Diagram d = Diagram(filename);
	

	cout << "Is any of the side todo lists not-empty? " << d.exists_nonempty_todo_sidelist() << endl;
	cout << "Which of the side todo lists is not empty for sure? " << d.give_nonempty_todo_side() << endl << endl;


	d.color_diagram();


	cout << endl << endl << "Is any of the side todo lists not-empty? " << d.exists_nonempty_todo_sidelist() << endl;
	cout << "Which of the side todo lists is not empty for sure? " << d.give_nonempty_todo_side() << endl << endl;

	// int pdx_id;
	// int s = -1;
	// for(int i=0; i<5;i++){
	// 	pdx_id = d.pop_pdxid_from_sidelist(s);
	// 	cout << "Popped id: " << pdx_id << " from side " << s << endl;
	// 	s = -s;
	// }



	return 0;
}