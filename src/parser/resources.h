#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>


namespace PacketProperty
{

	enum Protocol
	{
		TCP,
		HTTP,
		UDP,

		UNKNOWN //this is the error/catch all type
	};

	static const char* ProtocolNames[4] = {
		"TCP",
		"HTTP",
		"UDP",

		"UNKNOWN"
	};

}

class StreamId
{
public:
	StreamId()
		:type(PacketProperty::UNKNOWN)
	{
	}

	StreamId(std::string sourceIn, std::string destIn, PacketProperty::Protocol typeIn)
		:source(sourceIn), destination(destIn), type(typeIn)
	{
	}

	std::string source;
	std::string destination;
	PacketProperty::Protocol type;

	bool operator < (const StreamId& other) const
	{
		if (source != other.source){
			return source < other.source;
		}
		if (destination != other.destination){
			return destination < other.destination;
		}
		return type < other.type;
	}

};




#endif //RESOURCES_H