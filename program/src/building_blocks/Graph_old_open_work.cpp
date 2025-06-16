#include "Graph.h"

#include <unordered_set>
#include <iostream>

using namespace std;

//g++ -std=c++17 ./src/parser/PD_X.cpp ./src/parser/Diagram.cpp ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp ./src/building_blocks/Color.cpp ./src/building_blocks/CrossingsCollection.cpp ./src/building_blocks/EdgesCollection.cpp ./src/building_blocks/ColorsCollection.cpp ./src/building_blocks/Graph.cpp


Graph::Graph(Diagram &colored_diagram){

	this->graph_colors = ColorsCollection(); //note: dodać wszelkie rzeczy związane z kolorami
	this->graph_crossings = CrossingsCollection();
	this->graph_edges = EdgesCollection();

	//two collections with indices of pd_crossings to-convert to Crossing objects
	unordered_set<int> &pdx_ids_right = colored_diagram.seifert_unassigned_pdxs_right;
	unordered_set<int> &pdx_ids_left = colored_diagram.seifert_unassigned_pdxs_left;




	//CREATE NEW EDGES UNTILL YOU USE ALL PD_X's TWICE -- each loop run creates a new Edge (=Seifert circle)
	while( colored_diagram.exists_nonempty_todo_sidelist() ){

		cout << "------------------- NEW EDGE ------------------------------------------------------" << endl;
		//ADD NEW EMPTY EDGE to graph (corresponds to Seifert circle)
		int edge_id = (this->graph_edges).add_empty_edge();
		Edge &new_edge = (this->graph_edges).edges.at(edge_id);


		//PICK START PD_X and its side
		//note: sprawdzić czy w ogóle tak może być, że jeszcze zostało coś i przy tym jedna z list jest pusta (może zdegenerowany przypadek tu też grać rolę)
		int start_pdx_side = colored_diagram.give_nonempty_todo_side();
		int start_pdx_id = colored_diagram.give_pdxid_from_sidelist(start_pdx_side); //note: może do tego w Diagram zrobić jedną funkcje od razu pobierającą id z niepustej strony i zwracającą stronę i id (to da chyba porównań w Diagram)
		PD_X &start_pdx = (colored_diagram.pd_crossings).at(start_pdx_id); //note: czemu tu "error: no matching constructor for initialization of 'PD_X'" kiedy nie ma konstruktora bezargumentowego w PD_X?
		cout << "Start PD_X id is: " << start_pdx_id << endl << endl;


		//CREATE NEW CROSSING object from appropriate side of picked PD_X
		Crossing *start_cx_ptr = new Crossing(start_pdx, start_pdx_side);
		int start_cx_id = (this->graph_crossings).add_element_return_id(start_cx_ptr); //note: czy można to new Crossing od razu do tej funkcji?
		start_pdx.manage_conjugations(start_cx_ptr); 


		//ANCHOR NEW EDGE ON FIRST CROSSING
		new_edge.set_anchor(start_cx_id);


		//FIND NEXT PD_X for START_PD_X and SIDE
		//note: tu uprościć te wszystke zmienne, żeby to lepiej działało i sie dobrze czytało
		array<int,2> curr_position;
		PD_X* curr_pdx_ptr = nullptr;
		int curr_pdx_id = -1;
		int curr_pdx_side = 0;
		Crossing* new_cx_ptr = start_cx_ptr;
		Crossing* prev_cx_ptr = nullptr; //note: tego nie trzeba będzie jak anchor będzie się przesuwał z każdym nowo dodanym cx do krewędzi

		curr_position = colored_diagram.pop_next_pdx_id_with_side(start_pdx_id, start_pdx_side); //NOTE: INEFFICIENT IMPLEMENTATION OF FIND_NEXT...
			
		curr_pdx_id = curr_position[0];
		curr_pdx_ptr = &(colored_diagram.pd_crossings.at(curr_pdx_id));
		curr_pdx_side = curr_position[1];
		cout << endl << "Now pdx_id is: " << curr_position[0] <<endl;


		//GO THROUGH FOLLOWING PD_Xs on started seifert circle
		while(curr_pdx_id != start_pdx_id){
			//NOTE: SPRAWDZIĆ CZY JEDNO SKRZYŻOWANIE NIE MOŻE BYĆ Z DWÓCH STRON W TYM SAMYM OKRĘGU (NIE MOŻE)
			cout << "\tentered while loop  (curr_pdx_id = " << curr_pdx_id << ", start_pdx_id = " << start_pdx_id << ")" << endl;


			//.
			//.
			//creating naew Crossing and linking with previous (and managing its conjugation)
			prev_cx_ptr = new_cx_ptr;
			new_cx_ptr = (this->graph_crossings).add_new_crossing_return_ptr(*curr_pdx_ptr, curr_pdx_side);
			(*curr_pdx_ptr).manage_conjugations(new_cx_ptr);
			prev_cx_ptr->next_crossing = new_cx_ptr;
			new_cx_ptr->prev_crossing = prev_cx_ptr;

			//saving color information about the edge-crossing in ColorsCollection
			(this->graph_colors).add_cx_info_to_collection(new_cx_ptr, edge_id);
			new_edge.add_colors_ids_from_crossing(*new_cx_ptr);


			curr_position = colored_diagram.pop_next_pdx_id_with_side(curr_pdx_id, curr_pdx_side); //NOTE: INEFFICIENT IMPLEMENTATION OF FIND_NEXT...
			
			curr_pdx_id = curr_position[0];
			curr_pdx_ptr = &(colored_diagram.pd_crossings.at(curr_pdx_id));
			curr_pdx_side = curr_position[1];
			cout << endl << "Now pdx_id is: " << curr_position[0] <<endl;



			//.
			//create new crossing from appropriate side of this PD_X
			//create links with previous Crossing
			//manage conjugating
			//delete pd_x from side-todolist
			//get next pd_x 
			//.
			//.
			//.
			//.
			cout << "\tfinished single run of while loop  (curr_pdx_id = " << curr_pdx_id << ", start_pdx_id = " << start_pdx_id << ")" << endl << endl;

		}
		//get things done when returned to start crossing (links etc)
		//finish current seifert (new_cx_ptr points to one-but-last Crossing; curr_position holds pdx_id for START pdx)
		cout << endl << "Returned to START pdx: " << curr_position[0] <<endl;	

		//CONNECT LAST CROSSING WITH FIRST AND FINISH EDGE
		cout << "Finishing edge..." << endl;
		prev_cx_ptr = new_cx_ptr;
		prev_cx_ptr->next_crossing = start_cx_ptr;
		start_cx_ptr->prev_crossing = prev_cx_ptr;
		cout << "...edge finished." << endl << endl;

		//print edge
		cout << "Printing new edge: " << endl << endl;
		this->print_edge(edge_id); //note: not implemented
		cout << endl << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
			




		// //#########################################################
		// cout << "--- BREAK CODE HERE (TESTING)----" << endl << endl;
		// exit(1);



	};



	cout << "Creating Edges finished." << endl;
	cout << "Number of edges: " << (this->graph_edges).edges.size() << endl;
	cout << "End of constructor call for Graph object." << endl;

};




void Graph::print_edge(int edge_id){
	Edge &e = graph_edges.edges.at(edge_id);
	Crossing* start_cx_ptr = graph_crossings.crossings.at((e.anchor_crossing_id));
	Crossing* cx_ptr = start_cx_ptr->next_crossing;

	start_cx_ptr->print_crossing();
	while( cx_ptr != start_cx_ptr){
		cx_ptr->print_crossing();
		cx_ptr = cx_ptr->next_crossing;

	}};





void Graph::perform_move(){
	//find 2 edges and pair of crossings on which we can perform the move:
		//chose an arbitrary color on todo list of colors
		//arbitrary chose a side of this color (but has to contain at least two seiferts (=edges) )
		//arbitrary chose one crossing from each of edges chosen in a previous step

	//perform a move, managing all necessary information about colors


	if(graph_colors.colors_todo.size() == 0){
		cout << "Already in closed braid form." << endl;
		cout << "No more moves to perform" << endl;
		return;
	}


	//FIND COLOR, EDGES AND CROSSINGS TO PERFORM MOVE
	//Color &mv_color = graph_colors.give_todo_color(); //note: napisać
	array<int,4> mv_col_side_e1_e2 = graph_colors.give_todo_color_side_and_edges();
	array<Crossing*,2> mv_crossings = graph_colors.give_todo_crossing_ptrs(mv_col_side_e1_e2[0], mv_col_side_e1_e2[1], mv_col_side_e1_e2[2], mv_col_side_e1_e2[3]); //(mv_color, mv_side, mv_edge_1, mv_edge_2)

	int mv_col = mv_col_side_e1_e2[0];
	int mv_side = mv_col_side_e1_e2[1];
	int mv_e1_id = mv_col_side_e1_e2[2];
	int mv_e2_id = mv_col_side_e1_e2[3];


	//SUBSTITUTE E2 WITH E1 IN ALL COLORS AND SIDES
	this->absorb_e2_by_e1_in_colors_collection(mv_e1_id , mv_e2_id);

	

	//PERFORM JOINING AND RECOLORING EDGES (AND ADDING NEW EDGE)
	this->join_edges(mv_col, mv_side, mv_e1_id, mv_e2_id);
	





}





//auxiliary functions

//note: tu może jakoś poprawić sumowanie tych zbiorów skrzyżowań (?) (za każdym razem kiedy łączenie, jest liniowo od liczby skrzyżowań w e2)
void Graph::absorb_e2_by_e1_in_colors_collection(int edge1_id, int edge2_id){

	//Edge objects to be combined (variables created for easy atrribute access)
	Edge e1, e2;
	e1 = graph_edges.edges.at(edge1_id);
	e2 = graph_edges.edges.at(edge2_id);


	//left colors //(note: później połączyć te pętle, żeby nie pisać 2x tego samego)
	int s = -1; //side
	for(auto col_it : e2.colors_left){
		//MOVE A SET OF CROSSINGS THAT WAS UNDER E2 TO E1 (sum with a set under e1 key)
		col_it->absorb_e2_by_e1(edge1_id, edge2_id, s);
		if( !( col_it->is_edge_in_color(e1_id, s) )){e1.colors_left.insert(*col_it)}; //save color to Edge colors for Edge e1
	}

	//right colors update //(note: później połączyć te pętle, żeby nie pisać 2x tego samego)
	s = 1;
	for(auto col_it : e2.colors_right){
		col_it->absorb_e2_by_e1(edge1_id, edge2_id, s);
		if( !( col_it->is_edge_in_color(e1_id, s) )){e1.colors_right.insert(*col_it)}; //save color to Edge colors for Edge e1
	}
}

// //! maybe manage assigning crossings to eges 
// /* Function description:
// Based on pointers to start Crossings of both admissible segments,
// this function:
// 	- creates four new Crossings (x1, y1, x2, y2),
// 	- links them with appropriate previous and next crossings
// 	- assigns them their conjugates (_1 conjugated with _2, for _ in {x,y})
// 	- returns an array of pointers to newly created crossings

// NOTE: Crossings created here don't have any colors (and edges? assigned -- this is done in the next steps)
// */
// array<Crossing*, 4> Graph::create_new_crossings_linked_and_conjugated(Crossing* seg1_start, Crossing* seg2_start){

// 	//get ends of segments
// 	Crossing* seg1_end = seg1_start->next_crossing;
// 	Crossing* seg2_end = seg2_start->next_crossing;

//  	//CREATE NEW CROSSINGS
// 	//x2 is conjugate to x1, y2 to y1;     x2, y2 are on new edge;       x1 is always a name of new crossing 'next' for seg1_start

// 	int x_sign = 1; //maybe randomly sampled from {-1, 1} in later versions
// 	Crossing* x1 = new Crossing(x_sign);
// 	Crossing* x2 = new Crossing(x_sign); //conjugate to x1
// 	Crossing* y1 = new Crossing(-x_sign);
// 	Crossing* y2 = new Crossing(-x_sign);

// 	//setting names for new crossings (version for testing)
// 	x1->name = "x1";
// 	x2->name = "x2";
// 	y1->name = "y1";
// 	y2->name = "y2";

// 	//SET CONJUGATES
// 	x1->conjugate_crossing = x2;
// 	x2->conjugate_crossing = x1;
// 	y1->conjugate_crossing = y2;
// 	y2->conjugate_crossing = y1;


// 	//PUSH NEW CROSSINGS TO E1
// 	//insert new crossing x to edge1 (and link the endpoint of edge2 with x)
// 	x1->prev_crossing = seg1_start;
// 	x1->next_crossing = seg2_end;
// 	seg1_start->next_crossing = x1;
// 	seg2_end->prev_crossing = x1;

// 	//insert new crossing y to edge1 (and link the endpoint of edge2 with y)
// 	y1->prev_crossing = seg2_start;
// 	y1->next_crossing = seg1_end;
// 	seg2_start->next_crossing = y1;
// 	seg1_end->prev_crossing = y1;


// 	//LINK CRSSINGS THAT WILL BE ASSIGNED TO NEW EDGE
// 	x2->next_crossing = y2;
// 	x2->prev_crossing = y2;
// 	y2->next_crossing = x2;
// 	y2->prev_crossing = x2;


// 	//return array of pointers to linked new crossings (no colors, no ids)
// 	array<Crossing*, 4> new_crossings = {x1, y1, x2, y2};
// }









Edge& Edge::join_admissible_edges(Edge* edge2, Crossing* seg1_start, Crossing* seg2_start, string new_color_new_edge, string new_color_old_edge){
	//seg1_start - początek odcinka łączenia, na krawędzi, na której wywoływana metoda,
	//seg2_start - początek odcinka łączenia, na krawędzi, która wstawiana do this,
	//edge2 - krawędź wstawiana do bieżącej (ją później usuwamy)

	// //(not-comprihensive) check if segments are admissible indeed
	// assert( (seg1_start->left_color == seg2_start->left_color) ^ (seg1_start->right_color == seg2_start->right_color) );


	// //get ends of segments
	// Crossing* seg1_end = seg1_start->next_crossing;
	// Crossing* seg2_end = seg2_start->next_crossing;

	//##################################
	//
	//
	//
	// TO PÓŹNIEJ PRZEPISAĆ NA POZIOM GRAFU CHYBA NAJLEPIEJ
	//
	//
	//
	//##################################

	this->length += (2 + edge2->length); //wstawienie dwóch nowych skrzyżowań, które powstają na okręgu powstałym z połaczenia przy ruchu


	// ###### CREATING NEW CROSSINGS ######
	//new crossings (__1 conjugate to __2)
	// int x_sign = 1; //maybe randomly chosen between +/-1 in later versions
	// Crossing* x1 = new Crossing(x_sign);
	// Crossing* x2 = new Crossing(x_sign); //conjugate to x1
	// Crossing* y1 = new Crossing(-x_sign);
	// Crossing* y2 = new Crossing(-x_sign);


	// //new crossings - conjugates
	// x1->conjugate_crossing = x2;
	// x2->conjugate_crossing = x1;
	// y1->conjugate_crossing = y2;
	// y2->conjugate_crossing = y1;


	// //setting names for new crossings (version for testing)
	// x1->name = "x1";
	// x2->name = "x2";
	// y1->name = "y1";
	// y2->name = "y2";



	// ###### CREATING NEW SMALL EDGE ######

	// //create new edge (with crossings x2,y2)
	// Edge* p_new_small_edge = new Edge();
	// //Edge new_small_edge = Edge();
	// p_new_small_edge->push_crossing(x2);
	// p_new_small_edge->push_crossing(y2);



	// ###### INSERTING THE SECOND OF JOINING EDGES TO THE FIRST ONE ######

	// //insert new crossing x to edge1 (this) (and link the startpoint of edge2 with x)
	// x1->prev_crossing = seg1_start;
	// x1->next_crossing = seg2_end;
	// seg1_start->next_crossing = x1;
	// seg2_end->prev_crossing = x1;


	// //insert new crossing y to edge1 (and link the endpoint of edge2 with y)
	// y1->prev_crossing = seg2_start;
	// y1->next_crossing = seg1_end;
	// seg2_start->next_crossing = y1;
	// seg1_end->prev_crossing = y1;

	//delete edge2 (it has been inserted to edge1)
	delete edge2;


	// ###### RECOLORING ######
	//######################################################################
	//
	//
	//			RECOLORING HERE 
	//			OUTGOING EDGES PROPERTIES USED HERE
	//
	//

	// //are we joining two outgoing (by the color on the left side) or two incoming (by the color on the right side) edges
	// int joining_side = (seg1_start->left_color == seg2_start->left_color) ? -1 : 1;

	// //crossing put on edge1 before all crossings from edge2
	// x1->get_sidecolor_attr(joining_side) = x1->next_crossing->get_color(1, joining_side);
	// x1->get_sidecolor_attr(-joining_side) = x1->next_crossing->get_color(1, -joining_side);




	//colors for crosing x2 (conjugate to x1) on new small circle
	x2->get_sidecolor_attr(joining_side) = x1->get_color(1,-joining_side);
	x2->get_sidecolor_attr(-joining_side) = new_color_new_edge;



	//colors for y1 (on joined edge)
	y1->get_sidecolor_attr(joining_side) = x1->get_color(-1, joining_side);
	y1->get_sidecolor_attr(-joining_side) = x1->get_color(-1, -joining_side);
	//y1->left_color = x1->prev_crossing->left_color;//######## tu będzie nowy kolor zaraz (+'joining'-side)
	//y1->right_color = x1->prev_crossing->right_color; //-'joining'-side



	//colors for y2
	y2->get_sidecolor_attr(joining_side) = y1->get_color(1, -joining_side);
	y2->get_sidecolor_attr(- joining_side) = new_color_new_edge;
	//y2->left_color = y1->right_color; //+'joining'-side
	//y2->right_color = new_color_new_edge; //-'joining'-side

	//int start_side = -1; //outgoing edges
	y1->traverse_color_and_change_it(1, joining_side, new_color_old_edge);


	//
	//
	//
	//######################################################################




	//return *p_new_small_edge;
	return *p_new_small_edge;

	///////summary ///////
	//wstaw x
	//wstaw dobrze skrzyżowania z edge 2 //robi się samo
	//wstaw y
	//usuń edge2 jako krawędź (ale skrzyżowań nie) (czyli tylko zwolnić miejsce spod wskaźnika)
	//utwórz edge3 z odpowiednim podłączeniem i dwoma nowymi skrzyzowaniami
	//zrob całość przekolorowania (jezeli nie zrobione z punktami wczesniej)
}
/**/

