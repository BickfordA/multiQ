
#include "parser.h"
#include "packetStream.h"
#include "flow.h"


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
	if (argc != 5){
		fprintf(stdout, "Incorrect number of arguments\n");
		fprintf(stdout, "Correct syntax: \"mp <filename>  <sourceIPAddress> <destinationIPaddress> <protocol>\n");
		return 1;
	}

	//Pass the file name to the parser

	Parser* inputParser = new Parser();

	inputParser->openFile(argv[1]);
	if (inputParser->hasError()){
		fprintf(stdout, inputParser->errMsg().c_str());
		return 1;
	}

	//read all of the packets from the file
	fprintf(stdout, "Loading streams...\n");
	inputParser->loadFile();
	if (inputParser->hasError()){
		fprintf(stdout, inputParser->errMsg().c_str());
		return 1;
	}

	//check if there is a flow corresponding to the parameters
	PacketStream selectedStream = inputParser->getStream(string(argv[2]), string(argv[3]), string(argv[4]));
	vector<double> arrivalTime = selectedStream.calculateInterarrivalTimes();

	//pass the packets to multiq
	Flow multiQFlow(arrivalTime, Flow::DATA_FLOW);

	//get calculations from multiQ
	vector<Flow::Capacity> capacities = multiQFlow.getCapacities();




	delete inputParser;
}