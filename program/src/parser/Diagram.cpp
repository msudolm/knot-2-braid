#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>


#include "Diagram.h"

using namespace std;



/* ************************************************************ */
/* *****  constructor and its auxiliary methods *************** */

Diagram::Diagram(const string &pd_file){
	cout << endl << endl <<"==================================================================================================" <<endl;
	cout <<  "Reading PD-diagram from file: \'" << pd_file << "\'..." << endl;
	ifstream file(pd_file);
	if(! file.good() ){ cout<<"File " << pd_file <<" does not exist"; exit(1);} //ensure that the file is open and in a good state for reading //note: lepsza obsługa wyjątku później

	int n = 0; //pd_crossings count; and map key
	pd_crossings = {};
	max_segment_no = -1;
	

	/* ***************************************************************** */
	/* *****   read data and create PD-crossings representations   ***** */
	string line;
	int a,b,c,d;
	char delimiter;
	int m;

	while (getline(file, line)) {
		istringstream iss(line);
		if (iss >> delimiter >> a >> delimiter >> b >> delimiter >> c >> delimiter >> d >> delimiter){
			pd_crossings.insert({ n, PD_X({a,b,c,d}) }); //note: tu można zamiast i może kodować numerem, który jest na x_i (np x_1) (nie powinien się powtórzyć; sprawdzić to)
			m = max(d,max(c, max(b,a)));
			if( m > max_segment_no){max_segment_no = m;}
			n++;
		}
		else{ cout << "Wrong line format: <"<< line << ">. Line ommited." << endl;}
	}

	file.close();
	/* ***************************************************************** */


	/* ************************************************************************************************************************************************ */
	/* 1. compute signs of PD-crossings (based on the relationships betwen segment numbers and using information from max_segment_no)                   */
	/* 2. generate map segment_id --> (pdx1_id, pdx2_id); pdx_1 is a crossing for which segment is an outgoing arm, pdx2 - has segment as incoming arm */
	cout << "\tMax segment no. encountered: " << max_segment_no << endl;
	cout << "\tNow computing PD-crossings signs..." << endl;
	this->compute_crossing_signs();
	cout << "\t... computing PD-crossings signs finished." << endl;
	

	segments_to_pdxs.reserve(max_segment_no);
	segment_colors.reserve(max_segment_no);

	this->calculate_segment_to_pdxs_mapping(); /* prepare segments-to-pdx_id mapping */
	this->initialize_diagram_colors(); /* mark all segments as uncolored */
	/* **************************************************************************************************************************** */

	
	//auxiliary sets for coloring diagram and assigning PD-crossing to Seifert circles //note: move to graph level?
	// uncolored_pdxs.reserve(pd_crossings.size());
	// for(const auto &pair : pd_crossings){ uncolored_pdxs.insert(pair.first);};
	// seifert_unassigned_pdxs_right = uncolored_pdxs; //note: move this one to graph level
	// seifert_unassigned_pdxs_left = uncolored_pdxs;


	cout << "...reading diagram from PD file finished." << endl << endl;};

/** DESTRUCTOR here  */

// (!) numbers in pd-code must start from 0
void Diagram::compute_crossing_signs(){

	//Case: unknot in figure-eight shape (one crossing, two segments: 0 and 1)
	if (max_segment_no == 1){
		//cout << "void Diagram::compute_crossing_signs(): in unknot of shape 8 branch (max_segment_no==1)"<<endl;
		assert(pd_crossings.size() == 1);
		PD_X& only_pdx = pd_crossings[0];

		bool is_pos, is_neg;
		is_pos = only_pdx.get_arm_label(0) == only_pdx.get_arm_label(1);
		is_neg = only_pdx.get_arm_label(0) == only_pdx.get_arm_label(3);
		assert( is_pos != is_neg); //XOR

		if(is_pos){ only_pdx.sign=1;} //possible pd-codes: (0,0,1,1) /(1,1,0,0)
		else{ only_pdx.sign=-1;} //possible pd-codes:: (0,1,1,0) / (1,0,0,1)
		return;
	}
	else{//note: no additional checking for correctness of pd-code here
		
		for(auto& it : pd_crossings){

			if( (it.second.get_arm_label(3) + 1) % (max_segment_no+1) == it.second.get_arm_label(1)){ //(x4+1) mod N == x2 in (x1, x2, x3, x4)
					it.second.sign = 1;
			}
			//if not positive, set as negative
			else{ it.second.sign = -1;
			}
		}
		return;
	}}
void Diagram::calculate_segment_to_pdxs_mapping(){

	//initialize `segments_to_pdx` with dummy crossing ids
	array<int,2> dummy_value = {-1,-1};
	for(int n=0; n<=max_segment_no; n++){
		segments_to_pdxs.insert({n, dummy_value});
	}

	//add segment_no->pdx_id mapping; segment_no: [pdx1, pdx2], where pdx1 has segment as outgoing, pdx2 has segment as incoming
	for(auto& pair : pd_crossings){
		array<int,2> outcoming_segs = pair.second.give_outgoing_segment_numbers();
		array<int,2> incoming_segs = pair.second.give_incoming_segment_numbers();

		for(int seg_no : outcoming_segs){
			assert(segments_to_pdxs[seg_no][0] == -1);
			segments_to_pdxs[seg_no][0] = pair.first;
		}
		for(int seg_no : incoming_segs){
			assert(segments_to_pdxs[seg_no][1] == -1);
			segments_to_pdxs[seg_no][1] = pair.first;
		}
	}
	return;}
/* ************************************************************ */



void Diagram::print()const{

	cout << endl << string(60, '-') << endl;
	cout << "Printing diagram..." << endl;
	for(const auto& pair : pd_crossings){
		int key = pair.first;
		const PD_X &pdx = pair.second;

		cout << "No. " << key << ":   ";
		pdx.print();
	};
	cout << "...printing diagram finished." << endl;
	cout << string(60, '-') << endl;}
void Diagram::print_segment_to_pdx_mapping()const{
	cout << "Segments to pdxs mapping:" << endl;
	for(const auto& pair : segments_to_pdxs){
		cout << pair.first << " : [" << pair.second[0] << ", " << pair.second[1] << "]" << endl; 
	}}



/* **************************************************************************************************************************************************** */
/* *******    coloring diagram    ********************************************************************************************************************* */
int Diagram::get_segment_color(int seg_no, int side)const{
	assert(abs(side)==1);
	assert(0<= seg_no && seg_no <= max_segment_no);
	return  (side==-1) ? segment_colors.at(seg_no).at(0) : segment_colors.at(seg_no).at(1);}
void Diagram::set_segment_color(int color, int seg_no, int side){
	//cout << "In: void Diagram::set_segment_color(int color, int seg_no, int side)" << endl;
	//cout << "(color, seg_no, side)=(" << color << ", " << seg_no << ", " << side << ")" << endl; 
	assert(segment_colors.find(seg_no) != segment_colors.end());
	assert(color > -1);
	assert(abs(side)==1);
	assert( ! this->is_segment_side_colored(seg_no,side) );

	int i = (side==-1) ? 0 : 1;
	segment_colors.at(seg_no).at(i)=color;
	/* else{
	 	cout << "\tTrying to set color " << color << " on " << side << " side of segment " << seg_no << " but segment already colored with color " << this->get_segment_color(seg_no,side) << "." << endl;
	 	cout << "\tSet override=true in Diagram::set_segment_color to override" << endl;
	 }*/
	return;}

TwoSets<int> Diagram::color_face_boundary(const int color, const int start_seg, const int start_seg_side, const ColorMoves &colorMvsArr){
	//cout << endl << "----- Starting coloring new face boundary -----" << endl;

	TwoSets<int> colored_segments; //segments from boundary; first set: labels of segments having face on the left, second set: having face on the right
	/* ************************************************************** */
	// 1. Go to the START segment
	// 2. Look on the appropriate side, ensure that side is uncolored
	// 3. no [was: Set desired color on current side of START segment]
	// 4. Find PDX containing this segment, set start state based on this pdx
	//	  (PDX_sign, direction, segment side, arm position (xi) in crossing)
	// 5. Get next move:
	//		- if: "find the second crossing with this segment":
	//			 - find next PDX with this segment (update sign and arm position, don't change direction/side)
	//		- else ("change arm inside current crossing"):
	//			 - change arm coordinate, don't change crossing sign, change direction/side if move says so
	//	  		 - numer segmentu się zmienia --> pokoloruj opuszczany
	/* ************************************************************** */

	int current_seg;
	int pdx_id;

	/* state variables */
	int pdx_sign;
	int direction;
	int seg_side;
	int xi_pos;
	array<int,2> new_pdx_new_pos; //[pdx_id, xi_pos], xi_pos \in {0,1,2,3}


	direction = 1;
	current_seg = start_seg;
	seg_side = start_seg_side;

	/* ************ debugging *************************************** */
		// cout << "Start segment: " << start_seg << endl;
		// cout << "Start side: " << start_seg_side << endl;
		// cout << "Start color --> on start side of the start segment (before the 'do' loop): " << this->get_segment_color(start_seg, start_seg_side) << endl;
	/* ******************************************************* */

	pdx_id = segments_to_pdxs.at(start_seg).at(0); //pdx with this segment as outcoming
	pdx_sign = pd_crossings.at(pdx_id).get_sign();
	xi_pos = pd_crossings.at(pdx_id).return_segment_position_in_pdx(start_seg, "outgoing"); //
	//if //handle the case of xi==-1 (segment 2x in this pdx)

	/* ************ debugging *************************************** */
		//cout << "Start PDX xi position: " << xi_pos << endl;
	/* ******************************************************* */
	assert(xi_pos != -1);
	const int start_xi_pos = xi_pos;
	const int start_pdx_id = pdx_id;
	array<int,4> state = {pdx_sign, direction, seg_side, xi_pos};
	array<int,3> move; //(next_xi, direction_coef, side_coef) //note: always direction_coef==side_coef, to change in color_moves later


	/* Traverse face boundary and color its segments (on their appropriate sides) until you reach start segment again */
	do{


		move = colorMvsArr.get_move_by_state(state);

		
		if(move[0]==-1){ /* Case 1: find next PDX with this segment ("next xi" in move is "-1") [note: this may be the same segment, if Reidemeister I loop in pdx]*/
			

			/* Color current segment */
			this->set_segment_color(color, current_seg, seg_side);
			colored_segments.add_element(current_seg, seg_side);


			/* Find second PDX crossing sharing current segment number (this may be the same PDX in Reidemeister type I twist case) */
			new_pdx_new_pos = this->get_second_pdx_with_seg_no_and_seg_pos(current_seg, pdx_id, xi_pos);
			pdx_id = new_pdx_new_pos[0];
			xi_pos = new_pdx_new_pos[1];
			pdx_sign = pd_crossings.at(pdx_id).get_sign();
			
		}
		else{ /* Case 2: stay in pdx, change arm (and side/direction if move says so) */

			/* Change arm within current pdx, switch direction and side if appropriate */
			xi_pos = move[0];
			direction *= move[1];
			seg_side *= move[1];
			
			/* Get segment number for new current arm */
			current_seg = pd_crossings.at(pdx_id).get_arm_label(xi_pos);

		}
		
		/* Update state based on new values */
		state = {pdx_sign, direction, seg_side, xi_pos};

	}while( ! ((current_seg==start_seg) && (pdx_id==start_pdx_id) && (xi_pos==start_xi_pos) && (seg_side==start_seg_side)) ); //note: is segment number needed if the rest is checked?

	assert(colored_segments.size()>0);
	return colored_segments;}

void Diagram::color_diagram(){
	//int safe_escape=0;


	cout << endl << "Coloring diagram..."<<endl;

	/* Initialize color moves map*/
	ColorMoves color_moves_map = ColorMoves(); //note: może to przenieśc jako atrybut


	/* Initialize auxiliary sets containing "todo" segments */
	unordered_set<int> todo_segments_left;
	unordered_set<int> todo_segments_right;

	todo_segments_left.reserve(segments_to_pdxs.size());
	todo_segments_right.reserve(segments_to_pdxs.size());

	for(int k=0; k<=max_segment_no; k++){
		todo_segments_left.insert(k);
		todo_segments_right.insert(k);
	}
	/* ********************************** */



	/* while there exist todo segments:
		1. pick one segment and its uncolored side
		2. apply "color diagram face" starting from chosen side of chosen segment
		3. remove colored segments from todo lists */
	int color = -1;
	while( ! (todo_segments_left.empty() && todo_segments_right.empty()) ){
		//cout << endl << "-------- Starting new color ---------" << endl;



		/*
		cout << string(60, '-') << endl;
		cout << "\tTodo segments left: ";
		for(const int k : todo_segments_left){cout << k << " ";}
		cout << "\n\tTodo segments right: ";
		for(const int k : todo_segments_right){cout << k << " ";}
		cout << endl << endl;
		*/

		++color;
		int start_segment;
		int segment_side;
		//cout << "Current color (new): " << color << endl;

		/* get segment and side */
		segment_side = (! todo_segments_left.empty()) ? -1 : 1;
		unordered_set<int>& segments = (segment_side==-1) ? todo_segments_left : todo_segments_right;
		start_segment = *segments.begin();
		//cout << "Start segment number: " << start_segment << endl;
		//cout << "Start segment side: " << segment_side << endl << endl;


		/* Color face boundary (and collect segments from boundary)*/
		TwoSets<int> colored_segments = this->color_face_boundary(color, start_segment, segment_side, color_moves_map);


		/* remove done segments from "todo" containers */

		/*
		cout << "\tDone segments left: ";
		for(const int k : colored_segments.left){cout << k << " ";}
		cout << "\n\tDone segments right: ";
		for(const int k : colored_segments.right){cout << k << " ";}
		cout << endl << endl;
		*/

		for (const auto& seg : colored_segments.left){
			assert(todo_segments_left.find(seg) != todo_segments_left.end());
    		todo_segments_left.erase(seg);
    	}
		for (const auto& seg : colored_segments.right){
			assert(todo_segments_right.find(seg) != todo_segments_right.end());
    		todo_segments_right.erase(seg);
    	}
    	//cout << string(60, '-') << endl;
	}
	cout << "...coloring diagram finished."<<endl;}






void Diagram::initialize_diagram_colors(){
	array<int,2> uncolored = {-1,-1};
	for(int k=0; k<=max_segment_no; k++){ segment_colors[k] = uncolored;}
	return;}
// void Diagram::reset_diagram_colors(){
// 	array<int,2> uncolored = {-1,-1};
// 	for(int k=0; k<=max_segment_no; k++){ segment_colors[k] = uncolored;}
// 	return;}
/* **************************************************************************************************************************************************** */


/* ************************************************************ */
/* ***    misc      ******************************************* */
array<int,2> Diagram::get_second_pdx_with_seg_no_and_seg_pos(int seg_no, int pdx1_id, int current_xi_pos)const{
	//cout << "In: array<int,2> Diagram::get_second_pdx_with_seg_no_and_seg_pos(int seg_no, int pdx1_id, int current_xi_pos)const" << endl;
	array<int,2> seg_pdxs = segments_to_pdxs.at(seg_no);
	int n_pdx1 = count(seg_pdxs.begin(), seg_pdxs.end(), pdx1_id);
	assert( n_pdx1 ==1 || n_pdx1==2);
	
	int pdx2_id; //second pdx with this segment (may be pdx1 -> if Reidemeister I twist in pdx1)
	int xi_pos;

	if(n_pdx1==1){
		pdx2_id = (seg_pdxs[1]==pdx1_id) ? seg_pdxs[0] : seg_pdxs[1];
		xi_pos = pd_crossings.at(pdx2_id).return_segment_position_in_pdx(seg_no);
	}
	else{
		pdx2_id = pdx1_id;
		int smaller_xi_pos = pd_crossings.at(pdx2_id).return_first_segment_occurence_in_pdx(seg_no);
		if( smaller_xi_pos == current_xi_pos){ /*if current arm has smaller xi position, return the higher one and vice versa*/
			//cout << "\t-in branch: pd_crossings.at(pdx2_id).return_last_segment_occurence_in_pdx(seg_no) == current_xi_pos" <<endl;
			xi_pos =  pd_crossings.at(pdx2_id).return_last_segment_occurence_in_pdx(seg_no);
			//cout << "\t xi_pos to be returned: " << xi_pos <<endl;
		}
		else{
			//cout << "\t-in branch: pd_crossings.at(pdx2_id).return_first_segment_occurence_in_pdx(seg_no) != current_xi_pos" <<endl;
			xi_pos =  pd_crossings.at(pdx2_id).return_first_segment_occurence_in_pdx(seg_no);
			//cout << "\t xi_pos to be returned: " << xi_pos <<endl;
		}
	}
	
	return {pdx2_id, xi_pos};}
/* ************************************************************ */

