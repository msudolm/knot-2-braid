#include <unordered_map>
#include <unordered_set>

#include "Crossing.h"

using namespace std;




class Color{
public:
	int color_id;
	//collection of pairs: (edge_id: {crossings from this edge that have this color on particular_side})
	unordered_map<int, unordered_set<int>> leftside_seiferts; //
	unordered_map<int, unordered_set<int>> rightside_seiferts; //
	//int max_lr; //max{left_seiferts, right_seiferts} <-- for dedecting if in braid form (if max_s'es for all colors == 1)


	Color(int color_id): color_id(color_id) {} //, max_lr(0){}
	// unordered_map<int, unordered_set<int>>* give_sideseiferts_ptr(int side); //
	unordered_map<int, unordered_set<int>>& give_sideseiferts_ref(int side);

	bool is_color_done(){ return ( this->max_side_size() == 1); }
	bool is_edge_in_side(int e_id, int side);

	void print_color();
	int size_left(){return leftside_seiferts.size();}
	int size_right(){return rightside_seiferts.size();}
	int max_side_size(){return max(this->size_left(), this->size_right());}
	int num_edges(){return (this->size_left() + this->size_right());};


	//adding and removing crossings
	void add_crossing_info_to_color(Crossing &crossing, int edge_id); //old name: add_cx_info_to_color, add_crossing_info_to_color
	void add_crossing_id_to_colorside(int color_side, int edge_id, int cx_id); //old name: add_cx_id_to_color_side
	void remove_crossing_id_from_colorside(Crossing* c, int side);
	
	//searching for admissible segments
	array<int,2> give_two_mv_edge_ids(int side);
	array<int, 2> give_two_mv_crossing_ids(int side, int e1_id, int e2_id); //

	void absorb_e2_by_e1(int e1_id, int e2_id, int side);



private:
	void print_color_half(int side);
	void remove_edge_from_colorside(int edge_id, int side, bool rm_only_empty=true);



};