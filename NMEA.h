/*------------------------------------------------------------------------------
* NMEA.h: NMEA class and functions
*
* version: $Revision:$ $Date:$
* history: 2022/06/29 1.0 move from decodeNMEA.cpp
*						  change the type of "buffer": string -> vector<string>
*						  remove "Maxsize"
------------------------------------------------------------------------------*/
#pragma once

#include <string>
#include <vector>
#include <ctype.h>
#include "cmnfun.h"
using namespace std;

/* class definition -----------------------------------------------------------*/

class NMEA {
public:
	NMEA() {
		generatedChecksum = 0;
		convertedChecksum = 0;
		buffer.clear();
	}
	bool parseData();
	void generateChecksum();
	void convertChecksum();
	bool verify();
	int generatedChecksum;
	int convertedChecksum;
	string inputdata;
	vector<string> buffer;
};

bool NMEA::parseData() {
	size_t pos = 0;
	string token;
	string inputData = this->inputdata;
	int count = 0;
	stringstream ss_;
	int buffer_i = 0;

	if (inputData.size() < 6 || inputData.substr(0, 2) != "$G") return false;

	while ((pos = inputData.find(',')) != string::npos) {
		if (pos == 0) {
			count++;
			inputData.erase(pos, 1);
		}
		else {
			token = inputData.substr(0, pos);
			buffer.push_back(token);
			// ss_ << hex << buffer.back();
			// ss_ >> buffer_i;
			// if (!isascii(buffer_i))
			// 	return false;
			count++;
			inputData.erase(0, pos + 1);
		}
	}

	pos = inputData.find('*');
	token = inputData.substr(0, pos);
	buffer.push_back(token);
	// ss_ << hex << buffer.back();
	// ss_ >> buffer_i;
	// if (!isascii(buffer_i))
	// 	return false;
	count++;
	inputData.erase(0, pos + 1);

	buffer.push_back(inputData);
	// ss_ << hex << buffer.back();
	// ss_ >> buffer_i;
	// if (!isascii(buffer_i))
	// 	return false;


	// for (int i = 1;i < buffer.size();i++) {
	// 	ss_ << hex << buffer[i];
	// 	ss_ >> buffer_i;
	// 	if (!isascii(buffer_i))
	// 		return false;
	// }

	return true;
}

void NMEA::generateChecksum() {
	int sum = 0;
	string inputData = this->inputdata;
	for (unsigned i = 0; i < inputData.length() - 2; i++) {
		if (inputData[i] == '$' || inputData[i] == '*')  continue;
		sum ^= inputData[i];
	}
	this->generatedChecksum = sum;
}

void NMEA::convertChecksum() {
	stringstream ss;
	string checksumStr = this->buffer.back();
	int checksumValue = 0;
	ss << hex << checksumStr;
	ss >> checksumValue;
	this->convertedChecksum = checksumValue;
}

bool NMEA::verify() {
	if (this->convertedChecksum == this->generatedChecksum)  return true;
	else  return false;
}