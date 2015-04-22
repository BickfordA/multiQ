
#include "parser.h"
#include "packetStream.h"
#include "flow.h"
#include "bandwidth\bandwidth.h"

#include "delimitedTextWriter/delimitedTextWriter.h"

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
	if (argc < 5){
		fprintf(stdout, "Incorrect number of arguments\n");
		fprintf(stdout, "Correct syntax: \"mp <filename>  <sourceIPAddress> <destinationIPaddress> <protocol>\n");
		fprintf(stdout, "Optional trailing flag -i=<filename> will print the interarrival times to the specified file");
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

	fprintf(stdout, "Found Streams:\n");
	list<string> found = inputParser->streamsFound();
	for (string f : found){
		string out = "\t" + f + "\n";
		fprintf(stdout, out.c_str());
	}

	fprintf(stdout, "\nSelected Stream: \n");
	fprintf(stdout, argv[2]);
	fprintf(stdout, " ");
	fprintf(stdout, argv[3]);
	fprintf(stdout, " ");
	fprintf(stdout, argv[4]);
	fprintf(stdout, "\n");

	fprintf(stdout, "Finding interarrival times\n");
	//check if there is a flow corresponding to the parameters
	PacketStream selectedStream = inputParser->getStream(string(argv[2]), string(argv[3]), string(argv[4]));
	vector<double> arrivalTime = selectedStream.calculateInterarrivalTimes();

	if (argc == 6){
		fprintf(stdout, "Saving interarrival times\n");
		//this is not the most flexible way to do this but it should work
		//as long as you know the flag has to be last cmd things should be alright

		//check to make sure that the value is what it is 
		//supposed to be 
		string intTimesCmd(argv[5]);
		if (intTimesCmd.find("-i=") == 0){
			string file(intTimesCmd.substr(3));
			DelimitedTextWriter dtWriter;

			dtWriter.addData(arrivalTime, "arrivalTime");
			dtWriter.writeFileRows(file);
		}
	}

	if (arrivalTime.empty()){
		fprintf(stdout, "This selected stream was empty.\n");
		return 1;
	}

	//pass the packets to multiq

	Flow multiQFlow(arrivalTime, Flow::DATA_FLOW);

	//get calculations from multiQ
	vector<Flow::Capacity> capacities = multiQFlow.getCapacities();


	Bandwidth bw(selectedStream.getPackets());
	double bandwidth = bw.measureBandwidth();


	fprintf(stdout, "The following capacities were found\n");

	for (Flow::Capacity c : capacities){
		fprintf(stdout, to_string(c.bandwidth).c_str());
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "The bandwidth acheived by the flow was:\n");
	fprintf(stdout, to_string(bandwidth).c_str());
	fprintf(stdout, "\n");


	delete inputParser;
}