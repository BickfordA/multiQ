#ifndef BANDWIDTH_H
#define BANDWIDTH_H

#include <vector>
#include "../parser/packet.h"


class Bandwidth
{

	//Given a set of packets and their
	//corresponding size and arrival time
	//calculate the per packet bandwith as the
	//size fo the packet devided by the amoutn fo time between it
	//and the preceding packet. 

	//Discard the top ten percent and the bottom 30 percent
	//source: https://support.speedtest.net/hc/en-us/articles/203845400-How-does-the-test-itself-work-How-is-the-result-calculated-

public:

	Bandwidth(std::vector<Packet> packetStream);

	double measureBandwidth();


	std::vector<Packet> _packetStream;
};



#endif // BANDWIDTH_H