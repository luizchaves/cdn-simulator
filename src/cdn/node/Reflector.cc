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

#include "Reflector.h"

Define_Module(Reflector);

void Reflector::initialize()
{
	//TODO Create outros cache de cdn
	LruCache cache(time(NULL), 1, 1000000000.0, 0);
	this->_cacheVector.push_back(&cache);
	for(vector<Cache*>::iterator it = this->_cacheVector.begin(); it < this->_cacheVector.end(); it++){
		Cache* c = (*it);
		std::cout << "<->" << c->getCDNId() << endl;
	}
}

void Reflector::handleMessage(cMessage *msg)
{
    // TODO se chegar um solicitação de segmento que não exista cria um cache ou colocar no cache existente
}

Cache* Reflector::getCache(int cdnId){
	for(vector<Cache*>::iterator it = this->_cacheVector.begin(); it != this->_cacheVector.end(); it++)
		if((*it)->getCDNId() == cdnId)
			return (*it);
	return NULL;
}

vector<Cache*> Reflector::getCacheVector(){
	return this->_cacheVector;
}
