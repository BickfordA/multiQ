#include "parser.h"
#include "packetStream.h"
#include "resources.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace PacketProperty;


//struct streamCompare
//{
//	bool operator() (const PacketStream&  leftPS, const PacketStream& rightPS) const
//	{
//		if (leftPS.source() != rightPS.source()){
//			return leftPS.source() < rightPS.source();
//		}
//		else if (leftPS.destination() != rightPS.destination()){
//			leftPS.destination() < rightPS.destination();
//		}
//		return leftPS.protocol < rightPS.protocol();
//	};
//};

Parser::Parser()
{
	_inputFile = NULL;
}

Parser::~Parser()
{
	if (_inputFile){
		_inputFile->close();
		delete _inputFile;
	}
}

bool Parser::openFile(string filePath)
{
	if (_inputFile){
		_inputFile->close();
		delete _inputFile;
	}

	_inputFile = new ifstream(filePath);

	if (_inputFile->is_open()){
		_errMsg = "Could not open the specified file";
		return false;
	}

	return true;
}

PacketStream Parser::getStream(std::string source, std::string destination, std::string protocol)
{

	Protocol filterProtocol = stringToProtocol(protocol);

	StreamId query(source, destination, filterProtocol);

	return _packetStreams[query];
}

bool Parser::loadFile()
{
	if (!_inputFile->is_open()){
		return false;
	}

	//read the whole file into memory

	int lineCount = 0;
	char buffer[1024];

	//skip the titles
	_inputFile->getline(buffer, sizeof(buffer));

	while (!_inputFile->eof()){
		_inputFile->getline(buffer, sizeof(buffer));
		lineCount++;

		string line(buffer);
		if (line.empty()){
			continue;
		}
		Packet nextPacket;
		string err;
		if (!parseLine(line, nextPacket, err)){
			_errMsg += err + " at line: " + to_string(lineCount) + "\n";
			continue;
		}

		_packetStreams[nextPacket.streamId()].appendPacket(nextPacket);
	}

	return true;
}

bool Parser::parseLine(const string& line, Packet& parsedPacket, string& err)
{
	list<string> lineValues = spiltLine(line, ',');

	//remove the quotes from each of the values
	for (string& val: lineValues){
		unquotify(val);
	}
	
	if (lineValues.size() != 7){
		_errMsg = "Incorrect number of values were parsed";
	}

	//first value is the number of the packet
	//skip this for now (it doesnt seem usefull)
	lineValues.pop_front();

	//the second value is the arrival time of the packet in ms
	double arrivalTime = atof(lineValues.front().c_str());
	lineValues.pop_front();


	//the third value is the Source of the packet
	//could make this numeric.. string for now
	string source = lineValues.front();
	lineValues.pop_front();

	//the fourth value is the Desitnation of the packet
	string destination = lineValues.front();
	lineValues.pop_front();


	//next is the Protocol of the packet
	Protocol protocol = stringToProtocol(lineValues.front());
	lineValues.pop_front();


	//next is the length of the packet
	int length = atoi(lineValues.front().c_str());
	lineValues.pop_front();

	//lastly is the Info which is a bunch of stuff
	//all thrown in one field which could be usefull?
	string info = lineValues.front();

	parsedPacket = Packet(arrivalTime, source, destination, protocol, length, info);
	return true;
}

Protocol Parser::stringToProtocol(string protocolString)
{
	std::transform(protocolString.begin(), protocolString.end(), protocolString.begin() , ::toupper);

	if (protocolString == "TCP"){
		return TCP;
	}

	if (protocolString == "HTTP"){
		return HTTP;
	}

	if (protocolString == "UDP"){
		return UDP;
	}


	return UNKNOWN;
}

list<string> Parser::spiltLine(const string value, char delimiter)
{
	list<string> returnValue;

	istringstream valueStream(value);
	string s;
	while (getline(valueStream, s, delimiter)){
		returnValue.push_back(s);
	}
	return returnValue;
}

void Parser::unquotify(string& stringValue)
{
	//might want to do something fancy here someday
	if (stringValue.size())
		stringValue.erase(0, 1);
	if (stringValue.size())
		stringValue.erase(stringValue.size() - 1, 1);

}