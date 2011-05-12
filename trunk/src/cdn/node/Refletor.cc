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

#include "Refletor.h"

Define_Module(Refletor);

void Refletor::initialize()
{
	LruCache cache(time(NULL), 1, 1000000000.0, 0);
	this->_cache.push_back(&cache);
}

void Refletor::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

Cache* Refletor::getCache(int cdnId){
	// TODO fazer a consulta
	return NULL;
}

vector<Cache*> Refletor::getCacheVector(){
	return this->_cache;
}
