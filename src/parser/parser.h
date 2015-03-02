#ifndef PARSER_H
#define PARSER_H

#include "packet.h"

#include <string>

class Parser
{
public:
	Parser(std::string filePath);

	virtual bool getNextPacket(Packet& packet) = 0;

	bool hasError() { return _errMsg.empty(); }

protected:

	std::string _errMsg;
};






#endif PARSER_H
