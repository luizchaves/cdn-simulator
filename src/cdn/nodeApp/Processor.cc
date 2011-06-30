//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Processor.h"

Define_Module(Processor);

void Processor::initialize()
{
	bindToPort(1000);
}

void Processor::networkProcessor(){

}

void Processor::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}


void Processor::bindToPort(int port) {
	EV<< "Binding to UDP port " << port << endl;

	// TODO UDPAppBase should be ported to use UDPSocket sometime, but for now
	// we just manage the UDP socket by hand...
	cMessage *msg = new cMessage("UDP_C_BIND", UDP_C_BIND);
	UDPControlInfo *ctrl = new UDPControlInfo();
	ctrl->setSrcPort(port);
	ctrl->setSockId(UDPSocket::generateSocketId());
	msg->setControlInfo(ctrl);
	send(msg, "udpOut");

}

void Processor::sendToUDP(cPacket *msg,
		int srcPort, const IPvXAddress& destAddr, int destPort) {
	// send message to UDP, with the appropriate control info attached
	msg->setKind(UDP_C_DATA);

	UDPControlInfo *ctrl = new UDPControlInfo();
	ctrl->setSrcPort(srcPort);
	ctrl->setDestAddr(destAddr);
	ctrl->setDestPort(destPort);
	msg->setControlInfo(ctrl);

	EV<< "Sending packet: ";
	//printPacket(msg);

	send(msg, "udpOut");
}

