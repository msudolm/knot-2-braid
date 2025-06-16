
#include "../src/building_blocks/Graph.h"

using namespace std;

////g++ -std=c++17 ./src/parser/color_moves.cpp  ./src/parser/PD_X.cpp ./src/parser/Diagram.cpp ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp ./src/building_blocks/Color.cpp ./src/building_blocks/CrossingsCollection.cpp ./src/building_blocks/EdgesCollection.cpp ./src/building_blocks/ColorsCollection.cpp ./src/building_blocks/Graph.cpp ./tests/test_Graph.cpp



int main(){
	cout << endl << "Hello world from test_Graph.cpp" << endl << endl;


	ColorMoves cmap = ColorMoves();
	cmap.read_moves_from_config_file("./src/parser/color_traversal_moves.txt");
	//cmap.print_moves();
	
	string pd_infile = "./pd_data/pds_small_test/k_5.txt"; //(works; only todo-done size checked for correctness for now)
	//string pd_infile = "./pd_data/pds_small_test/k_4.txt";
	//string pd_infile = "./pd_data/pds_test/k_40.txt";
	//string pd_infile = "./pd_data/pds_test/k_50.txt";
	//string pd_infile = "./pd_data/pds_test/k_60.txt";
	//string pd_infile = "./pd_data/pds_test/k_70.txt";
	//string pd_infile = "./pd_data/pds_test/k_80.txt";
	//string pd_infile = "./pd_data/pds_test/k_90.txt";
	//string pd_infile = "./pd_data/pds_test/k_100.txt";
	//string pd_infile = "./pd_data/pds_test/k_110.txt";
	//string pd_infile = "./pd_data/pds_test/k_120.txt";
	//string pd_infile = "./pd_data/pds_test/k_130.txt";

	Diagram d = Diagram(pd_infile);

	d.color_diagram(cmap);
	d.print_diagram(); cout << endl <<endl;

	Graph g = Graph(d);


	//g.graph_edges.print_edges_collection();
	g.graph_crossings.print_collection(); cout << endl;
	g.graph_colors.print_colors_collection(); cout << endl;

	g.print_graph();


	// ################     PRINTING COLORS COLLECTION     #####################################
	// cout << endl << endl << endl;
	// cout << "--- PRINTING GRAPH COLORS COLLECTION: ---" << endl << endl; 
	// (g.graph_colors).print_colors_collection();
	// cout << endl<< "--- PRINTING GRAPH COLORS COLLECTION FINISHED. ---" << endl << endl; 
	// #########################################################################################



	// cout << "TEST: exiting" << endl;
	// exit(1);

/* //FRAGMENT FOR TESTING SINGLE MOVE BEFORE THE GRAPH METHOD WAS WRITTEN FOR THIS
	array<int,3> c1_c2_side = g.find_admissible_segments_starts_and_side();
	int c1 = c1_c2_side[0];
	int c2 = c1_c2_side[1];
	int mv_side = c1_c2_side[2];
	cout << endl << "Found admissible crossings: " << c1 << " and " << c2 << endl;
	cout << "On edges: " << (g.get_crossing_ptr(c1))->edge_id << " and " << (g.get_crossing_ptr(c2))->edge_id<< endl;
	cout << "Move color is: " << (g.get_crossing_ptr(c2))->get_color(mv_side) << endl;
	cout << "Mv_side is: " << mv_side << endl << endl;

	cout << "Performing move..." << endl;
	g.join_add_and_recolor_edges(c1, c2, mv_side);
	cout << "...done." << endl << endl;
*/

/*
	cout << endl << "Looking for traversal bug:" << endl;


	cout << "Printing c7:" << endl;
	Crossing* c7 = g.graph_crossings.get_ptr_by_key(7);
	c7->print_crossing(); cout << endl;
	cout << "Printing c7->conjugate_crossing" << endl;
	(c7->conjugate_crossing)->print_crossing();	cout << endl << endl; 

	cout << "Printing c4 and c4->prev:" << endl;
	Crossing* c4 = g.graph_crossings.get_ptr_by_key(4);
	Crossing* c4_prev = c4->prev_crossing;
	c4->print_crossing(); cout << endl;
	c4_prev->print_crossing(); cout << endl << endl;

	cout << "printing c4->get_color(1,-1):" << c4->get_color(1,-1) << endl;

	cout << "printing c7.check_if_conjugate_continues_traversal_color(int current_direction1, int current_side, int color)" << endl;
	cout << "d=1, s=-1, color=2" << endl;
	cout << c7->check_if_conjugate_continues_traversal_color(1,-1,2); cout << endl;

	// cout << "printing c7.get_conjugate_traversal_color(int current_side: -1, int current_direction: 1)" << endl;
	// cout << c7->get_conjugate_traversal_color(-1,1); cout << endl;

	cout << "int conj_color = (c7->conjugate_crossing)->get_color(-current_side: -(-1), -current_direction: -(1));" << endl;
	cout <<  (c7->conjugate_crossing)->get_color(1, -1) << endl;

	cout << "int conj_color = (c7->conjugate_crossing)->get_color(-current_side: -(1), -current_direction: -(1));" << endl;
	cout <<  (c7->conjugate_crossing)->get_color(-1, -1) << endl;
*/

	//g.perform_move();
	//g.graph_colors.print_colors_collection();

	g.transform_to_braid_form();

	//g.graph_crossings.print_collection();
	g.graph_colors.print_colors_collection();
	g.print_graph();

	vector<int> braid_word = g.generate_braid_word();

	cout << "Printing braid word: " << endl;
	for(const auto& elt : braid_word){
		cout << elt << ", ";
	}

	// cout << "TEST: exiting" << endl;
	// exit(1);

	return 0;
}