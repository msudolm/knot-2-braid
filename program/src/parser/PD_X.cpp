#include <iostream>
#include <array>
#include <algorithm>

#include "PD_X.h"
using namespace std;


//colors as diagram features?

PD_X::PD_X(){
	xs = {};
	left_colors = {};
	right_colors = {};
	sign = 0;

}



PD_X::PD_X(const array<int,4> &xs_nums){

	sign = 0;
	for(int i=0; i<4; i++){ xs[i] = xs_nums[i];};
	left_colors = {-1,-1,-1,-1};
	right_colors = {-1,-1,-1,-1};
	
	p_first_conjugate_holder = nullptr;
	both_conjugates_done = false;
}

void PD_X::print() const{

	//numbers from segments (from pd-file)
	cout << "PD_crossing: ( "; for(int i=0; i<4; i++){cout << xs[i] << " ";}; cout << ") ";

	//sign
	cout << " (" << sign << ") " << endl;
	
	// //left & right colors
	// string col;
	// cout << "   left colors: [ ";
	// for(int i=0; i<4; i++){
	// 	col = (left_colors[i] != -1) ? to_string(left_colors[i]) : " . ";
	// 	cout << col << " "; 
	// };
	// cout << "]  right colors: [ ";
	// for(int i=0; i<4; i++){
	// 	col = (right_colors[i] != -1) ? to_string(right_colors[i]) : " . ";
	// 	cout << col << " "; 
	// };
	// cout << "]" << endl;	
}

//set given color on a given position and side of a crossing
void PD_X::set_arm_color(int color, int side, int pos){
	assert(abs(side)==1);
	assert(0 <= pos && pos <=3);

	array<int,4> &colors =  (side == -1) ? left_colors : right_colors;
	colors[pos] = color; //set color on pos-th arm of PDcrossing
};
int PD_X::get_arm_color(int side, int pos) const{
	assert(abs(side)==1);
	assert(0 <= pos && pos <=3);

	if(side == 1){ return right_colors[pos];}
	else{ return left_colors[pos];}
};

array<int,2> PD_X::get_outgoing_xi_positions()const{
	assert(abs(sign)==1);
	if(sign==1){return {1,2};}
	else{ return {2,3};};
};

int PD_X::return_number_of_segment_occurences_in_pdx(int seg_no)const{
	int n_seg = count(begin(xs), end(xs), seg_no);
	assert(n_seg==0 || n_seg==1 || n_seg==2);
	return n_seg;}


int PD_X::return_segment_position_in_pdx(int seg_no, string mode)const{
	assert(mode=="unambiguous" || mode=="any" || mode=="outgoing"); //other modes e.g. "incoming", "left"/"right", "lower_xi"/"higher_xi" may be added
	assert(seg_no >=0);

	int xi_pos=-1;
	int n_seg = this->return_number_of_segment_occurences_in_pdx(seg_no);
	assert(n_seg >0 && abs(n_seg)<3);


	/* There are no segment occurences in pdx: return invalid index */
	if(n_seg==0){ return -1;}


	/* Mode unambiguous: return valid index only if segment number on exacty one arm (not 0, not 2)*/
	if(mode=="unambiguous"){
		if(n_seg==1){xi_pos = this->return_first_segment_occurence_in_pdx(seg_no);}
		else{ xi_pos = -1; } //two occurences of segment number (-> Reidemeister I type twist in this PDX)
	}

	else if(mode=="any"){
		xi_pos = this->return_first_segment_occurence_in_pdx(seg_no);
	}

	/* Mode outgoing: return index of outgoing arm with segment number (may be -1 if n_seg==1, should always be exactly one outgoing arm with seg_no, when n_seg==2 and PD code is correct) */
	else if(mode=="outgoing"){
		//cout << "In: int PD_X::return_segment_position_in_pdx(int seg_no, string mode)const" <<endl;
		//cout << "mode=='outgoing'"<<endl;

		array<int,2> out_xis = this->get_outgoing_xi_positions();
		//cout << "\txi positions in pdx returned as outgoing: " << out_xis[0] << ", " << out_xis[1] << endl;

		int j = this->return_last_segment_occurence_in_pdx(seg_no);
		//cout << "\txi position found as the last segment number ("<< seg_no<<") occurence: " << j << endl;
		
		/* check if arm with segment number, with higher xi position (it may be only arm with segment number in pdx) is one of outgoing */
		/* if yes - return */
		if( find(out_xis.begin(), out_xis.end() ,j) != out_xis.end() ){
			xi_pos = j;
			//cout << "\txi_pos to be return as outgoing: " << xi_pos << endl;
				
			assert( find(out_xis.begin(), out_xis.end(), this->return_first_segment_occurence_in_pdx(seg_no)) ); //ensure that there is no error here -- there shouldn't be two outgoing arms with the same segment number
		}

		 
		else if(n_seg==1){
				xi_pos = -1;
		}
		else if(n_seg==2){ /* segment number on two arms on pdx and arm with lower index is not outgoing (=>arm with higher index should be outgoing if no errors in pd code representation)*/
			//cout << "\ttwo segment occurences; arm with lower index is not outgoing" << endl;
			xi_pos  = this->return_first_segment_occurence_in_pdx(seg_no);
			//cout << "\txi_pos to be return as outgoing: " << xi_pos << endl;
			assert( find(out_xis.begin(), out_xis.end() ,xi_pos) != out_xis.end()); //ensure that the second arm with segment number is among outgoing arms
		}
	}


	return xi_pos;
}

int PD_X::return_first_segment_occurence_in_pdx(int seg_no)const{ //returns xi position {0,1,2,3}; position of first occurence if two pdx arms come from this segment
	//cout << "In: int PD_X::return_first_segment_occurence_in_pdx(int seg_no)const" << endl;
	//cout << "PDX: "; this->print();
	int xi_pos_first;
	int n_seg = this->return_number_of_segment_occurences_in_pdx(seg_no);
	assert(n_seg==1 || n_seg==2);

	//cout << "\tnumber of segment ("<<seg_no<< ") occurences in pdx: " << n_seg << endl;
	const int* it = find(begin(xs), end(xs), seg_no);
	xi_pos_first = distance(begin(xs), it);

	//cout << "\tdistance(begin(xs), it): " << xi_pos_first << endl;
	assert(xi_pos_first >-1 && abs(xi_pos_first) <4);
	return xi_pos_first;}

int PD_X::return_last_segment_occurence_in_pdx(int seg_no)const{ //returns xi position {0,1,2,3}; position of second occurence if two pdx arms come from this segment
	//cout << "In: int PD_X::return_last_segment_occurence_in_pdx(int seg_no)const" << endl;
	//cout << "PDX: "; this->print();
	int xi_pos_last = -1;
	int n_seg = this->return_number_of_segment_occurences_in_pdx(seg_no);
	assert(n_seg==1 || n_seg==2);

	//find intex with the last occurence
    for (int i = 0; i < xs.size(); ++i) {
        if (xs[i] == seg_no) {
        	xi_pos_last = i;
        }
    }

	//cout << "\txi_pos_last: " << xi_pos_last << endl;
	assert(xi_pos_last>-1 && abs(xi_pos_last) < 4);
	return xi_pos_last;}



int PD_X::return_arm_side(int seg_no)const{
	assert(this->is_segment_number_in_pdx(seg_no));
	assert(abs(sign)==1);

	int pdx_side = 0;
	int xi_pos = this->return_segment_position_in_pdx(seg_no, "any");
	
	if(sign==1){ pdx_side = (xi_pos>1) ? -1 : 1;} //PDX(+) (x0,x1,x2,x3) --> L[x2,x3] R[x0,x1]
	else{ pdx_side = (xi_pos==0 || xi_pos==3) ? -1 : 1;} //PDX(-) (x0,x1,x2,x3) --> L[x0,x3] R[x1,x2] 

	return pdx_side;
}



int PD_X::get_outgoing_arm_number(int pdx_side)const{
	assert(abs(pdx_side)==1);
	assert(abs(sign)==1);

	int seg_no;
	if(sign==1){ seg_no = (pdx_side==1) ? xs[1] : xs[2];} // for pdx code (x1,x2,x3,x4) outgoing arms are encoded by x2 (right), x3 (left) in (+) crossings and by x3 (right), x4 (left) in (-) crossings
	else{ seg_no = (pdx_side==1) ? xs[2] : xs[3];}

	return seg_no;
}



array<int,2> PD_X::give_outgoing_segment_numbers()const{
	assert(abs(sign)==1);
	array<int,2> outgoing_seg_ns;

	// for pdx code (x1,x2,x3,x4) outgoing arms are encoded by x3,x4 in (-) crossings and by x2,x3 in (+) crossings
	if(sign==-1){ outgoing_seg_ns = {xs[2], xs[3]};}
	else{ outgoing_seg_ns = {xs[1],xs[2]};}
	return outgoing_seg_ns;
}

array<int,2> PD_X::give_incoming_segment_numbers()const{
	assert(abs(sign)==1);
	array<int,2> incoming_seg_ns;

	// for pdx code (x1,x2,x3,x4) incoming arms are encoded by x1,x2 in (-) crossings and by x1,x4 in (+) crossings
	if(sign==-1){ incoming_seg_ns = {xs[0], xs[1]};}
	else{ incoming_seg_ns = {xs[0], xs[3]};}
	return incoming_seg_ns;
}




array<int,2> PD_X::return_uncolored_segment_position_and_side()const{
	//returns {xi, side} [changed form: {side, xi}]
	//where side is -1 (left) or 1 (right)
	//and xi \in {0,1,2,3} is a pd-code position of found non-colored segment

		//check if there is any not left-colored
		for(int i=0; i < 4; i++){
			if(left_colors[i] == -1){
				array<int,2> pos = {i,-1};
				return pos;
			}
		};
		//check if there is any not right-colored
		for(int i=0; i < 4; i++){
			if(right_colors[i] == -1){
				array<int,2> pos = {i,1};
				return pos;
			}
		};

		//all segments colored on both sides
		return {-1,0}; //no_pos;
	}



void PD_X::manage_conjugations(Crossing* new_cx_ptr){
	assert(!both_conjugates_done);

	//new_cx is first of the conjugates pair
	if(p_first_conjugate_holder == nullptr){
		p_first_conjugate_holder = new_cx_ptr; //PD_X keeps information about first conjugate
	}
	//pointer to first conjugate crossing saved in PD_X, new_cx is the second one
	else{
		p_first_conjugate_holder->conjugate_crossing = new_cx_ptr; //first points to new
		new_cx_ptr->conjugate_crossing = p_first_conjugate_holder; //new points to first
		both_conjugates_done = true;
	}
}
