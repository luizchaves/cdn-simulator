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

#include "NetConfigurator.h"

Define_Module(NetConfigurator);

NetConfigurator::NetConfigurator() {
	// TODO Auto-generated constructor stub

}

NetConfigurator::~NetConfigurator() {
	// TODO Auto-generated destructor stub
}

void NetConfigurator::configNet(){
	cTopology topo("topo");
	// will be of size topo.nodes[]
	NodeInfoVector nodeInfoNet;
	nodeInfo = &nodeInfoNet;

	// extract topology into the cTopology object, then fill in
	// isIPNode, rt and ift members of nodeInfo[]
	extractTopology(topo, nodeInfoNet);

	// assign addresses to IP nodes, and also store result in nodeInfo[].address
	assignAddresses(topo, nodeInfoNet);

	// add default routes to hosts (nodes with a single attachment);
	// also remember result in nodeInfo[].usesDefaultRoute
	addDefaultRoutes(topo, nodeInfoNet);

	// calculate shortest paths, and add corresponding static routes
	fillRoutingTables(topo, nodeInfoNet);

	// update display string
	setDisplayString(topo, nodeInfoNet);
}

void NetConfigurator::configNode(cModule* mod){
	//TODO Que erro é esse!!!!!!!!!!!!!!
	cTopology topo("topo");
	NodeInfoVector nodeInfoNet;
	extractTopology(topo, nodeInfoNet);
	assignAddresses(topo, nodeInfoNet);
	addDefaultRoutes(topo, nodeInfoNet);
	//TODO não repetir
	fillRoutingTables(topo, nodeInfoNet);
	setDisplayString(topo, nodeInfoNet);



}
