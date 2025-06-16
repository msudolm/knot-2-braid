//#include <iostream>
//#include <fstream> //reading moves from file
//#include <sstream> //reading data from file lines
#include <sstream>
#include <iostream>

#include "ColorMoves.h"

using namespace std;



ColorMoves::ColorMoves(){
	moves = {};
	string color_moves_txt = ""
	"(1,1,1,0)	{1,1,1}\n"
	"(1,1,1,1)	{-1,1,1}\n"
	"(1,1,1,2)	{-1,1,1}\n"
	"(1,1,1,3)	{0,-1,-1}\n"

	"(1,1,-1,0)	{3,-1,-1}\n"
	"(1,1,-1,1)	{-1, 1, 1}\n"
	"(1,1,-1,2)	{-1, 1, 1}\n"
	"(1,1,-1,3)	{2, 1, 1}\n"


	"(1,-1,1,0)	{-1,1,1}\n"
	"(1,-1,1,1)	{0,1,1}\n"
	"(1,-1,1,2)	{1,-1,-1}\n"
	"(1,-1,1,3)	{-1,1,1}\n"

	"(1,-1,-1,0)	{-1,1,1}\n"
	"(1,-1,-1,1)	{2,-1,-1}\n"
	"(1,-1,-1,2)	{3,1,1}\n"
	"(1,-1,-1,3)	{-1,1,1}\n"

	//

	"(-1,1,1,0)	{1,-1,-1}\n"
	"(-1,1,1,1)	{2,1,1}\n"
	"(-1,1,1,2)	{-1,1,1}\n"
	"(-1,1,1,3)	{-1,1,1}\n"

	"(-1,1,-1,0)	{3,1,1}\n"
	"(-1,1,-1,1)	{0,-1,-1}\n"
	"(-1,1,-1,2)	{-1,1,1}\n"
	"(-1,1,-1,3)	{-1,1,1}\n"


	"(-1,-1,1,0)	{-1,1,1}\n"
	"(-1,-1,1,1)	{-1,1,1}\n"
	"(-1,-1,1,2)	{1,1,1}\n"
	"(-1,-1,1,3)	{2,-1,-1}\n"

	"(-1,-1,-1,0)	{-1,1,1}\n"
	"(-1,-1,-1,1)	{-1,1,1}\n"
	"(-1,-1,-1,2)	{3,-1,-1}\n"
	"(-1,-1,-1,3)	{0,1,1}\n";


	//parse moves from string to unordered_map <state: value>
	istringstream moves_iss(color_moves_txt);
    string line;
    while(getline(moves_iss, line)){

        array<int, 4> state;
        array<int, 3> value;
        char _;


    	if(!line.empty()){
    		//cout << line << endl;

    		istringstream iss(line);

    		//extracting information about state and value
            
    		iss >> _ >> state[0] >> _ >> state[1] >> _ >> state[2] >> _ >> state[3] >> _; //state = (cx_sign, direction, side, xi)
            //!note: direction changes <=> side changes --> change values from <int,3> to <int,2>
    		iss >> _ >> value[0] >> _ >> value[1] >> _ >> value[2]; //value = (new_xi, direction_change_coef, side_change_coef)


    		// //sanity check
    		// for(int i=0; i<4; i++){
    		// 	cout << "State " << i << ": " << state[i] << endl;
    		// }
    		// cout << endl;
    		// for(int i=0; i<3; i++){
    		// 	cout << "Value " << i << ": " << value[i] << endl;
    		// }
    		this->moves[state] = value;
    	}
    	
    }



	}



void ColorMoves::print_moves() const{ 

	//no (state: move) pairs in color moves map
	if(this->is_empty()){
		cout << "ColorMoves map is empty" << endl;

	}
	//color moves map not empty
	else{

		cout << "Color Moves: " << endl << endl;

		for(const auto& pair : this->moves){

			cout << "(";
			for(const auto& s : pair.first){
				cout << s << " ";
			};
			cout << ")";

			cout << "    ";
			cout << "{";
			for(const auto& v : pair.second){
				cout << v<< " ";

			};
			cout << "}";
			cout << endl;

		};
		cout << endl << endl;
	};}
void ColorMoves::print_move(const array<int,4> state) const{
	array<int,3> move = this->get_move_by_state(state);
	cout << "[ ";
	for(const int elt : move){
		cout << elt << " ";
	}
	cout << "]" << endl;
	return;
}

void ColorMoves::print_state(const array<int,4> state)const{
	cout << "[ " << state[0] << " "<< state[1] << " " << state[2] << " " << state[3] << " " << "]" << endl;
	return;
}
// void ColorMoves::read_moves_from_config_file(string infile){

// 	cout << "Reading color moves from file \"" << infile << "\"..." << endl;

// 	string line;
// 	//unordered_map<array<int,4>, array<int,3>, ArrayHash> colorMap;
// 	//unordered_map<array<int,4>, array<int,3>, ArrayHash>* p_colorMap = new unordered_map<array<int,4>, array<int,3>, ArrayHash>();

//     //open file
//     ifstream input_file(infile);
//     if (!input_file.is_open()) {
//         cout << "Failed to open file." << endl;
//         exit(1);
//     }

//     //iterate line by line
//     while(getline(input_file, line)){


//         array<int, 4> state;
//         array<int, 3> value;
//         char _;


//     	if(!line.empty()){
//     		//cout << line << endl;

//     		istringstream iss(line);

//     		//extracting information about state and value
//             //state = (cx_sign, direction, side, xi)
//     		iss >> _ >> state[0] >> _ >> state[1] >> _ >> state[2] >> _ >> state[3] >> _;
//             //value = (new_xi, direction_change_coef, side_change_coef) !note: direction changes <=> side changes --> change values from <int,3> to <int,2>
//     		iss >> _ >> value[0] >> _ >> value[1] >> _ >> value[2];


//     		// //sanity check
//     		// for(int i=0; i<4; i++){
//     		// 	cout << "State " << i << ": " << state[i] << endl;
//     		// }
//     		// cout << endl;
//     		// for(int i=0; i<3; i++){
//     		// 	cout << "Value " << i << ": " << value[i] << endl;
//     		// }


//     		//colorMap[state] = value;
//     		this->moves[state] = value;
//     	}
    	
//     }


//     input_file.close();
//     cout << "...finished." << endl;}



// int main(){

// 	cout << "Hello world from color_moves.cpp" << endl;
// 	ColorMoves cmap = ColorMoves();
// 	//cout << cmap.empty();
// 	cmap.print_moves();
// 	cmap.read_moves_from_config_file("color_traversal_moves.txt");
// 	cmap.print_moves();



// 	return 0;
// }