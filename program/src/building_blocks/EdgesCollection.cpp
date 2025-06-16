#include "EdgesCollection.h"


using namespace std;



EdgesCollection::~EdgesCollection(){
	for (auto& entry : edges) {
            delete entry.second;
        }
        edges.clear();
}



/*Function description:
	This function creates new empty Edge object,
	adds it to collection with key == max_key+1
	and returns key value.*/
int EdgesCollection::add_new_return_key(){

	assert(edges.find(max_key+1) == edges.end());
	++(this->max_key);
	Edge* e = new Edge(max_key);
	edges.insert({max_key, e});
	return max_key;
}


// //for now used only for testing 
// void add_new(){
// 	assert(edges.find(max_key+1) == edges.end());
// 	++(this->max_key);
// 	Edge* e = new Edge(max_key);
// 	edges.insert({max_key, e});	
// }

Edge* EdgesCollection::add_new_return_ptr(){
	assert(edges.find(max_key+1) == edges.end());
	++(this->max_key);
	Edge* e = new Edge(max_key);
	edges.insert({max_key, e});
	return e;
}


void EdgesCollection::set_edge_anchor(int edge_id, int anchor_cx_id){
	assert(this->is_key_in_collection(edge_id));
	Edge* e = this->get_ptr_by_key(edge_id);
	e->set_anchor(anchor_cx_id);
	return;
}


void EdgesCollection::remove_edge(int edge_id){
	delete edges[edge_id];
	edges.erase(edge_id);
}


void EdgesCollection::print_edges_collection(){
	//iterate over elements in collection
	cout << "---------- Edges Colletion -----------" << endl;
	if(edges.size() == 0){
		cout << endl <<"-----------------------------" << endl;
		cout << "Edges Collection is empty." << endl;
	}

	else{
		for(const auto& pair : edges){

			cout <<"-----------------------------" << endl;
			cout << "Edge id in collection: " << pair.first << ". " << endl;
			pair.second->print_edge();
		}

	cout << "-----------------------------" << endl;
	}


}



//implementation of private methods