#ifndef COLORS_COLLECTION_H
#define COLORS_COLLECTION_H


#include <unordered_map>
#include <unordered_set>
#include <array>

#include "ColorsCollection.h"
#include "Color.h"


using namespace std;



class ColorsCollection{
public:
	unordered_map<int, Color*> colors; //note: zmienić na unique_ptr później
	unordered_set<int> colors_todo; //[collection of uniquepointers to Color objects] //note: zmienić na unique_ptr później
	unordered_set<int> colors_done; //[collection of uniquepointers to Color objects] //note: zmienić na unique_ptr później
	//maybe add keeping colors of bottom and top edges..?


	//ColorsCollection();
	~ColorsCollection();
	int size(){return colors.size();}

	Color* get_ptr_by_key(int color_id){return colors.at(color_id);}
	//void add_existing_color(Color* p_color);
	bool is_color_in_collection(int color_id){return ( colors.find(color_id) != colors.end() );}

	int add_new_empty_color_return_id();
	Color* add_new_empty_color(int color_nr);

	void add_crossing_info_to_collection(Crossing& crossing); //old name: add_cx_info_to_collection(Crossing* c) //note const ref? //note: maybe change name to: add colors from crosing
	void add_side_crossing_info_to_collection(Crossing& crossing, int side); //note: const ref?


	void actualize_todo_done_color_status(int color_id);
	void assign_to_todo_done(); //note: uzupełnić //note: może w ogóle tego nie używać, tylko wrzucać przy tworzeniu wszystkie? (a później i tak się zarządza na bieżąco)

	void print_colors_collection()const; //note: change prints to const methods
	void print_colors_todo()const;
	void print_colors_done()const;


	

	array<int,4> give_todo_color_side_and_edges();
	//array<Crossing*,2> give_todo_crossing_ptrs(int col_id, int side, int e1_id, int e2_id); //REMOVE LATER (CHANGE FOR VERSION RETURNING IDS)
	array<int,2> give_mv_crossing_ids(int col_id, int side, int e1_id, int e2_id);

	//note: może zmienić te metody przenoszące id na takie, które biora Crossing* jako argument i stronę 
	void move_crossing_id_between_colors(Crossing* c, int side, int new_color);
private:
	void add_crossing_id_to_color_side(int color_id, int color_side, int edge_id, int cx_id);
	void remove_crossing_id_from_color_side(int color_id, int side, Crossing* c);
};


#endif