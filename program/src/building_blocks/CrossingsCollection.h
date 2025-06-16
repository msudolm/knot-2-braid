#ifndef CROSSINGS_COLLECTION_H
#define CROSSINGS_COLLECTION_H

#include <unordered_map>
#include "Crossing.h"
#include "../parser/PD_X.h"

using namespace std;





class CrossingsCollection{
public:
	unordered_map<int, Crossing*> crossings; //some collection for storing Crossing objects accesible by id (vector? unordered map? ordered map?)


	~CrossingsCollection();
	int size()const{return crossings.size();};

	//int add_new_from_pdx(PD_X &pd_crossing, int side);
	int generate_new_crossing(int sign); //note: delete or rename to add_new_return_key
	Crossing* add_new_return_ptr(int sign); //note: check if used and remove if not
	//Crossing* add_new_return_ptr(int sign, int cx_l_color, int cx_r_color); //note: check if used and remove if not
	Crossing* add_new_return_ptr(int sign, int edge_id, int cx_l_color, int cx_r_color);
	

	void print_collection();

	int add_element_return_id(Crossing* cx_ptr);
	//int add_from_pdx_return_key(PD_X &pdx , int pdx_side);
	Crossing* add_from_pdx_return_ptr(PD_X &pdx , int pdx_side, int edg_id);

	Crossing* get_ptr_by_key(int cx_id){ return crossings.at(cx_id);} //note: check if throws an error when key not in u_map //old name: get_crossing_ptr

};




#endif