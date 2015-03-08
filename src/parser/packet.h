#ifndef PACKET_H
#define PACKET_H

#include "packetStream.h"

#include <string>

class Packet
{
public:
	
	enum Protocol
	{
		TCP,
		HTTP,
		UDP,

		UNKNOWN //this is the error/catch all type
	};

	Packet();

	Packet(double arrivalTime, 
		   std::string source, 
		   std::string destination, 
		   Protocol protocol, 
		   int legnth, 
		   std::string info
		   );

	double interarrivalTime(const Packet& other) const;
	
	double arrivalTime() const { return _arrivalTime; }
	double packetSize()const { return _length; }

	StreamId streamId() const { return StreamId(_source, _destination, _protocol); }

private:

	double _arrivalTime;
	std::string _source;
	std::string _destination;
	Protocol _protocol;
	int _length;
	std::string _info;
	

	std::string _originalData;
};




#endif //PACKET_H