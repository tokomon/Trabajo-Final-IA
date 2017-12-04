#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <map>
#include <set>
#include <limits>
using namespace std;
#include "KMeans.h"


int main(int argc, char* argv[])
{
	// Checking the number of arguments
    if(argc != 3)
	{
		cout << "\nIncorrect number of arguments!" << endl;
		cout << "Usage:\t" << argv[0] << " <input_file_path> <number_of_clusters>" << endl;
		return 1;
	}

	KMeans *kMeans = new KMeans();

	// Example of Getter and Setter - to update the private member through public member
	kMeans->setNumberOfClusters(stoi(argv[2]));

	// Start k-means clustering for input data
	string file(argv[1]);
	kMeans->process(file);

	return 0;
}
