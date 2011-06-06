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

#ifndef NETCONFIGURATOR_H_
#define NETCONFIGURATOR_H_

#include <FlatNetworkConfigurator.h>
#include <IPAddressResolver.h>
#include <InterfaceEntry.h>
#include <InterfaceTable.h>
#include <IRoutingTable.h>
#include <IPRoute.h>

class NetConfigurator : public FlatNetworkConfigurator{

protected:
	NodeInfoVector* nodeInfo;
public:
	NetConfigurator();
	virtual ~NetConfigurator();
	void configNet();
	void configNode(cModule* mod);
};

#endif /* NETCONFIGURATOR_H_ */
