/*
 * SharedSegment.h
 *
 *  Created on: Mar 26, 2009
 *      Author: vasile
 */

#ifndef SHAREDSEGMENT_H_
#define SHAREDSEGMENT_H_


struct jvm_ids {

	map<int,int> jvms;
	//pids,  node_names
	map<int, char*> node_names;

};

#endif /* SHAREDSEGMENT_H_ */
