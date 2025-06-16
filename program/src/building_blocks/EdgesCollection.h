#ifndef EDGES_COLLECTION_H
#define EDGES_COLLECTION_H

#include <unordered_map>
#include "Edge.h"

using namespace std;



class EdgesCollection{
public:
	int max_key;
	unordered_map<int, Edge*> edges;

	EdgesCollection(){max_key=-1; edges={};}
	~EdgesCollection();

	int size()const{return edges.size();}
	bool is_key_in_collection(int edge_id)const{ return (edges.find(edge_id) != edges.end()); }

	void add_new();
	Edge* add_new_return_ptr();
	int add_new_return_key(); //add new empty edge and return key
	void set_edge_anchor(int edge_id, int anchor_cx_id);
	void remove_edge(int edge_id);

	Edge* get_ptr_by_key(int edge_id){ return edges.at(edge_id);}

	void print_edges_collection(); //note: mark as const
};


#endif