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

#ifndef __CDN_PROCESSOR_H_
#define __CDN_PROCESSOR_H_

#include <omnetpp.h>
#include <VideoSet.h>
#include <Cache.h>
#include <IPvXAddress.h>
#include <Storage.h>
#include <Reflector.h>
#include <UDPSocket.h>
#include <UDPControlInfo_m.h>

/**
 * TODO - Generated class
 */
class Processor : public cSimpleModule
{
  public:
	void bindToPort(int port);
	void sendToUDP(cPacket *msg,
			int srcPort, const IPvXAddress& destAddr, int destPort);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};

#endif
