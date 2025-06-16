#include "./building_blocks/Graph.h"
#include <fstream>

using namespace std;



int main(int argc, char* argv[]){


	// Check if exactly one additional argument (pd-code file) was passed
	if(argc != 2){
		cout << "Usage: " << argv[0] << "<pd-code-filename>" << endl;
		return 1;
	}


	//Get the name of the PD-code file
	const char* filename = argv[1];


	// fstream pd_infile(filename);
	// if( !pd_infile.is_open()){
	// 	cout << "Error openig file: " << filename << endl;
	// 	return 1;
	// }


	// Read the diagram from PD-file, build graph from diagram
	Diagram d = Diagram(filename);
	d.color_diagram(); //d.print(); cout << endl <<endl; //print diagram
	Graph g = Graph(d);

	//Sanity checks - print graph components
	//g.graph_edges.print_edges_collection();
	//g.graph_crossings.print_collection(); cout << endl;
	//g.graph_colors.print_colors_collection(); cout << endl;
	//g.print_graph();


	// For testing: perform single move
	//g.perform_move();
	//g.graph_colors.print_colors_collection();

	g.transform_to_braid_form();

	// Sanity checks: print some of the graph components
	//g.graph_crossings.print_collection();
	//g.graph_colors.print_colors_collection();
	//g.print_graph();


	vector<int> braid_word = g.read_braid_word();

	cout << "Printing braid word: " << endl;
	for(const auto& elt : braid_word){
		cout << elt << ", ";
	}

	return 0;
}