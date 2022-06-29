/*------------------------------------------------------------------------------
* decodeNMEA.cpp: decode all NMEA sentences in file.
*
* version: $Revision:$ $Date:$
* history: 2021/06/21 1.0 new
*		   ????/??/?? 1.1
*		   2022/06/30 1.2 add write file function
*						  print some useful information
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <sstream>
#include "NMEA.h"

using namespace std;

int main() {
	NMEA data;
	int dataNum = 0;
	int passedNum = 0;
	int failedNum = 0;
	bool checksum_flag = true;
	bool fout_flag = true;

	ifstream fin; string fin_path = "data\\test_data3.txt"; fin.open(fin_path, ios::binary);
	ofstream fout; if (fout_flag) { string fout_path = "res\\test_data3_nmea.txt"; fout.open(fout_path), ios::binary; }

	if (!fin) { cerr << "failed to open file!\n"; exit(EXIT_FAILURE); }

	while (getline(fin, data.inputdata)) {
		dataNum++;
		if (data.parseData()) {
			cout << data.inputdata << "\n";
			if (checksum_flag) {
				data.generateChecksum();
				data.convertChecksum();
				if (data.verify()) {
					passedNum++;
					if (fout_flag) fout << data.inputdata;
				}
				else {
					failedNum++;
				}
			}
		}
	}

	fin.close();
	if (fout_flag) fout.close();

	cout << "\n------------------------Result------------------------\n";
	cout << "Total sentences: " << dataNum << "\n";
	cout << "Checksum passed: " << passedNum << "\n";
	cout << "Checksum failed: " << failedNum << "\n";
	cout << "Invalid sentences: " << dataNum - (passedNum + failedNum) << "\n";
	cout << "-------------------------End-------------------------\n";

	// system("PAUSE");
	return 0;
}
