/*
 * RequestContentCDN.h
 *
 *  Created on: Jun 6, 2011
 *      Author: lucachaves
 */

#ifndef REQUESTCONTENTCDN_H_
#define REQUESTCONTENTCDN_H_

#include <cmodule.h>

class RequestContentCDN {
public:
	int cdnID;
	int segmentID;
	cModule *client;
	RequestContentCDN();
	virtual ~RequestContentCDN();
};

#endif /* REQUESTCONTENTCDN_H_ */
