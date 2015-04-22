#include "bandwidth.h"

#include <algorithm>

using namespace std;

Bandwidth::Bandwidth(vector<Packet> packetStream)
	:_packetStream(packetStream)
{
}

double Bandwidth::measureBandwidth()
{
	vector<double> perPacketBandwidth(_packetStream.size()-1);

	vector<Packet>::const_iterator pp = _packetStream.cbegin();
	vector<Packet>::const_iterator cp = ++_packetStream.cbegin();
	vector<double>::iterator bw = perPacketBandwidth.begin();
	

	while (cp != _packetStream.cend()){

		double interArrivalTime = cp->interarrivalTime(*pp);
		*bw = cp->packetSize() / interArrivalTime * 8; /* 8 bits in a byte */

		//increment
		pp++;
		cp++;
		bw++;
	}

	
	sort(perPacketBandwidth.begin(), perPacketBandwidth.end());


	//discard the bottom 30%
	//and the top 10%

	int topTen = (double) perPacketBandwidth.size() * 0.1;
	int bottomThrity = (double)perPacketBandwidth.size() * 0.3;

	
	double average = 0;
	int count = 0;

	for (int i = bottomThrity; i < perPacketBandwidth.size() - topTen; i++){
		average += perPacketBandwidth.at(i);
		count++;
	}

	average = average / count;

	return average;
}