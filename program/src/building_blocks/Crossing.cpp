#include "Crossing.h"


Crossing::Crossing(int sign, int cx_id){
	//cout << "TEST - FUNCTION NOT USED (Crossing::Crossing(int sign, int cx_id))" << endl << endl;
	assert( abs(sign) == 1 );
	assert( cx_id > -1);

	this->sign = sign;
	this->crossing_id = cx_id;
	this->edge_id = -1;
	this->next_crossing = nullptr;
	this->prev_crossing = nullptr;
	this->conjugate_crossing = nullptr;
	this->left_color = -1;
	this ->right_color = -1;
	this->name = "";	}




Crossing::Crossing(PD_X &pdx, int pdx_side, int new_cx_id, int edg_id){
	cout << "TEST - FUNCTION NOT USED (Crossing::Crossing(PD_X &pdx, int pdx_side))" << endl << endl;
	assert(abs(pdx_side) == 1);

	// Get colors from ouotgoing arm on requested side
	int xi;
	if(pdx.sign == 1){xi = (pdx_side == -1) ? 2 : 1;}; //positive pd_crossing
	if(pdx.sign == -1){xi = (pdx_side == -1) ? 3 : 2;}; //negative pd_crossing

	crossing_id = new_cx_id;
	edge_id = edg_id;

	sign = pdx.sign;
	left_color = pdx.left_colors[xi];
	right_color = pdx.right_colors[xi];
	//this->conjugate_crossing = pdx.p_first_conjugate_holder;
	this->conjugate_crossing = nullptr;

	this->next_crossing = nullptr;
	this->prev_crossing = nullptr;
	this->name = "";}




void Crossing::print_crossing(){

	string sign_str = (this->sign == 1) ? "+" : "-";
	cout << name << " " << sign_str << " " << "   ( " << left_color << " : "<< right_color <<" ) " << " (id=" << crossing_id << ", edge: " << edge_id << ") ";}



/*arg descr
direction - (1) if fe want the color from this Crossing, (-1) if we want the color behing this Crossing (from prev Crossing)
side - from which side of the segment do we want to pick the color (left (-1) / right (+1) in respect to edge orientation)
*/
int Crossing::get_color(int side, int direction){
	assert(abs(side)==1 && abs(direction)==1);
	Crossing* c_ptr = (direction==1) ? this : this->prev_crossing;
	return (side == -1) ? c_ptr->left_color : c_ptr->right_color;}




void Crossing::set_color(int color, int side, int direction){ //note: maybe better change the order to int color, int side later //(int side, int color, int direction)
	assert(abs(side)==1 && abs(direction)==1);

	/* Take this crossing or previous, depending on the direction */ 
	Crossing* c_change = (direction==1) ? this : this->prev_crossing;

	//cout << "\tsetting color (" << color << ") on side (" << side << ") in the direction (" << direction << ") of crossing " << this->name << " with id=" << this->crossing_id << endl;
	//if(c_change != this){ cout << "\tthe direction is (-1), so technically the color attribute will be changed for the this->prev_crossing (with id=" << c_change->crossing_id << ")" << endl;} 
	int &sidecolor = (side== -1) ? c_change->left_color : c_change->right_color;
	sidecolor = color;}

void Crossing::elongate_color_from_neigbour(int direction, int side){
	Crossing* neigb_ptr = (direction==1) ? (this->next_crossing) : (this->prev_crossing);
	assert(neigb_ptr != nullptr);

	//note: optionally: check if no color set on this side

	this->set_color( neigb_ptr->get_color(side), side); }







/*Funkcja dla zadanego kierunku strony krawędzi (odniesionych do krawędzi, na której skrzyżowanie, dla którego wywoływana funkcja)
 sprawdza czy przy poruszaniu się po brzegu obszaru zgodnie z zadanymi parametrami (kierunek, strona), po przejściu do skrzyzowania sprzężonego,
 znajdziemy w nim kontynuację koloru
 (inaczej mówiąc - sprawdzamy czy srzyżowanie sprzężone należy do brzegu obszaru, z kolorem, którym chcemy się poruszać).*/


bool Crossing::check_if_conjugate_continues_traversal_color(int current_direction, int current_side, int color){
	//note: można zmienić na wnioskowanie koloru z bieżącej strony i zwrotu -- ale wtedy trzeba byc uważnym przy różnych obieganiach, kiedy się go zmienia -- i sprawdzać to w odpowiednim momencie (-->czy przed/po zmianie chcemy sprawdzac)

	//color on the desired side (in respect to edge orientation) of conjugate crossing
	int conj_color = (this->conjugate_crossing)->get_color(-current_side, -current_direction);
	return (conj_color == color);
}

/*Funkcja obiegająca obszar o kolorze, który znajduje się w kierunku (+1/-1) przed/za skrzyżowaniem, na którym jest wywoływana
	i po stronie prawej/lewej (+1/-1) skrzyżowania (strona względem orientacji krawędzi).
  W przypadku, w którym kolor jest kontynuowany zarówno na skrzyżowaniu sprzężonym, jak i sąsiednim na tej samej krawędzi - 
  w pierwszej kolejności wybierane jest skrzyżowanie sprzężone (chyba, że bieżąca krawędź ma więcej niż jedno skrzyżowanie, a właśnie w byliśmy w sprzężonym).*/
void Crossing::traverse_color(int start_direction, int start_side){


	int color = this->get_color(start_side, start_direction);
	Crossing* current_cx = this;
	Crossing* prev_cx = this->conjugate_crossing;

	int current_direction = start_direction;
	int current_side = start_side;

	int* d = &current_direction;
	int* s = &current_side;
	
	int num_start_passes_left = 0; //how many time we can pass the start crossing before considering traversal complete




	// ##### PRINT TRAVERSAL INITIAL INFO ######
	cout << "\nTraversing color: \'" << color << "\' from crossing \'" << this->name << "\'..." << endl;
	cout << "\tcurrent_cx: \'" << current_cx->name << "\'     ("<< *d << ", " << *s <<")" << endl;
	// #########################################



	//if we are starting form crossing that has color continuation on its conjugate -- we increase number of times we will pass the start crossing by one (from 0 to 1)
	if(this->check_if_conjugate_continues_traversal_color(start_direction, start_side, color)){
		//cout << "Maybe replace this condition with straightforward comparisions - to avoid confusion because of using this method in this context"
		//if color on both sides - we will be visiting start crossing two times (during traversal and at the end)
		num_start_passes_left++;
	};

	while(num_start_passes_left > -1){


		if( (current_cx->conjugate_crossing != prev_cx) && (current_cx->check_if_conjugate_continues_traversal_color(*d, *s, color)) ){
			cout << "\t\tmoving to conjugate" << endl;
			prev_cx = current_cx;
			current_cx = prev_cx->conjugate_crossing;
			*d = -(*d);
			*s = -(*s);
		}
		else{
			cout << "\t\tmoving to next in the direction " << *d << endl;
			prev_cx = current_cx;
			current_cx = prev_cx->give_neigbour_crossing(*d);
		}

		assert((current_cx->get_color(*s, *d) == color)); //testing



		// ### PRINT CURRENT CROSSING THAT WE ARE IN ####
		cout << "\tcurrent_cx: \'" << current_cx->name << "\'     ("<< *d << ", " << *s <<")" << endl;
		// ##############################################


		// If arrived to the start crossing: decrease number of permissible START passes by one
		if(current_cx == this){
			cout << "\t\tarrived at the START crossing." << endl;
			num_start_passes_left--;
		}

	}

	cout << "...finished.";


	//namierz kolejne skrzyżowanie kontynuujące kolor (pierwszeństwo sprzężonego)
	//przenieś się do tego skrzyżowania
	//[wypisz skrzyzowanie, w którym się znalazłeś]
}