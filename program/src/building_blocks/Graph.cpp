#include "Graph.h"

#include <iostream>
#include <unordered_map>

using namespace std;

//g++ -std=c++17 ./src/parser/PD_X.cpp ./src/parser/Diagram.cpp ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp ./src/building_blocks/Color.cpp ./src/building_blocks/CrossingsCollection.cpp ./src/building_blocks/EdgesCollection.cpp ./src/building_blocks/ColorsCollection.cpp ./src/building_blocks/Graph.cpp


Graph::Graph(Diagram &colored_diagram){

	cout << "Building Graph object..." << endl;
	this->graph_colors = ColorsCollection();
	this->graph_crossings = CrossingsCollection();
	this->graph_edges = EdgesCollection();


	// Set of PD-diagram segments not used so far. Segments will be removed from this collection when visited
	unordered_set<int> segments_todo;
	segments_todo.reserve(colored_diagram.segment_colors.size());
	for(const auto& seg_colors : colored_diagram.segment_colors){ segments_todo.insert(seg_colors.first); }


	// Auxiliary structure for managing Crossings conjugations during Graph building
	unordered_map<int,Crossing*> conjugations;
	conjugations.reserve(colored_diagram.pd_crossings.size());
	for(const auto& id_pdx : colored_diagram.pd_crossings) { conjugations[id_pdx.first] = nullptr;}


	// Create new Edges as long as there are any unused segments in `segments_todo` set -- each loop run creates new Edge (Seifert circle)
	while( !segments_todo.empty() ){

		//cout << "------------------- NEW EDGE ------------------------------------------------------" << endl;
		int edge_id;
		int start_segment;
		int current_segment;
		int current_pdx_id;
		int pdx_side;



		// Create new empty Edge
		edge_id = graph_edges.add_new_return_key();
		//cout << "New edge id: " << edge_id << "." << endl;


		// Get start segment for new Edge (=Seifert circle)
		start_segment = *segments_todo.begin();
		segments_todo.erase(start_segment);
		//cout << "Start segment number: " << start_segment << "." << endl;
		

		// Get start PDX crossing (-> having start segment as incoming)
		current_pdx_id = colored_diagram.get_pdxid_with_segment_as_incoming_arm(start_segment);
		PD_X &current_pdx = colored_diagram.pd_crossings.at(current_pdx_id);
		pdx_side = current_pdx.return_arm_side(start_segment);


		// Move to the outgoing segment of current PDX 
		current_segment = current_pdx.get_outgoing_arm_number(pdx_side);

		// Create first Crossing for new Edge - from start PDX
		Crossing* start_cx = graph_crossings.add_new_return_ptr(current_pdx.get_sign());
								

		Crossing* current_cx = start_cx;
		Crossing* prev_cx = nullptr;

		current_cx->set_edge_id(edge_id);
		int l_color = colored_diagram.get_segment_color(current_segment, -1);
		int r_color = colored_diagram.get_segment_color(current_segment, 1);
		current_cx->set_color(l_color, -1);
		current_cx->set_color(r_color, 1);


		// Manage conjugations
		if(conjugations.at(current_pdx_id)==nullptr){ conjugations.at(current_pdx_id) = current_cx; }
		else{
			current_cx->set_conjugate(conjugations.at(current_pdx_id)); //note: check if everything ok here
			conjugations.at(current_pdx_id)->set_conjugate(current_cx);
		}

		// Update ColorsCollection
		graph_colors.add_crossing_info_to_collection(*start_cx);

		// Set first Crossing id as current Edge anchor
		graph_edges.set_edge_anchor(edge_id, start_cx->get_crossing_id());


		// Follow PDXs from current Seifert circle (=Edge), create Crossing objects, link and manage conjugations
		while(current_segment != start_segment){	
			//cout << "current segment number: " << current_segment << "." << endl;
			/* 1) Move to PDX that has current segment as incoming arm (->this is next PDX in Seifert circle) */
			/* 2) Erase current segment from `segments_todo` (erasing is done when visiting incoming PDX arm) */
			/* 3) Move forward to the outcoming arm of current PDX (on its appropriate side) */
			/* 4) Create new Crossing object 
			      Manage:
			        - colors,
			        - sign,
			        - previous- and next- Crossings
			        - conjugation information */
			/* 5) Update ColorsCollection information after adding new Crossing*/


			// (1) Move to PDX that has current segment as incoming arm (->this is next PDX in Seifert circle)
			current_pdx_id = colored_diagram.get_pdxid_with_segment_as_incoming_arm(current_segment);
			PD_X &current_pdx = colored_diagram.pd_crossings[current_pdx_id];  //PDX that has segment as incoming
			pdx_side = current_pdx.return_arm_side(current_segment); 

			// (2) Erase current segment from `segments_todo` (erasing is done when visiting incoming PDX arm)
			segments_todo.erase(current_segment);

			// (3) Move forward to the outcoming arm of current PDX (on its appropriate side)
			current_segment = current_pdx.get_outgoing_arm_number(pdx_side);

			/*  *(4)*  **************************************************************** */ 
			prev_cx = current_cx;
			current_cx = graph_crossings.add_new_return_ptr(current_pdx.get_sign());

			current_cx->set_edge_id(edge_id);
			int l_color = colored_diagram.get_segment_color(current_segment, -1);
			int r_color = colored_diagram.get_segment_color(current_segment, 1);
			current_cx->set_color(l_color, -1);
			current_cx->set_color(r_color, 1);

			// Manage cyclic connections (between new Crossing and previous in Seifert circle)
			prev_cx->set_next_ptr(current_cx);
			current_cx->set_prev_ptr(prev_cx);

			// Manage conjugations (between new Crossing and derived from the same PDX, if PDX used earlier)
			if(conjugations.at(current_pdx_id)==nullptr){ conjugations[current_pdx_id] = current_cx; }
			else{
				current_cx->set_conjugate(conjugations.at(current_pdx_id));
				conjugations.at(current_pdx_id)->set_conjugate(current_cx);
			}

			// Update ColorsCollection
			graph_colors.add_crossing_info_to_collection(*current_cx);
		}

		// Set prevs and next pointers for first and last crossings to close the cycle
		current_cx->set_next_ptr(start_cx);
		start_cx->set_prev_ptr(current_cx);
	};


	graph_colors.assign_to_todo_done(); //note: is it useful here after code updates?

	//cout << "Creating Edges finished." << endl;
	cout << "...building Graph object finished." << endl;
	cout << "Number of edges in Graph: " << (this->graph_edges).edges.size() << endl;
	//cout << "End of constructor call for Graph object." << endl;
	cout << "==================================================================================================" <<endl << endl << endl;
};



// todo note (optional): join these two print methods to a single version
void Graph::print_edge_crossing_by_crossing(int edge_id){
	Edge &e = *(graph_edges.edges.at(edge_id));
	Crossing* start_cx_ptr = graph_crossings.crossings.at((e.anchor_crossing_id));
	Crossing* cx_ptr = start_cx_ptr->next_crossing;

	cout <<"\t";
	start_cx_ptr->print_crossing(); cout << endl;
	while( cx_ptr != start_cx_ptr){
		cout <<"\t";
		cx_ptr->print_crossing(); cout << endl;
		cx_ptr = cx_ptr->next_crossing;

	}
}; 
void Graph::print_graph(){

	cout << "--------------------------------------------------" << endl;
	cout << "---------- Printing Graph Edge by Edge -----------" << endl << endl;
	for(const auto &pair : graph_edges.edges){
		cout << "Edge with collection id: " << pair.first << endl;
		this->print_edge_crossing_by_crossing(pair.first);
	}
	cout << "--------------------------------------------------" << endl;
}





// Reading braid sequence from the chain form of the graph
vector<int> Graph::read_braid_word(){

	assert(this->is_in_braid_form());

	vector<int> braid_word; //may be replaced by another structure representing braids if needed
	//reserve vector size -- when crossings set size added to graph attributes

	// Checking if not trivial unknot case
	if(graph_edges.size() == 1){
		assert(graph_crossings.size() == 0);
		assert(graph_colors.size() == 0);
		cout << "The braid consist of one strand (no crossings). Braid word is empty." << endl;
		return braid_word;
	}



	/* **************************************************************************************************** */
	/* Read the crossings preserving their order in closed braid and then write the sequence of generators */

	/* Auxiliary structures */
	unordered_map<Crossing*, Crossing*> crossings_sequence; //key: pointer to Crossing, value: pointer to the Crossing on the rightside of key Crossing in current braid sequence (only conjugates from lower edges included in values and keys)
	unordered_map<int, int> edge_ids_to_generators; //mapping {edge_id : sigma_generator_subscript} (j in sigma_j)


	/* Get id of bottom (start) and top (stop) chain edges */
	int top_edge_id = this->get_key_of_top_chain_edge(); // id of top chain edge in EdgesCollection 
	int e_i_key = this->get_key_of_bottom_chain_edge(); // edge_{i} key in EdgesCollection (current edge)
	int e_i_minus_1_key = -1;     //edge_{i-1} key in EdgesCollection (prviously visited edge)
	int current_generator_no = 0; //sigma_i subscript for current edge (=position of the edge in chain starting from the bottom)
	


	Crossing* e_i_1_START = nullptr; //START crossing on the next (upper) edge
	Crossing* e_i_START_cx; //START crossing on the current edge
	Crossing* e_i_current_cx; //current crossing on current edge
	Crossing* braid_prev_cx; //pointer to the braid word sequence element, after which the next element should be added (previously visited crossing or its lower conjugate, if current Crossing has conjugate on lower edge)
	

	e_i_START_cx = this->get_anchor_crossing_ptr_for_edge_key(e_i_key);

	// Add very first crossing to the sequence
	crossings_sequence.insert({e_i_START_cx, e_i_START_cx}); //or point to null -- depends if we want linear or circular form at that stage (circular for now)

	
	// Move one crossing forward on the current (first) edge
	braid_prev_cx = e_i_START_cx;
	e_i_current_cx = e_i_START_cx->next_crossing;
	


	// Traverse graph edge by edge from the bottom to the top and extend braid sequence
	while(e_i_key != top_edge_id){ //always at least one loop (unknot case handled earlier; at least two edges in all other cases)
		
		// Map current edge identifier to corresponding generator subscript
		++current_generator_no;
		edge_ids_to_generators.insert({e_i_key, current_generator_no});


		// Go crossing by crossng along current edge and add not-yet-visited crossings from current edge to the braid word
		do{	

			// Case 1: current crossing has conjugate on upper edge (means that none of crossings from this pair have been seen yet)
			// Action: add crossing to the braid sequence
			if(e_i_current_cx->give_conjugate_edge_id() != e_i_minus_1_key){


				/* Add current crossing to sequence: point from new braid crossing to pointed from previous and point from previous to new in sequence holder */
				crossings_sequence.insert({e_i_current_cx, crossings_sequence.at(braid_prev_cx)}); //add current cx to braid word; it's next is now the prev's old next; insert don't overwrite element -- if only one crossing on the first, no new element added
				crossings_sequence.at(braid_prev_cx) = e_i_current_cx;


				/* If this is the first crossing with conjugate on upper edge encountered on current edge: set this crossing conjugate as START for next edge */
				if(e_i_1_START == nullptr){ e_i_1_START = e_i_current_cx->conjugate_crossing;}

				/* Set current crossing as 'prev in sequence' (it is the lower conjugate) */
				braid_prev_cx = e_i_current_cx;
			}

			// Case 2: representant of this crossing pair was added to sequence while visiting previous edge
			// Action: just move to the next crossing
			else{ 
				// prev crossing in the braid is located on the edge below (lower conjugate))
				/* Updating previous braid crossing pointer */
				braid_prev_cx = e_i_current_cx->conjugate_crossing; 			
			}

			/* Move to the next crossing on current edge */
			e_i_current_cx = e_i_current_cx->next_crossing;
		}
		while(e_i_current_cx != e_i_START_cx);	


		/* Move to upper edge (=update current and prev e_i keys) */
		e_i_minus_1_key = e_i_key;
		e_i_key = this->give_outcoming_neigbour_id(e_i_minus_1_key);

		/* Set start information for the new current edge (in this implementation it is always a crossing conjugated with edge below) */
		e_i_START_cx = e_i_1_START;
		e_i_current_cx = e_i_1_START;
		braid_prev_cx = e_i_1_START->conjugate_crossing; //previous crossing in braid for current crossing (in this implementation we leverage the fact that start crossing always has conjugate on a bottom edge)
		
		e_i_1_START = nullptr; //START crossing for the new upper edge is not known for now (will be defined during current edge traversal)
	}



	// Convert sequence of crossings to resulting vector of signed generators
	braid_word.reserve(crossings_sequence.size());
	Crossing* start_seq_crossing = (crossings_sequence.begin())->first;
	Crossing* current_seq_crossing = start_seq_crossing;
	int current_generator;

	do{
		current_generator = edge_ids_to_generators.at(current_seq_crossing->edge_id);
		current_generator = (current_seq_crossing->sign == -1) ? -current_generator : current_generator;
		braid_word.push_back(current_generator);

		current_seq_crossing = crossings_sequence.at(current_seq_crossing);
	}
	while(current_seq_crossing != start_seq_crossing);

	assert(braid_word.size() == (graph_crossings.size()/2));
	return braid_word;
}



// Transforming graph to the chain form
void Graph::transform_to_braid_form(){
	cout << endl << "Transforming knot diagram to braid form..." << endl;

	if(this->is_in_braid_form()){
		cout << "Knot diagram is already in a braid form." << endl << endl;
		return;
	}

	while(!(this->is_in_braid_form())){
		this->perform_move();


		//for testing only
		/*
		cout << "Graph crossings after move: " << endl;
		graph_crossings.print_collection();
		cout << "Graph edges after move: " << endl;
		this->print_graph();
		cout << "Graph colors after move: " << endl;
		graph_colors.print_colors_collection();
		*/
		}
		cout << "...done." << endl << endl;
}

void Graph::perform_move(){
	//cout << "Performing move..." << endl;

	/*
	(1) Check if graph is not in braid form already
	(2) Get pair of admissible Edges
	(3) Get pair of "admissible" Crossings
	(4) Create new edge
	(5) Join old edges (but first, generate new crossings)
	(6) Perform "small colorings"
	(7) Perform "big recoloring" on a part of a mv_color
	*/

	/* get a pair of admissible segments */
	array<int,3> c1_c2_side = this->find_admissible_segments_starts_and_side();
	int c1, c2, mv_side;
	c1 = c1_c2_side[0];
	c2 = c1_c2_side[1];
	mv_side = c1_c2_side[2];


	//make all neccessary operations to perform move
	this->join_add_and_recolor_edges(c1, c2, mv_side);

	/*cout << "...performing move finished." << endl << endl;*/
}

array<int,3> Graph::find_admissible_segments_starts_and_side(){
	//find 2 edges and pair of crossings on which we can perform the move:
		//chose an arbitrary color on todo list of colors
		//arbitrary chose a side of this color (but has to contain at least two seiferts (=edges) )
		//arbitrary chose one crossing from each of edges chosen in a previous step

	//ENSURE THAT GRAPH IS NOT IN CLOSED-BRAID FORM
	if(graph_colors.colors_todo.size() == 0){
		cout << "Already in closed braid form." << endl;
		cout << "No more moves to perform" << endl;
		return {-100,-100,-100};}


	//FIND COLOR, EDGES AND CROSSINGS TO PERFORM MOVE
	array<int,4> mvcol_side_e1_e2 = graph_colors.give_todo_color_side_and_edges();
	//array<Crossing*,2> mv_crossings = graph_colors.give_todo_crossing_ptrs(mvcol_side_e1_e2[0], mvcol_side_e1_e2[1], mvcol_side_e1_e2[2], mvcol_side_e1_e2[3]); //(mv_color, mv_side, mv_edge_1, mv_edge_2)

	int mv_col = mvcol_side_e1_e2[0];
	int mv_side = mvcol_side_e1_e2[1];
	int mv_e1 = mvcol_side_e1_e2[2];
	int mv_e2 = mvcol_side_e1_e2[3];

	array<int,2> c1_c2 = graph_colors.give_mv_crossing_ids(mv_col, mv_side, mv_e1, mv_e2);

	/*
	cout << endl << "\tFound admissible crossings: " << c1_c2[0] << " and " << c1_c2[1] << endl;
	cout << "\tOn edges: " << mv_e1 << " and " << mv_e2 << endl;
	cout << "\tMove color is: " << mv_col << endl;
	cout << "\tMv_side is: " << mv_side << endl << endl;
	*/

	return {c1_c2[0], c1_c2[1], mv_side};
}

/**
 *
 * @brief Perfrm elementary operation on a graph given an admissible pair of segments and move side (left/right)
 *
 * Method description:
 *	This method, given ids of two segment starts: cx1_id, cx2_id,
 *	performs a move (=elementary operation) on the Vogel graph.
 *	That means it:
 *		1. Changes all e2 keys in CollorsCollection to e1 keys (this means absorbing edge2 by edge1 on a color level);
 *		2. Creates new Edge object; adds it to the Edges collection;
 *		3. Creates 2 new colors; add edge ids to them; adds them to the ColorsCollection;
 *		4. Creates 4 new Crossing objects (x1, x2, y1, y2); adds them to the CrossingsCollection;
 *		5. Performs small (re)colorings;
 *		6. Performs big recoloring (second part of joining color)
 *
 * @param cx1_id Identifier of the start crossing of admissible segment on edge 1 (absorbing edge)
 * @param cx2_id Identifier of the start crossing of admissible segment on edge 2 (absorbed edge)
 * @param side side on which we found the shared color; -1 for left +1 for right. Determined by the node shared by the edges (initial/terminal)
*/
void Graph::join_add_and_recolor_edges(int cx1_id, int cx2_id, int mv_side){

	// Get color from the joining side of segments
	int mv_color = (this->get_crossing_ptr_by_key(cx1_id))->get_color(mv_side);

	// Get identifiers of admissible-segments edges
	int e1_id, e2_id;
	e1_id = (this->get_crossing_ptr_by_key(cx1_id))->edge_id;
	e2_id = (this->get_crossing_ptr_by_key(cx2_id))->edge_id;

	// Move all crossings from e2 to e1: (i) move in ColorsCollection and (ii) update 'edge_id' attributes in Crossing objects)
	this->absorb_e2_by_e1_in_colors_collection(e1_id, e2_id);


	// Create new edge e3 and add it to the EdgesCollection
	graph_edges.remove_edge(e2_id);
	int new_e_id = graph_edges.add_new_return_key();


	// Create two new colors, add them to ColorsColeection (one color for new edge, second - for recoloring part of the joining color)
	int newedge_color_id = (this->graph_colors).add_new_empty_color_return_id();
	int recolor_color_id = (this->graph_colors).add_new_empty_color_return_id();

	
	// Create four new crossings, add to the CrossingsCollection
	array<Crossing*, 4> x1_y1_x2_y2 = this->generate_new_crossings_linked_and_conjugated(cx1_id, cx2_id, e1_id, new_e_id); 


	// Segments recoloring: add colors to the new segments
	this->perform_small_recolorings(mv_side, cx1_id, cx2_id, x1_y1_x2_y2, newedge_color_id);

	// Segments recoloring: recolor part of the joining area
	Crossing* y1 = x1_y1_x2_y2[1];
	this->perform_big_recoloring(y1->crossing_id, mv_side, mv_color, recolor_color_id);
}


// Traversing graph upwards
/**
 * 
 * Gives id of any edge having a tail in e's head, where e is an edge with edge_id key in EdgesCollection
 */
int Graph::give_outcoming_neigbour_id(int edge_id){
	assert(graph_edges.is_key_in_collection(edge_id));

	//get color that is shared between edge of interest and its outcoming naeigbour (at least one of them)
	int color_id = (this->get_crossing_ptr_by_key(((this->get_edge_ptr_by_key(edge_id))->anchor_crossing_id)))->right_color;
	Color* color = this->get_color_ptr_by_key(color_id);

	assert(color->size_left() >0); //ensure that outcoming neigbour exists (may be changed to returning id=-1)

	return ((color->leftside_seiferts).begin())->first;
}



/* ============================================================================================================ */
/* Private                                                                                                    */

/**
 *
 * @brief Add color to Crossing object and manage information in color collection
 * 
*/
void Graph::set_crossing_color(int crossing_id, int new_color_id, int side, bool enable_override){
	//cout << "\t\t In: Graph::set_crossing_color(int crossing_id, int new_color_id, int side, bool enable_override)" << endl;
	assert(abs(side)==1);
	assert(graph_crossings.crossings.find(crossing_id) != graph_crossings.crossings.end());
	assert(new_color_id > -1);

	// Get Crossing ptr by id
	Crossing* crossing = this->get_crossing_ptr_by_key(crossing_id);

	// Get old color (may be defined (>-1) or undefined (-1))
	int old_color_id = crossing->get_color(side);


	// Check if there are no override violations
	try{
		if( (old_color_id != -1) && (!enable_override)){ throw old_color_id; }
	}
	catch(int old_color_id){
		cout << "You are trying to override an existing color (" << old_color_id << ") on (" << side << ") side of crossing with id=" << crossing_id << " with color (" << new_color_id << ")." << endl;
		cout << "If that was your intention, set \'enable_override\'=true in \'Graph::set_crossing_color\'." << endl;
		cout << "Exiting the program." << endl;
		exit(1);
	}

	// If overriding existing color -- remove crossing info for this color from ColorsCollection
	if(old_color_id != -1){
		if(new_color_id == old_color_id){
			cout << "\tColor (" << new_color_id << ") is already set on (" << side << ") side of Crossing with id=" << crossing_id << "." << endl;
			return;
		}
		else{
			//remove crossing id from appropriate color side; remove edge key / whole color if necessary.
			Color* old_color = graph_colors.get_ptr_by_key(old_color_id);
			old_color->remove_crossing_id_from_colorside(crossing, side);
		}
	}

	// Set new color as crossing attribute
	crossing->set_color(new_color_id, side);

	// Update information about new color on the Crossing side in ColorsCollection
	graph_colors.add_side_crossing_info_to_collection(*crossing, side);
}




/* -------------------------------------------- */
/*           PERFORMING MOVE                   */
/* ------------------------------------------- */


/**
 * 
 * 	Moves all crossings from under key e2_id to under key e1_id in given side of given color
 * 	(function creates e1_id key in given side of a given color if not exist).
 * 	and changes each moved crossing attribute `edge_id` from e2_id to e1_id.
 */
void Graph::absorb_e2_by_e1_in_single_colorside(int e1_id, int e2_id, int color_id, int side){

	//get color
	Color* col_ptr = this->get_color_ptr_by_key(color_id);
	assert(col_ptr->is_edge_in_side(e2_id, side));

	//get side of the color
	unordered_map<int, unordered_set<int>> &sideseiferts = col_ptr->give_sideseiferts_ref(side);
	
	//insert e1 key to requested side of the color if not present
	if( !(col_ptr->is_edge_in_side(e1_id, side)) ){
		sideseiferts.insert( {e1_id, {}} ); 
	} 

	//move all crossings from under e2 key to under e1 key and change crossings attributes
	unordered_set<int> &e2_crossings = sideseiferts.at(e2_id);
    for (const auto& it : e2_crossings){
        (sideseiferts.at(e1_id)).insert(it);
        Crossing* curr_cx = this->get_crossing_ptr_by_key(it);
        curr_cx->edge_id = e1_id;
    }

    //remove e2 key from sideseiferts
    sideseiferts.erase(e2_id);
}

/**
 * @brief Moves crossings from edge e1 to edge e2 in ColorsCollection
 *
 * Moves all crossings from e2 key to e1 key in all colors present on e2 crossings (also changes edge_id attribute in moved crossing objects).
 * Complexity: O(number_of_crossings_in_edge2)
 */
void Graph::absorb_e2_by_e1_in_colors_collection(int e1_id, int e2_id){


	//cout << "\tAbsorbing e2 (id = " << e2_id << ") by e1 (id = " << e1_id << ") in ColorsCollection..." << endl;

	unordered_set<int> leftside_colors_done;
	unordered_set<int> rightside_colors_done;
	//todo (optional): maybe reserve memory for colors done

	// Get the start crossing on edge2
	Edge* e2 = this->get_edge_ptr_by_key(e2_id);
	Crossing* anch_cx = this->get_crossing_ptr_by_key(e2->anchor_crossing_id);
	Crossing* curr_cx = anch_cx;

	do{
		int l_col = curr_cx->left_color;
		int r_col = curr_cx->right_color;

		// If e2 key not removed from current sidecolor yet - move all its crossings to e1 key and remove e2 from keys
		if( leftside_colors_done.find(l_col) == leftside_colors_done.end() ){
			this->absorb_e2_by_e1_in_single_colorside(e1_id, e2_id, l_col, -1);
			leftside_colors_done.insert(l_col);
		}
		if( rightside_colors_done.find(r_col) == rightside_colors_done.end()){
			this->absorb_e2_by_e1_in_single_colorside(e1_id, e2_id, r_col, 1);
			rightside_colors_done.insert(r_col);
		}


		// Go to the next crossing on edge2
		curr_cx = curr_cx->next_crossing;
	}
	while(curr_cx != anch_cx);

	/*cout << "\t...absorbing e2 by e1 in ColorsCollection finished." << endl << endl;*/
}

/**
 *
 * Function description:
 *	This function,
 *	given the pointers to start Crossings of both admissible segments:
 *		1. creates 4 new Crossings (x1, y1, x2, y2),
 *		2. links them with appropriate previous and next crossings
 *		3. assigns them their conjugates (_1 conjugated with _2, for _ in {x,y})
 *		4. returns an array of pointers to newly created crossings
 *	NOTE: Crossings created here don't have any colors assigned -- this should be done in the next steps.
 */
array<Crossing*, 4> Graph::generate_new_crossings_linked_and_conjugated(int cx1_id, int cx2_id, int e1_id, int new_e_id){

	//cout << "\tGenerating new crossings for the current move..." << endl;
	/* start1 ----> x1 -----> end2 --- [..egde2 from end to start...] --- start2 ----> y1 ----> end1 */

	// Get start crossings of admissible pair segments (as Crossing*)
	Crossing* seg1_start = graph_crossings.get_ptr_by_key(cx1_id);
	Crossing* seg2_start = graph_crossings.get_ptr_by_key(cx2_id);

	// Get ends of admissible pair segments
	Crossing* seg1_end = seg1_start->next_crossing;
	Crossing* seg2_end = seg2_start->next_crossing;

 	//Create new Crossing objects
	//Convention:
	// - x2 is conjugate to x1, y2 to y1
	// - x2, y2 are inserted on the new edge;
	// - x1 is always a name of new crossing 'next' for seg1_start

	int x_sign = 1; //note: may be randomly sampled from {-1, 1} in later versions

	Crossing* x1 = graph_crossings.add_new_return_ptr(x_sign);
	int x2_id = graph_crossings.generate_new_crossing(x_sign); //conjugate to x1; id needed to anchor new edge on it
	Crossing* x2 = this->get_crossing_ptr_by_key(x2_id); 
	Crossing* y1 = graph_crossings.add_new_return_ptr(-x_sign);
	Crossing* y2 = graph_crossings.add_new_return_ptr(-x_sign);

	// Set names for new crossings (version for testing)
	x1->name = "x1";
	x2->name = "x2";
	y1->name = "y1";
	y2->name = "y2";

	// Set conjugations
	x1->conjugate_crossing = x2;
	x2->conjugate_crossing = x1;
	y1->conjugate_crossing = y2;
	y2->conjugate_crossing = y1;


	// Add x1, y1 to e1
	x1->edge_id = e1_id;
	y1->edge_id = e1_id;

	// Link x1 with neighbours
	x1->prev_crossing = seg1_start;
	x1->next_crossing = seg2_end;
	seg1_start->next_crossing = x1;
	seg2_end->prev_crossing = x1;

	// Link y1 with neigbours
	y1->prev_crossing = seg2_start;
	y1->next_crossing = seg1_end;
	seg2_start->next_crossing = y1;
	seg1_end->prev_crossing = y1;


	// Add x2, y2 to the new edge
	(this->get_edge_ptr_by_key(new_e_id))->set_anchor(x2_id);
	x2->edge_id = new_e_id;
	y2->edge_id = new_e_id;

	// Link x2 and y2 on the new edge
	x2->next_crossing = y2;
	x2->prev_crossing = y2;
	y2->next_crossing = x2;
	y2->prev_crossing = x2;


	//cout << "\t...generating four new linked and conjugated crossings finished." << endl << endl;
	//return array of pointers to linked new crossings (no colors, no ids)
	//todo note (optional): maybe pass array of crossing ids -- to comunicate only by ids in the graph 
	return {x1, y1, x2, y2};
}

void Graph::perform_small_recolorings(int mv_side, int cx1_id, int cx2_id, array<Crossing*,4> x1_y1_x2_y2, int newedge_color_id){
	//cout << "CHECK FOR CORRECTNESS LATER (Graph.perform_small_recolorings)" << endl<< endl;
	//cout << "\tPerforming small recolorings...." << endl;

	// Get pointers to start crossings of admissible segments
	Crossing* seg1_start = this->get_crossing_ptr_by_key(cx1_id);
	Crossing* seg2_start = this->get_crossing_ptr_by_key(cx2_id);
	assert(seg1_start->get_color(mv_side) == seg2_start->get_color(mv_side));

	// Get 'exclusive' seg1 and seg2 colors and joining color
	int col_seg1, col_seg2, mv_col;
	col_seg1 = seg1_start->get_color(-mv_side);
	col_seg2 = seg2_start->get_color(-mv_side);
	mv_col = seg1_start->get_color(mv_side);

	//cout << "\t\tcol_seg1: (" << col_seg1 << "), col_seg2: (" << col_seg2 << "), mv_col: (" << mv_col << ")" << endl;

	// Unpack newly created crossing from the array passed
	Crossing* x1 = x1_y1_x2_y2[0];
	Crossing* y1 = x1_y1_x2_y2[1];
	Crossing* x2 = x1_y1_x2_y2[2];
	Crossing* y2 = x1_y1_x2_y2[3];

	/*
	x1->print_crossing(); cout << endl;
	y1->print_crossing(); cout << endl;
	x2->print_crossing(); cout << endl;
	y2->print_crossing(); cout << endl;
	*/


	// Set colors on new crossings
	// Note: new color on new edge -- is always on the opposite side to mv_sde
	// Comment: 'this->set_crossing_color' adds color to the Crossing object and manages changes in ColorsCollection
	this->set_crossing_color(x2->crossing_id, newedge_color_id, -mv_side); //x2->set_color(newedge_color_id, -mv_side);
	this->set_crossing_color(y2->crossing_id, newedge_color_id, -mv_side); //y2->set_color(newedge_color_id, -mv_side);

	// Prolonge seg1 and seg2 -mv_side colors on the new edge (mv_side of new edge)
	this->set_crossing_color(x2->crossing_id, col_seg2, mv_side);  //x2->set_color(col_seg2, mv_side);
	this->set_crossing_color(y2->crossing_id, col_seg1, mv_side);  //y2->set_color(col_seg1, mv_side);

	// Update colors for new crossings inserted to e1
	this->set_crossing_color(x1->crossing_id, mv_col, mv_side);  //x1->set_color(mv_col, mv_side);
	this->set_crossing_color(x1->crossing_id, col_seg2, -mv_side); //x1->set_color(col_seg2, -mv_side);

	//y1 <- mv_side of y1 will get the new recolring-color during the big_recoloring
	//this->set_crossing_color(y1->crossing_id, mv_color, mv_side); //dummy setting --> this color will be changed during recoloring
	this->set_crossing_color(y1->crossing_id, col_seg1, -mv_side); //y1->set_color(col_seg1, -mv_side);

	/*
	cout << "\tCrossings created for current elementary operation:" << endl;
	x1->print_crossing(); cout << endl;
	x2->print_crossing(); cout << endl;
	y1->print_crossing(); cout << endl;
	y2->print_crossing(); cout << endl;
	*/

	/*cout << "\t...performing small recolorings finished." << endl << endl;*/
}

void Graph::perform_big_recoloring(int start_cx_id, int mv_side, int mv_color, int new_color){
	assert(abs(mv_side) == 1);
	//cout << endl << "IN Graph::perform_big_recoloring(int start_cx_id, int mv_side, int mv_color, int new_color)" << endl;


	/* Set start traversal direction and side (we always start in the direction of \tau_{AB} edge) */ 
	int d = 1;
	int s = mv_side;


	// Get pointer of start "y1" Crossing; first step is always from "y1" in the direction of the edge
	Crossing* y1 = this->get_crossing_ptr_by_key(start_cx_id);
	this->set_crossing_color(y1->crossing_id, new_color, mv_side); //add new color on appropriate side of new y1 crossing

	//ASSIGN CURRENT AND (dummy) PREVIOUS CROSSING POINTERS
	Crossing* current_cx = y1;
	Crossing* prev_cx = y1->conjugate_crossing; //set to start going forward, not to conjugate in the first move
	


	/* How many time we can pass the start crossing before considering traversal complete */
	//WHEN conj(-s,-d) has the same color, we will finish traversal after arriving to START crossing the second time (not first)
	int num_start_passes_left = 0; 
	if((current_cx->conjugate_crossing)->get_color(-s,-d) == mv_color){
		++num_start_passes_left;
	}


	while(num_start_passes_left > -1){
		/* Follow face boundary Crossing by Crossing and change its color (on Crossing-level and ColorCollection level) */
		/* Change colors always when moving along the elementary segment */


		/* IF we haven't just been in conjugate crossing and if it continues old color - move to conjugate */
		if( (current_cx->conjugate_crossing != prev_cx) && (current_cx->check_if_conjugate_continues_traversal_color(d, s, mv_color) /*|| current_cx->conjugate_crossing == y1*/) ){
			
			/* Conjugate crossing continues color; moving to conjugate crossing */

			prev_cx = current_cx;
			current_cx = prev_cx->conjugate_crossing;
			d = -d;
			s = -s;
		}
		//ELSE: MOVE TO THE NEXT CROSSING ON CURRENT EDGE (IN THE CURRENT DIRECTION)
		else{
			/* Conjugate crossing does not continue color or we've just been there; moving to the next crossing in the direction */			


			/* Change appropriate color of crossing you will be leaving (technically this is the color owned by crossing behind current if we are going backward) */
			Crossing* cx_to_move = (d==1) ? current_cx : current_cx->prev_crossing;
			graph_colors.move_crossing_id_between_colors(cx_to_move, s, new_color); //mave crossing between colors in ColorsCollection
			/*note: todo-done colors lists to handle (and in previous steps too)*/
			//cout << "Graph::perform_big_recoloring(...) -- HANDLE TODO-DONE COLORS MANAGING WHEN RECOLORING!" << endl;
			current_cx->set_color(new_color, s, d); //change color attribute on Crossing level

			
			/* Move to the next crossing from current edge (means: not-conjugate) in current direction */
			prev_cx = current_cx;
			current_cx = prev_cx->give_neigbour_crossing(d);

		}


		/* Check if you have just arrived to the START (y1) Crossing */
		if(current_cx == y1){
			num_start_passes_left--;
		}

	}

	/*cout << "\t...performing big recoloring finished." << endl;*/
}




/* ---------------------------------------------------- */
/*           GENERATING BRAID WORD                      */
/* ---------------------------------------------------- */
int Graph::get_key_of_bottom_chain_edge(){

	//cout << "In 'int Graph::get_key_of_bottom_chain_edge()'." << endl;
	assert(this->is_in_braid_form());

	//find "bottom color" in colors collextion (note: this can be done more efficiently with storing top and bottom colors as ColorsCollection / Graph attributes)
	//note: in this version - iterating over all colors to ensure that there is exactly one "bottom color"
	int num_bottom = 0;
	int bottom_edge_key;


	for(const auto& pair : (this->graph_colors).colors){
		assert((pair.second)->max_side_size()==1); //additional sanity check (note: check if this hasn't been checked on the previous steps when code ready)

		//Find color that has no right edges (note: additional checking if only one such color)
		if((pair.second)->size_right()==0){
			++num_bottom;
			bottom_edge_key = ((pair.second)->leftside_seiferts.begin())->first; //get edge id from leftside seiferts
		}
	}
	assert(num_bottom == 1);
	return bottom_edge_key;
}

int Graph::get_key_of_top_chain_edge(){
	assert(this->is_in_braid_form());

	int num_top = 0;
	int top_edge_key;

	for(const auto& pair : (this->graph_colors).colors){
		assert((pair.second)->max_side_size()==1); //additional sanity check (note: check if this hasn't been checked on the previous steps when code ready)
		
		// Find the color that has no right edges (note: additional checking if only one such color)
		if((pair.second)->size_left()==0){
			++num_top;
			top_edge_key = ((pair.second)->rightside_seiferts.begin())->first; //get edge id from rightside seiferts
		}
	}
	assert(num_top == 1);
	return top_edge_key;
}


