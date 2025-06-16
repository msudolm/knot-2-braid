#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <string>
#include <array>
#include <unordered_map>
#include <unordered_set>


#include "PD_X.h"
//#include "color_moves.h"
#include "ColorMoves.h"

using namespace std;
//note: zaimporować mapę kolorów


template <typename T>
struct TwoSets {
    unordered_set<T> left;
     unordered_set<T> right;

    void add_element(int element, int side){
    	assert(abs(side)==1);
    	if(side==-1){left.insert(element);}
    	else{right.insert(element);}
    }

    int size(){
    	return left.size()+right.size();
    }
};



/* Class for representing diagram read from input PD-file */

class Diagram{


	
public:
	

	//sets of PD-crossings that haven't been assigned to Seifert circles yet
	//(each side (left/right) of a PD-crossing should be assigned to one (and different for each side) Seifert circle at the end of Seifert circle decomposition of the diagram)
	//unordered_set<int> seifert_unassigned_pdxs_right;
	//unordered_set<int> seifert_unassigned_pdxs_left;


	Diagram(const string &pd_filename);
	//~Diagram();

	void print()const;
	void print_segment_to_pdx_mapping()const;


	int get_segment_color(int seg_no, int side)const;
	void set_segment_color(int color, int seg_no, int side); //, bool override=false);
	bool is_segment_side_colored(int seg_no, int side)const{return this->get_segment_color(seg_no,side) > -1;};

	int get_pdxid_with_segment_as_incoming_arm(int seg_no)const{return segments_to_pdxs.at(seg_no).at(1);};

	void initialize_diagram_colors();
	void color_diagram(); //note: maybe change to perform_diagram_coloring later
	void reset_diagram_colors();




private:
	int max_segment_no; //max segment number from PD-code file;
	unordered_map<int,PD_X> pd_crossings;
	unordered_map<int,array<int,2>> segment_colors;
	unordered_map<int, array<int,2>> segments_to_pdxs; //note: initialize and define this in constructor

	friend class Graph;


	void compute_crossing_signs();
	void calculate_segment_to_pdxs_mapping();
	//void color_diagram_face(int start_cx_id, int start_xi, int start_side, int start_direction, const int color, const ColorMoves &colorMvsArr);
	//unordered_set<int> uncolored_pdxs;
	array<int,2> get_second_pdx_with_seg_no_and_seg_pos(int seg_no, int pdx1, int current_xi_pos)const; //int get_second_pdx_with_seg_no(int seg_no, int pdx1)const;

	TwoSets<int> color_face_boundary(const int color, const int start_seg, const int start_seg_side, const ColorMoves &colorMvsArr);
	


};


#endif