#include "packet.h"


Packet::Packet()
	:_arrivalTime(-1), _protocol(UNKNOWN), _length(-1)
{
	//default constructor;
}

Packet::Packet(double arrivalTime,
	std::string source,
	std::string destination,
	Protocol protocol,
	int legnth,
	std::string info
	)
	: _arrivalTime(arrivalTime), _source(source), _destination(destination), _protocol(protocol), _length(legnth), _info(info)
{
}


double Packet::interarrivalTime(const Packet& other) const
{
	if (other.arrivalTime() > _arrivalTime){
		return other.arrivalTime() - _arrivalTime;
	}
	return _arrivalTime - other.arrivalTime();
}
