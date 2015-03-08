#ifndef PACKET_STREAM
#define PACKET_STREAM

#include "packet.h"
#include <list>


struct StreamId
{
	std::string source;
	std::string destination;
	Packet::Protocol type;

	StreamId()
		:type(Packet::UNKNOWN)
	{
	}

	StreamId(std::string sourceIn, std::string destIn, Packet::Protocol typeIn)
		:source(sourceIn), destination(destIn), type(typeIn)
	{
	}

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

class PacketStream
{
public:
	PacketStream();
	void appendPacket(Packet newPacket);


	void sortPackets();
	Packet firstPacket();

private:
	std::list<Packet> _packetStream;

};


#endif //PACKET_STREAM