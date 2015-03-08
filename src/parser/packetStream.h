#ifndef PACKET_STREAM
#define PACKET_STREAM

#include "packet.h"
#include <queue>


struct StreamId
{
	std::string source;
	std::string destination;
	Packet::Protocol type;

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

	std::string source() const { return  _streamId.source; }
	std::string destination() const { return  _streamId.destination; }
	Packet::Protocol protocol() const { return _streamId.type; }

private:
	std::queue<Packet> _packetStream;


	StreamId _streamId;
};


#endif //PACKET_STREAM