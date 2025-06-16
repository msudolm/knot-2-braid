#include "Edge.h"


using namespace std;



//private
// void Edge::add_color_id(int color_nr, int side){
// 	assert(abs(side)==1);
// 	unordered_set<int> &colors_side = (side ==-1 ) ? colors_left : colors_right;
// 	colors_side.insert(color_nr);
// }






//public
void Edge::set_anchor(int cx_id){
	assert(anchor_crossing_id == -1 && "anchor crossing is already set");
	anchor_crossing_id = cx_id;
}


void Edge::print_edge(){
	cout << "Edge id: " << edge_id << endl;
	cout << "anchor crossing id: " << anchor_crossing_id << endl;
}

// void Edge::add_colors_ids_from_crossing(Crossing &cx){
// 	this->add_color_id(cx.left_color, -1);
// 	this->add_color_id(cx.right_color, 1);
// }