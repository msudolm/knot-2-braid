#ifndef PD_X_H
#define PD_X_H

#include <array>
#include "../building_blocks/Crossing.h"

using namespace std;

class Crossing; 

//structure for representing single PD-crossing and its additional information (color / seifert circle)
struct PD_X{
//note: czy nie lepiej by było mieć coś, co trzyma kolory odcinków osobno, a pd-skrzyżowania się do tego odwołują? (wtedy nie trzeba kolorować dwa razy każdego odcinka)

public:

	int sign; //pd-crossing sign
	array<int,4> xs; //segment numbers (from pd-file)
	array<int, 4> left_colors; //colors on left side of x1,x2,x3,x4
	array<int, 4> right_colors; // colors on right side of x1,x2,x3,x4
	

	Crossing* p_first_conjugate_holder;
	bool both_conjugates_done;

	PD_X();
	PD_X(const array<int,4> &xs);
	//note: destructor -> define later
	//~PD_X();
	void print() const;

	int get_sign()const{return sign;};
	int get_arm_label(int xi_pos)const{return xs.at(xi_pos);}; //was: get_segment_no
	int get_arm_color(int side, int pos) const;
	void set_arm_color(int color, int side, int pos);

	array<int,2> get_outgoing_xi_positions()const;

	bool is_segment_number_in_pdx(int seg_no)const{return find(begin(xs), end(xs), seg_no) != xs.end();};
	int return_number_of_segment_occurences_in_pdx(int seg_no)const;
	int return_first_segment_occurence_in_pdx(int seg_no)const;
	int return_last_segment_occurence_in_pdx(int seg_no)const;
	int return_segment_position_in_pdx(int seg_no, string mode="unambiguous")const; //note: get_segment_position in pdx (! return special number if two occurences) + add auxiliary: smaller position, greater position
	int return_arm_side(int seg_no)const; //note: rename to: return arm side by segment number
	int get_outgoing_arm_number(int pdx_side)const;
	array<int,2> give_outgoing_segment_numbers()const; //note: is this used? (maybe change definition -- use `int get_outgoing_arm_number(int side)const;`)
	array<int,2> give_incoming_segment_numbers()const;



	bool is_pdx_fully_colored()const{return find(begin(left_colors), end(left_colors), -1) == end(left_colors) && find(begin(right_colors), end(right_colors), -1) == end(right_colors);};
	

	array<int,2> return_uncolored_segment_position_and_side() const;

	void manage_conjugations(Crossing* new_cx_ptr);
};

#endif