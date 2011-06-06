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

#include "Client.h"

Define_Module(Client);

void Client::initialize()
{
	scheduleAt(simTime(), new cMessage());
}

void Client::handleMessage(cMessage *msg)
{
	if (!msg->isSelfMessage())
		error("This module does not process messages.");

	delete msg;

	cModule* moduleIndexer;
	for (cModule::SubmoduleIterator iter(
			getParentModule()->getParentModule()); !iter.end(); iter++) {
		if (strcmp(iter()->getModuleType()->getName(), "CDNNode") == 0) {
			//TODO usar algum critério de seleção
			if (!strcmp(((cModule*) iter())->par("type"), "i")) {
				moduleIndexer = iter();
				break;
			}
		}
	}
	std::cout << "Module Name " << moduleIndexer->getFullName() << endl;
	bindToPort(1000);
	cPacket *newMsg = new cPacket();
	IPvXAddress serverAddress = IPAddressResolver().addressOf(moduleIndexer);
	std::cout << "Requesting video stream from " << serverAddress << ":" << 1000 << "\n";
	sendToUDP(newMsg, 1000, serverAddress, 1000);
}

void Client::bindToPort(int port) {
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

void Client::sendToUDP(cPacket *msg,
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
