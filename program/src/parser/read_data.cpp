#include <iostream>
#include <fstream>
#include <sstream>

#include<algorithm> //max()
#include <unordered_set>

#include<stdexcept> //

//include "crossing.h"

using namespace std;




//#####################
//Building the graph from pd-code -- description:
//
// 1. Parse raw X[i,j,k,l] data from an input file. (is correctness checked?)
//	  Find the maximum segment number
// 2. Define a sign for each crossing based on the relationships between i,j,k,l
//	  (max segment number used for easy detection of n-->1 segments connection)
// 3. Walk across the intersections to assign the appropriate colors of diagram faces to the corresponding crossings.
// 4. Walk across the intersections to assign them to appropriate Seifert circles.
//	  Save the information about conjugate crossings.
// 5. Use the information obtained in the previous steps to transfer the collection of crossings to the graph structure.
//
// Possible extensions (?)
//	- store some information about admissible pairs of segments to use for braiding
//		(+information 'for free', no additional search needed (for the beggining)
//		 - probably impossible to effectively manage later; uses additional memory; may not be consistent with defined later graph-modifications policy and result in performing non-optimal moves;
//		(maye keep one pair just for the beggining))

//####################






struct Segment {
	int segment_nr;
	string l_color;
	string r_color;
	
};


//class storing all informations derived from pd-code
//helper class for building the main graph structure
struct CX {

	//attributes
	int x_id;
	int sign;
	//Segment segments[4]; //change later
	int x1, x2, x3, x4; //subsequent crossing's integers from pd-code
	//crossing* p_cx_seif1;


	//methods
	CX(int x, int y, int z, int w);

	//void read_pd(const string& filepath, set<CX>& cx_set);
	unordered_set<CX> read_pd(const string& filepath);

};



CX::CX(int x, int y, int z, int w){
	/**/
	.x1 = x;
	.x2 = y;
	.x3 = z;
	.x4 = w;
	
	.sign = 0;
	.x_id = -1;
	//p_cx_seif1 = nullptr;
};


/*
Function raeds pd-code data from input file
and returns a collection of CX objects (pd-crossings with fields for storing additional information)
*/

//void CX::read_pd(const string& filepath, set<CX>& crossings_set){
unordered_set<CX> CX::read_pd(const string& filepath){

		unordered_set<CX> pd_crossings;

	    ifstream f_in(filepath);
	    string line;


	    if (!f_in.is_open()){
			throw std::runtime_error("Unable to open file: " + filepath);
	    }


	    //preparing to read from file
	    int max_n = 0; //max segment nr in diagram
	    int max_4 = 0; //max segment nr in current pd-crossing
	    int x1, x2, x3, x4; //subsequent positions X[x1,x2,x3,x4] in single crossing written in PD-code
	    char c;

	    //reading from file
	    while (getline(f_in, line)) {

	    	//get integers for the current crossing
	    	cout << "Reading next pd-crossing..." << endl;
	       	stringstream ss(line);
	        ss >> c >> x1 >> c >> x2 >> c >> x3 >> c >> x4 >> c;
	        //cout << line << endl; // output the line to the console
	        //cout << "c: " <<c << ", x1: " << x1 << ", x2: " << x2 << ", x3: " << x3 << ", x4: " << x4 << endl;


	        // update if new max segment id encountered
	        int ns[4] = {x1,x2,x3,x4};
	        max_4 = *std::max_element(ns, ns + 4);
	        if( max_4 > max_n){max_n = max_4;
	        	cout<< "\tNew max found: " << max_n << endl;
	        };


	       	//create new CX instance for given xi's
	        cout << "\tCreating new CX instance..." << endl;
	        CX p_pdCx = CX(x1,x2,x3,x4);
	        pd_crossings.insert(p_pdCx);
	        cout << "\t...new CX created." << endl;
		}
		f_in.close(); // close the file


		//set crossings signs based on relative values of xi's


		return pd_crossings;
	
}






int main(){

	cout << "\nHello from read_data.cpp!\n" << endl;

	string test_pd_infile = "./pds_test/k_40.txt";









	return 0;
}