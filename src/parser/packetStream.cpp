#include "packetStream.h"

#include <algorithm>


struct packetArivalComparer
{
	bool operator() (Packet a, Packet b)
	{
		return a.arrivalTime() < b.arrivalTime();
	}
} arrivalCompare;

PacketStream::PacketStream()
{
}

void PacketStream::appendPacket(Packet newPacket)
{
	_packetStream.push_back(newPacket);
}

Packet PacketStream::firstPacket()
{
	Packet next = _packetStream.front();
	_packetStream.pop_front();
	return next;
}

void PacketStream::sortPackets()
{
	sort(_packetStream.begin(), _packetStream.end(), arrivalCompare);
}