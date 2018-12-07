/*
 * Kmean.h
 *
 *  Created on: Nov 24, 2018
 *      Author: user
 */

#ifndef KMEAN_H_
#define KMEAN_H_
#include <avr/io.h>
#include <vector>

class Kmean {
public:
	Kmean();
	virtual ~Kmean();
	void iterate(void);
	void addItem();
private:

};

#endif /* KMEAN_H_ */
