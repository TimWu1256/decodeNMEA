/*------------------------------------------------------------------------------
* decodeNMEA2.cpp: decode all NMEA sentences in file.
*
* version: $Revision:$ $Date:$
* history: 2021/06/21 1.0 new
*		   ????/??/?? 1.1
*		   2022/06/29 1.2 add "fin" and "fout"
*						  filter binary sentences
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
	bool fout_flag = false;

	ifstream fin; string fin_path = "data\\test_data3.txt"; fin.open(fin_path, ios::binary);
	ofstream fout; if (fout_flag) { string fout_path = "res\\test_data3_nmea.txt"; fout.open(fout_path); }

	if (!fin) { cerr << "failed to open file!\n"; exit(EXIT_FAILURE); }
	else {
		while (getline(fin, data.inputdata)) {
			if (data.parseData()) {
				dataNum++;
				cout << data.inputdata << "\n";
			}
			else {
				continue;
			}

			if (checksum_flag) {
				data.generateChecksum();
				data.convertChecksum();
				if (data.verify()) {
					// cout << "Checksum passed!\n";
					fout << data.inputdata << "\n";
					passedNum++;
				}
				else {
					// cout << "Checksum failed!\n";
					failedNum++;
				}
			}
		}
	}

	fin.close();
	if (fout_flag) fout.close();

	cout << "\n------------------------Result------------------------\n";
	cout << "Total sentences: " << dataNum << "\n";
	cout << "Valid sentences: " << passedNum << "\n";
	cout << "Invalid sentences: " << failedNum << "\n";

	// system("PAUSE");
	return 0;
}
