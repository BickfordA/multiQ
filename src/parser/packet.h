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

	std::string source() const { return _source; }
	std::string destination() const { return _destination; }
	double arrivalTime() const { return _arrivalTime; }
	Protocol protocol() const { return _protocol; }

	StreamId streamId() const { return _streamId; }

private:

	double _arrivalTime;
	std::string _source;
	std::string _destination;
	Protocol _protocol;
	int _length;
	std::string _info;

	StreamId _streamId;
	

	std::string _originalData;
};




#endif //PACKET_H