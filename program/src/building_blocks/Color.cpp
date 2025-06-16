#include "Color.h"
#include <iostream>

using namespace std;





// //note: change to passing by ref everywhere and delete
// unordered_map<int, unordered_set<int>>* Color::give_sideseiferts_ptr(int side){
// 	assert(abs(side)==1);
// 	unordered_map<int, unordered_set<int>>* sideseiferts_ptr = (side==-1) ? &leftside_seiferts : &rightside_seiferts;
// 	return sideseiferts_ptr;}

unordered_map<int, unordered_set<int>> &Color::give_sideseiferts_ref(int side){
	assert(abs(side)==1);
	unordered_map<int, unordered_set<int>> &sideseiferts_ref = (side==-1) ? leftside_seiferts : rightside_seiferts;
	return sideseiferts_ref;}

void Color::print_color(){
	cout << "Color id: " << color_id << endl;

	cout << "\tLeft side: " << endl;
	print_color_half(-1);

	cout << endl;
	cout << "\tRight side: " << endl;
	print_color_half(1); }

bool Color::is_edge_in_side(int e_id, int side){
	assert(abs(side) == 1);
	unordered_map<int, unordered_set<int>> &sideseiferts = give_sideseiferts_ref(side);
	return (sideseiferts.find(e_id) != sideseiferts.end());}







array<int,2> Color::give_two_mv_edge_ids(int side){
	assert(abs(side) == 1);
	unordered_map<int, unordered_set<int>> &sideseiferts = this->give_sideseiferts_ref(side);
	assert(sideseiferts.size() >= 2);

	//pick two edge ids on requested side of Color
	array<int,2> e1_e2;
	auto it = sideseiferts.begin();
	e1_e2[0] = it->first;
	++it;
	e1_e2[1] = it->first;

	return e1_e2;}
array<int,2> Color::give_two_mv_crossing_ids(int side, int e1_id, int e2_id){
	assert(abs(side) == 1);

	unordered_map<int, unordered_set<int>> &sideseiferts = this->give_sideseiferts_ref(side);
	assert(sideseiferts.size() >= 2); //sanity check: at least two edges on this side of color
	assert( (sideseiferts.at(e1_id).size() > 0) && (sideseiferts.at(e2_id).size() > 0)); //at least one crossing on each edge

	/* Pick one crossing id from each edge */
	array<int,2> c1_c2_ids;
	auto it = sideseiferts.at(e1_id).begin();
	c1_c2_ids[0] = *it;
	it = sideseiferts.at(e2_id).begin();
	c1_c2_ids[1] = *it;

	return c1_c2_ids;}




void Color::add_crossing_info_to_color(Crossing &crossing, int edge_id){ //note:  może zamienić na --> add crossing id to color (i sam wskaźnik Crossing albo same dwa inty?)
	assert( (crossing.left_color == color_id) != (crossing.right_color == color_id) ); //xor
	
	/* Get Color side corresponding to the color occurrence in Crossing (note: given color can occur at most on one side of crossing) */
	unordered_map<int, unordered_set<int>> &sideseiferts = (crossing.left_color == color_id) ? leftside_seiferts : rightside_seiferts;
	sideseiferts[edge_id].insert(crossing.crossing_id);}


void Color::add_crossing_id_to_colorside(int color_side, int edge_id, int cx_id){
	unordered_map<int, unordered_set<int>> &sideseiferts = this->give_sideseiferts_ref(color_side);
	if(sideseiferts.find(edge_id) == sideseiferts.end()){sideseiferts.insert( {edge_id, {}} );}; //add edge_id key to color side if not present
	sideseiferts.at(edge_id).insert(cx_id);/*add crossing id under edge_id key*/}

//note: czy to byłoby czyściej, gdyby wszystkie te color/edge/crossing id przkazywać bezpośrednio, nie przez wskaźnik na skrzyźowanie?
void Color::remove_crossing_id_from_colorside(Crossing* c, int side){
	assert(abs(side)==1);

	int color_id = c->get_color(side);
	int edge_id = c->edge_id;
	int cx_id = c->crossing_id;

	//remove color id from ids assigned to this edge on this side of the color
	unordered_set<int> &edge_crossings = give_sideseiferts_ref(side).at(edge_id); //note: ensure if this throws an error if key not present
	edge_crossings.erase(cx_id);

	//if no more crossings left, remove edge id from this side of the color
	if(edge_crossings.size()==0){
		//cout << "\tNo more (" << color_id << ") color on the (" << side << ") side of the (" << edge_id << ") edge;" << endl;
		//cout << "\tremoving edge (" << edge_id << ") from (" << side << ") of (" << color_id << ") color." << endl;

		remove_edge_from_colorside(edge_id, side);

	} 
	return;}


//##################################################################################################################
// private methods implementations
//##################################################################################################################

void Color::print_color_half(int side){

	//iterate over sideseiferts collection of requested side
	for(const auto& seif_cxset : give_sideseiferts_ref(side) ){
		//print edge id
		cout << "\t\tEdge " << seif_cxset.first << ":   ";
		//print subsequent crossing ids from this edge
		for(const auto& cxid_it : seif_cxset.second){
			cout << cxid_it << "   ";
		}
		cout << endl;

	}}

//remove 
void Color::remove_edge_from_colorside(int edge_id, int side, bool rm_only_empty){
	assert(abs(side)==1);

	//get edge: {crossings} collection from requested side of color
	unordered_map<int, unordered_set<int>> &sideseiferts = (side==1) ? rightside_seiferts : leftside_seiferts;
	assert(sideseiferts.find(edge_id) != sideseiferts.end());

	//we don't want to remove non-empty edges during the program run
	try{
		int rm_size = (sideseiferts.at(edge_id)).size();
		if( rm_only_empty && rm_size>0 ){ throw rm_size; }
	}
	catch(int rm_size){
		cout << "You are trying to remove an edge with not-empty crossings collection on " << side << " side of a color." << endl;
		cout << "Color: (" << color_id << "), Edge: (" << edge_id << "); number of crossing of color (" << color_id << ") on side (" << side << ") on Edge (" << edge_id << ") is: " << rm_size << endl;
		cout << "Exiting the program." << endl << endl;
		exit(1);
	}

	//cout << "\tRemoving edge (" << edge_id << ") form (" << side << ") of Color (" << color_id << ")." << endl;
	sideseiferts.erase(edge_id);
	return;}
