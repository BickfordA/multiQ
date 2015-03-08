#include "packetStream.h"

#include <algorithm>


using namespace std;

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

vector<double> PacketStream::calculateInterarrivalTimes()
{
	//this is probably unnessesary
	sort(_packetStream.begin(), _packetStream.end(), arrivalCompare);

	vector<double> interarrivalTimes;

	if (_packetStream.size() < 2){
		return interarrivalTimes;
	}

	Packet previousPacket = _packetStream.front();
	for (int i = 1; i < (int) _packetStream.size(); i++){
		interarrivalTimes.push_back(previousPacket.interarrivalTime(_packetStream[i]));
		previousPacket = _packetStream[i];
	}

	return interarrivalTimes;
}