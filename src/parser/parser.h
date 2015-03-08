#ifndef PARSER_H
#define PARSER_H

#include "packet.h"

#include <string>
#include <fstream>
#include <map>
#include <list>



struct streamCompare
{
	bool operator() (const PacketStream&  leftPS, const PacketStream& rightPS) const
	{
		if (leftPS.source() != rightPS.source()){
			return leftPS.source() < rightPS.source();
		}
		else if (leftPS.destination() != rightPS.destination()){
			leftPS.destination() < rightPS.destination();
		}
		return leftPS.protocol < rightPS.protocol();
	};
};


class Parser
{
public:
	Parser();
	~Parser();

	bool openFile(std::string filePath);
	bool loadFile();



	bool hasError() { return _errMsg.empty(); }
	std::string errMsg();

protected:
	bool parseLine(const std::string& line, Packet& parsedPacket, std::string& errMsg);

	std::list<std::string> spiltLine(const std::string value, char delimiter);
	
	Packet::Protocol stringToProtocol(std::string protocolString);

	void unquotify(std::string& stringValue);



	std::map<StreamId, PacketStream> _packetStreams;

	std::ifstream * _inputFile;

	std::string _errMsg;
};



#endif PARSER_H
