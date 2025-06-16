#ifndef CROSSING_H
#define CROSSING_H


#include <string>
#include <iostream>
#include <array>

#include "../parser/PD_X.h"

using namespace std;


struct PD_X;


class Crossing{

public:
	int crossing_id;
	int edge_id;

	int sign; // +1 or -1
	Crossing* next_crossing;
	Crossing* prev_crossing;
	Crossing* conjugate_crossing;

	int left_color;
	int right_color;
	string name; // for testing and sanity checks only



	Crossing(int sign, int cx_id);
	Crossing(PD_X &pdx, int pdx_side, int new_cx_id=-1, int edg_id=-1);
	//~Crossing();

	void print_crossing();

	// Accessors
	int get_crossing_id()const{return crossing_id;};
	int get_color(int side, int direction=1); //note: tu dodać obsługe przypadku, kiedy skrzyżowanie nie ma nikogo za sobą
	void set_color(int color, int side, int direction=1);
	void set_edge_id(int edg_id){edge_id=edg_id;};
	void set_conjugate(Crossing* conjugate_ptr){conjugate_crossing=conjugate_ptr;};
	void set_next_ptr(Crossing* next_ptr){ next_crossing=next_ptr;};
	void set_prev_ptr(Crossing* prev_ptr){ prev_crossing=prev_ptr;};
	
	void elongate_color_from_neigbour(int direction, int side);
	Crossing* give_neigbour_crossing(int direction){return ((direction == 1) ? next_crossing : prev_crossing);} //returns the neigbouring crossing from self on the Seifert circle (may return self)

	int give_conjugate_edge_id(){return conjugate_crossing->edge_id;};


	bool check_if_conjugate_continues_traversal_color(int current_direction, int current_side, int color); //side: l=-1, r=1
	Crossing* return_next_crossing_following_color(Crossing* prev, int* direction, int* side); //note: maybe better name would be: return_next_crossing_on_color_edge(border) ?
	void traverse_color(int start_direction, int start_side); //note: maybe better name would be: traverse ..._border(edge) (diagram border, color area border)
	void traverse_color_and_change_it(int start_direction, int start_side, int old_color, int new_color); //note: maybe better name would be: traverse_border_and_change_color

};


#endif
