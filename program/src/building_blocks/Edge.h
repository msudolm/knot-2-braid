#ifndef EDGE_H
#define EDGE_H

#include <unordered_set>

#include "Crossing.h"

using namespace std;


class Edge{

private:
void add_color_id(int color_nr, int side);




public:
	
	int edge_id;
	int anchor_crossing_id;
	//note: można ewentualnie dodać jeszcze trzymanie rozmiaru: to na etapie przeprowadzania ruchu umożliwi wybieranie do wchłonięcia krawędzi, któ®a ma mniej skrzyżowań
	//(wchłonięcie ma złożoność O(liczba_skrzyżowań_na_krawędzi_wchłanianej)

	Edge(): edge_id(-1), anchor_crossing_id(-1) {}; //note: maybe delete in final version
	Edge(int e_id, int anch_id=-1): edge_id(e_id), anchor_crossing_id(anch_id) {}; //!!! moźe powodować blędy w reszczie kodu, gdzie Edge(anch_id!)

	void set_anchor(int cx_id);
	void print_edge();

	//void add_colors_ids_from_crossing(Crossing &cx);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//lapidarium

	//colors on side can reduce the number of crossings vievwd on e2 while absorbing it by e1 - but at the cost of space -- and complexity of this absorbtion remains O(num_crossings_e2) anyway because of e.g. moving all e2 crossings from key e2 to key e1
	//unordered_set<int> colors_left;
	//unordered_set<int> colors_right; //ids of colors that this egde (Seifert circle) has on its particular side; used when joining edges (while moving crossing ids to the second edge)	
	
	//Edge(int anchr_cx_id){anchor_crossing_id = anchr_cx_id;};
	//void insert_another_edge(..., Crossing* cx_a); //for performing move (joining edges)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
};


#endif