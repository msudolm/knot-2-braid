#include "../src/building_blocks/Graph.h"
#include <fstream>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;
////g++ -std=c++17 ./src/parser/ColorMoves.cpp  ./src/parser/PD_X.cpp ./src/parser/Diagram.cpp ./src/building_blocks/Crossing.cpp ./src/building_blocks/Edge.cpp ./src/building_blocks/Color.cpp ./src/building_blocks/CrossingsCollection.cpp ./src/building_blocks/EdgesCollection.cpp ./src/building_blocks/ColorsCollection.cpp ./src/building_blocks/Graph.cpp ./src/main.cpp



int main(int argc, char* argv[]){
	

	//check if exactly one additional argument (pd-code file) was passed
	if(argc != 2){
		cout << "Usage: " << argv[0] << "<pd-code-filename>" << endl;
		return 1; }

	//get pd-code filename
	const char* filename = argv[1];



	Diagram d = Diagram(filename);
	d.color_diagram();
	Graph g = Graph(d);


	
	/* ******************************************* */
	cout << endl << "Measuring braiding time..." << endl;
	auto time_start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false); // unsync the I/O of C and C++.

	g.transform_to_braid_form();

	auto time_end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(time_end - time_start).count();
	time_taken *= 1e-9;
	cout << "...measuring braiding time finished." << endl;
	/* ******************************************* */


	cout << "Time taken by program is : " << fixed << time_taken << setprecision(9); cout << " sec" << endl;
	vector<int> braid_word = g.read_braid_word();


    // Extract the suffix from argv[1]
    string suffix;
    size_t last_slash_pos = string(argv[1]).find_last_of('/');
    if (last_slash_pos != string::npos) {
        suffix = string(argv[1]).substr(last_slash_pos + 1);
    } else {
        suffix = string(argv[1]);
    }



	string time_filepath = "/Users/gosia/Gosia/studia/bioinf/magisterka/repo/program/time_tests/results/time_results/cpp_time_" + suffix;
	string braid_filepath = "/Users/gosia/Gosia/studia/bioinf/magisterka/repo/program/time_tests/results/braid_results/cpp_braid_" + suffix;

    /* Save time result */
    ofstream time_file(time_filepath);
    time_file << fixed << time_taken << endl;
    time_file.close();

    /* Save braid result */
    ofstream braid_file(braid_filepath);
    for (int i = 0; i < braid_word.size(); ++i) {
        braid_file << braid_word[i];
        if (i < braid_word.size() - 1) {
            braid_file << ",";
        }
    }
    braid_file.close();


    cout << "Time result saved to: " << fs::absolute(time_filepath) << endl;
    cout << "Braid result saved to: " << fs::absolute(braid_filepath) << endl;

	return 0;
}