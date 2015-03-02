
#include "stdio.h"
#include <iostream>
#include <fstream>


using namespace std;

/*************************
* This is the main dirver
* which is used to read packet
* traces and calculate path metrics
* from the packet spaceing
**************************/

int main(int argc, char* argv[])
{
	if (argc != 2){
		fprintf(stdout, "Incorrect number of arguments");
		fprintf(stdout, "Correct syntax: \"mp <filename>\"");
		return 1;
	}

	ifstream inputFile(argv[1]);
	//just do this to make sure that is a valid file
	//the scanner will be responsible for it's own  file pointer

	if (!inputFile.is_open()){
		fprintf(stdout, "Could not open file!!! \nCheck the path and try again");
		return 1;
	}
	inputFile.close();

	//Pass the file name to the parser

	//read all of the packets from the file

	//pass the packets to multiq

	//get calculations from multiQ

}