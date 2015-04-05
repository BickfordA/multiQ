#include "delimitedTextWriter.h"


#include <fstream>
#include <iostream>

using namespace std;

DelimitedTextWriter::DelimitedTextWriter()
{
	//
}

void DelimitedTextWriter::addData(vector<double> values, string name)
{
	vector<string> doubleAsString(values.size());

	for (int i = 0; i < values.size(); i++){
		doubleAsString[i] = to_string(values.at(i));
	}
	_dataValues.push_back(pair<string, vector<string> >(name, doubleAsString));
}

void DelimitedTextWriter::addData(vector<string> columnValues, string name)
{
	_dataValues.push_back(pair<string, vector<string> >(name, columnValues));
}

bool DelimitedTextWriter::writeFileColumns(string fileName,string delimiter)
{
	//this takes forever.............
	ofstream outFile;
	outFile.open(fileName);

	fprintf(stdout, "Writing File");
	fprintf(stdout, fileName.c_str());
	fprintf(stdout, "\n");

	int percentDone = 0;
	//asume that all of the column value are the same lenght
	int rowCount = _dataValues.front().second.size();
	for (int row = 0; row < rowCount; row++){

		string rowString;
		bool isFirst = true;
		for (pair<string, vector<string> > column : _dataValues){
			if (isFirst)
				isFirst = false;
			else
				rowString += delimiter;

			rowString += column.second.at(row);
		}
		rowString += "\n";
		outFile << rowString;

		if (row % 10 == 0){
			int done = (double)row / rowCount * 100;
			if (done != percentDone){
				percentDone = done;
				string out(to_string(percentDone));
				out += "%\n";
				fprintf(stdout, out.c_str());
			}
		}
	}

	outFile.close();
	return true;
}

bool DelimitedTextWriter::writeFileRows(string fileName, string delimiter)
{

	ofstream outFile;
	outFile.open(fileName);

	fprintf(stdout, "Writing File");
	fprintf(stdout, fileName.c_str());
	fprintf(stdout, "\n");

	int percentDone = 0;
	//asume that all of the column value are the same lenght
	int rowCount  = _dataValues.size();
	for (int row = 0; row < rowCount; row++){

		string rowString;
		bool isFirst = true;
		for (string row : _dataValues.at(row).second){
			if (isFirst)
				isFirst = false;
			else
				rowString += delimiter;

			rowString += row;
		}
		rowString += "\n";
		outFile << rowString;

		if (row % 1 == 0){
			int done = (double)row / rowCount * 100;
			if (done != percentDone){
				percentDone = done;
				string out(to_string(percentDone));
				out += "%\n";
				fprintf(stdout, out.c_str());
			}
		}
	}

	outFile.close();
	return true;
}