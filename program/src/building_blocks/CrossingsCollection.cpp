#include "CrossingsCollection.h"


using namespace std;




CrossingsCollection::~CrossingsCollection(){
	for (auto& entry : crossings) {
            delete entry.second;
        }
        crossings.clear();
}


/*Method description:
	This method creates new Crossing object in which only two attributes are defined:
		- crossing_id (which is the current size of CrossingCollection)
		- crossing_sign (as passed)
	and adds pointer to this object to CrossingCollection under key:=crossing_id.
	Returns key (:=crossing_id)*/
int CrossingsCollection::generate_new_crossing(int sign){
	assert(abs(sign)==1);
	 
	int new_key = crossings.size();
	assert(crossings.find(new_key) == crossings.end());

	Crossing* c = new Crossing(sign, new_key);
	crossings.insert({new_key, c});
	return new_key;
}

Crossing* CrossingsCollection::add_new_return_ptr(int sign){
	int cx_id = this->generate_new_crossing(sign);
	return crossings.at(cx_id);}


// Crossing* CrossingsCollection::add_new_return_ptr(int sign, int cx_l_color, int cx_r_color){
// 	cout << "calling: Crossing* CrossingsCollection::add_new_return_ptr(int sign, int cx_l_color, int cx_r_color)" << endl;
// 	int cx_id = this->generate_new_crossing(sign);
// 	Crossing* c = crossings.at(cx_id);
// 	c->set_color(cx_r_color, 1);
// 	c->set_color(cx_l_color, -1);
// 	return c;
// }


// Crossing* CrossingsCollection::add_new_return_ptr(int sign, int edge_id, int cx_l_color, int cx_r_color){
// 	cout << "calling: Crossing* CrossingsCollection::add_new_return_ptr(int sign, int edge_id, int cx_l_color, int cx_r_color)" << endl;
// 	int cx_id = this->generate_new_crossing(sign);
// 	//Crossing* c = crossings.at(cx_id);
// 	crossings.at(cx_id)->set_edge_id(edge_id);
// 	crossings.at(cx_id)->set_color(cx_r_color, 1);
// 	crossings.at(cx_id)->set_color(cx_l_color, -1);
// 	return crossings.at(cx_id);
// }


int CrossingsCollection::add_element_return_id(Crossing* cx_ptr){
	cout << "Note: function CrossingsCollection::add_element_return_id(Crossing* cx_ptr) will be deprecated;" << endl;
	cout << "Use CrossingsCollection::generate_new_crossing(int sign) instead." << endl;

	int new_key = crossings.size();
	assert(crossings.find(new_key) == crossings.end()); 
	crossings.insert({new_key, cx_ptr});
	return new_key;}

Crossing* CrossingsCollection::add_from_pdx_return_ptr(PD_X &pdx , int pdx_side, int edg_id){
	cout << "NOTE: CHANGE THIS FUNTION LATER (CrossingsCollection::add_new_crossing_return_ptr(PD_X &pdx_id , int pdx_side))" << endl;

	int new_key = crossings.size();
	assert(crossings.find(new_key) == crossings.end());

	Crossing* c = new Crossing(pdx, pdx_side, new_key, edg_id);
	crossings.insert({new_key, c});
	return c;}







void CrossingsCollection::print_collection(){

	// note: maybe add division to todo-done
	cout << "---------- Crossings Colletion -----------" << endl;
	cout << "Number of crossings in collection: " << crossings.size() << endl;




	if(crossings.size() == 0){cout << endl <<"-----------------------------" << endl << "Crossings Collection is empty." << endl;}
	else{
		//iterate over colors in collection
		for(const auto &pair :  crossings){
			//cout <<"-----------------------------" << endl;
			cout << "Crossing id in collection: " << pair.first << ".    ";
			pair.second->print_crossing(); cout << "    |   conjugate: "; ((pair.second)->conjugate_crossing)->print_crossing(); cout << endl;
		}
	}
	cout << "-----------------------------" << endl;
}