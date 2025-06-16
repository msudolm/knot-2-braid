#ifndef GRAPH_H
#define GRAPH_H

#include "CrossingsCollection.h"
#include "EdgesCollection.h"
#include "ColorsCollection.h"

#include "../parser/PD_X.h"
#include "../parser/Diagram.h"

#include <unordered_set>
#include <vector>

using namespace std;


class Graph{

public:
	ColorsCollection graph_colors;
	EdgesCollection graph_edges;
	CrossingsCollection graph_crossings;
	
	Graph(Diagram &colored_diagram);



	// Braid-related operations
	vector<int> read_braid_word();
	void transform_to_braid_form();
	bool is_in_braid_form(){ return (graph_colors.colors_todo.size() == 0);}
	

	// Elementary operation
	array<int,3> find_admissible_segments_starts_and_side();
	void perform_move();


	// Accessors - elements from collections by key
	Color* get_color_ptr_by_key(int color_id){return graph_colors.get_ptr_by_key(color_id);};
	Edge* get_edge_ptr_by_key(int edge_id){return graph_edges.get_ptr_by_key(edge_id);};
	Crossing* get_crossing_ptr_by_key(int crossing_id){return graph_crossings.get_ptr_by_key(crossing_id);};
	Crossing* get_anchor_crossing_ptr_for_edge_key(int edge_id){return this->get_crossing_ptr_by_key( (this->get_edge_ptr_by_key(edge_id))->anchor_crossing_id); };
	
	
	// Printing
	void print_graph();
	void print_edge_crossing_by_crossing(int edge_id);


	// Traversing graph edges upward
	int give_outcoming_neigbour_id(int edge_id);



private:

	//? visibility?
	void set_crossing_color(int crossing_id, int color, int side, bool enable_override=false); //note: add int direction=1? or not?



	//Find admissible pair of segments
	Color* give_mv_color();
	array<int, 2> give_mv_edges_ids();
	array<Crossing*, 2> give_mv_segments_starts();


	// Elementary operation
	void join_add_and_recolor_edges(int cx1_id, int cx2_id, int mv_side); //PERFORMING MOVE (creating new crossings, joining edges, creating new edge, changing colors)

	// Join and recolor edges (on ColorsCollection level)
	void absorb_e2_by_e1_in_single_colorside(int e1_id, int e2_id, int color_id, int side);
	void absorb_e2_by_e1_in_colors_collection(int e1_id, int e2_id);

		// 1. Generate new crossings
		array<Crossing*, 4> generate_new_crossings_linked_and_conjugated(int cx1_id, int cx2_id, int e1_id, int new_e_id);


		// 2. Recolor fragments of edges (on Crossings and ColorsCollection level)
		void perform_small_recolorings(int mv_side, int cx1_id, int cx2_id, array<Crossing*,4> x1_y1_x2_y2, int newedge_color_id);
		void perform_big_recoloring(int start_cx_id, int start_side, int mv_color, int new_color);

		// void perform_big_recoloring_test_v2_start_B1(int start_cx_id, int mv_side, int mv_color, int new_color);




	//Return bottom and top edge of a final chain graph (as EdgesCollection identifiers)
	int get_key_of_bottom_chain_edge();
	int get_key_of_top_chain_edge();

};

#endif