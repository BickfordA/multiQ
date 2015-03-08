#ifndef PACKET_H
#define PACKET_H

#include "packetStream.h"
#include "resources.h"

#include <string>

class Packet
{
public:
	Packet();

	Packet(double arrivalTime, 
		   std::string source, 
		   std::string destination, 
		   PacketProperty::Protocol protocol, 
		   int legnth, 
		   std::string info
		   );

	double interarrivalTime(const Packet& other) const;
	
	double packetSize()const { return _length; }

	std::string source() const { return _source; }
	std::string destination() const { return _destination; }
	double arrivalTime() const { return _arrivalTime; }
	PacketProperty::Protocol protocol() const { return _protocol; }

	StreamId streamId() const { return _streamId; }

private:

	double _arrivalTime;
	std::string _source;
	std::string _destination;
	PacketProperty::Protocol _protocol;
	int _length;
	std::string _info;

	StreamId _streamId;
	

	std::string _originalData;
};




#endif //PACKET_H