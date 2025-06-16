#include "ColorsCollection.h"
#include <iostream>



using namespace std;


ColorsCollection::~ColorsCollection(){
	for (auto& entry : colors) {
            delete entry.second;
        }
        colors.clear();
}


void ColorsCollection::actualize_todo_done_color_status(int color_id){
	int is_done = (this->get_ptr_by_key(color_id))->is_color_done();
	unordered_set<int> &insert_set = is_done ? colors_done : colors_todo;
	unordered_set<int> &erase_set = is_done ? colors_todo : colors_done;

	erase_set.erase(color_id);
	insert_set.insert(color_id);}


/*Function description:
	This function creates new Color object with color number == new_id, where new_id := size of ColorsCollection
	ads {new_id, new_color_ptr} to ColorsCollection
	and returns new_id.
	*/
int ColorsCollection::add_new_empty_color_return_id(){ //old name: generate_new_color
	int new_id = colors.size();
	assert(colors.find(new_id)==colors.end());

	Color* c = new Color(new_id);
	colors.insert({new_id, c});
	actualize_todo_done_color_status(new_id);
	return new_id;}

//note: get rid of this function (check usage first)
Color* ColorsCollection::add_new_empty_color(int color_nr){
	assert(colors.find(color_nr) == colors.end());
	// assert(colors_todo.find(key) == colors_todo.end());
	// assert(colors_done.find(key) == colors_done.end());

	//create new color, add to todo collection, return pointer
	Color* c = new Color(color_nr);
	colors.insert({color_nr, c});
	actualize_todo_done_color_status(color_nr);
	return c;}



// Adds information about Crossing object from edge with id=seifert_id
// to two appropriate colors of CrossingsCollection (creates a new color / edge key on color if not exists)
void ColorsCollection::add_crossing_info_to_collection(Crossing& crossing){

	//add information from the left side
	this->add_side_crossing_info_to_collection(crossing, -1);
	//add information from the right side
	this->add_side_crossing_info_to_collection(crossing, 1);}


/*
Method adds color id from given side of passed Crossing to Colorscollection.
Craetes new color or new edge-key on existing color if necessary (not present before)
*/
void ColorsCollection::add_side_crossing_info_to_collection(Crossing& crossing, int side){
	assert(abs(side)==1);
	int col_nr = crossing.get_color(side);

	//cout << "\t\tadding Crossing " << crossing.name << " info to ColorsCollection; (color, side, edge_id, crossing_id)=(" << col_nr << ", " << side << ", " << crossing.edge_id << ", " << crossing.crossing_id << ")." << endl; 

	if( !this->is_color_in_collection(col_nr) ){this->add_new_empty_color(col_nr);}
	(colors[col_nr])->add_crossing_info_to_color(crossing, crossing.edge_id);
	this->actualize_todo_done_color_status(col_nr); //move color between todo and done if status changed after adding crossing
}


void ColorsCollection::print_colors_collection()const{

	// note: maybe add division to todo-done
	cout << "---------- Colors Colletion -----------" << endl;
	cout << "Size of colors_todo: " << colors_todo.size() << endl;
	cout << "Size of colors_done: " << colors_done.size() << endl;
	cout << "Number of colors in collection: " << colors.size() << endl;




	if(colors.size() == 0){cout << endl <<"-----------------------------" << endl << "Colors Collection is empty." << endl;}
	else{
		//iterate over colors in collection
		for(const auto& pair :  colors){
			cout <<"-----------------------------" << endl;
			cout << "Color id in collection: " << pair.first << ". " << endl;
			pair.second->print_color();
		}
	}
	cout << "-----------------------------" << endl;}




array<int,4> ColorsCollection::give_todo_color_side_and_edges(){
	assert(colors_todo.size() > 0);

	Color* c;
	int c_id, side;
	array<int,2> e1_e2;

	//get any color id from colors_todo set
	c_id = *(colors_todo.begin());
	c = colors.at(c_id);

	side = (c->size_left() > 1) ? -1 : 1; //note: można zmienić sposób wybierania strony; np losować jeżeli obie > 1
	e1_e2 = c->give_two_mv_edge_ids(side);

	return {c_id, side, e1_e2[0], e1_e2[1]};}


array<int,2> ColorsCollection::give_mv_crossing_ids(int col_id, int side, int e1_id, int e2_id){
	assert(abs(side) == 1);
	assert(colors.find(col_id) != colors.end()); //sanity check: color id in collection
	Color* c = get_ptr_by_key(col_id);
	array<int,2> c1_c2_ids = c->give_two_mv_crossing_ids(side, e1_id, e2_id);
	return c1_c2_ids;}




void ColorsCollection::add_crossing_id_to_color_side(int color_id, int color_side, int edge_id, int cx_id){
	get_ptr_by_key(color_id)->add_crossing_id_to_colorside(color_side, edge_id, cx_id);
	actualize_todo_done_color_status(color_id);}
void ColorsCollection::remove_crossing_id_from_color_side(int color_id, int side, Crossing* c){
	assert(abs(side)==1);
	Color* col = get_ptr_by_key(color_id);
	col->remove_crossing_id_from_colorside(c, side);
	//assert(col->num_edges() > 0); //note: maybe another handling later (but: there should always be at least one edge on a color in normal run (eg. no force color substitutions))
	//note: może tak byc, jeżeli kolor tylko wyłączny krawędzi, która właśnie się usuwa (dodać odpowiednią asercję tu później ewentualnie (uwzględniającą to))
	actualize_todo_done_color_status(color_id);
	return;}



void ColorsCollection::move_crossing_id_between_colors(Crossing* c, int side, int new_color){
	//cout << "\t\tIn: move_crossing_id_between_colors; crossing_id: " << c->crossing_id << ", side: " << side << ", new_color: " << new_color << ", old color: " << c->get_color(side) << endl;
	assert(abs(side)==1);

	int cx_id = c->crossing_id;
	int edge_id = c->edge_id;
	int old_color_id = c->get_color(side);

	//remove crossing id from requested side of old Color
	this->remove_crossing_id_from_color_side(old_color_id, side, c);
	this->add_crossing_id_to_color_side(new_color, side, edge_id, cx_id);
	//assigning to todo-dane managed in above functions
	return;
}



//note: maybe delete this one..? and color always assigned if modified / created
/*Loops over all colors in collection and assigning their ids either to
 "done" list (if they have at most one leftside edege and at most one rightside edge)
 or to "todo" list (otherwise).

 Time complexity: O(n), n-number of colors in collection
 */
void ColorsCollection::assign_to_todo_done(){
	for(const auto& key_color : colors){
		if( (key_color.second)->is_color_done()){colors_done.insert(key_color.first);}
		else{colors_todo.insert(key_color.first);}
	}
}

