#ifndef PACKET_STREAM
#define PACKET_STREAM


#include "packetStream.h"
#include "resources.h"
#include "packet.h"

#include <list>
#include <vector>

class Packet;
class PacketStream
{
public:
	PacketStream();
	void appendPacket(Packet newPacket);

	std::vector<double> calculateInterarrivalTimes();

	std::string source() const { return _streamId.source; }
	std::string destination() const { return _streamId.destination; }
	PacketProperty::Protocol protocol() const { return _streamId.type; }

private:
	StreamId _streamId;
	
	std::vector<Packet> _packetStream;

};


#endif //PACKET_STREAM