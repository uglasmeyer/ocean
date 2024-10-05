/*
 * Statistic.h
 *
 *  Created on: Oct 3, 2024
 *      Author: sirius
 */

#ifndef DATA_STATISTIC_H_
#define DATA_STATISTIC_H_
#include <Ocean.h>
#include <Logfacility.h>

/*
 *
 */
typedef struct statistic_struct
{
	long 	shm 	= 0;
	long 	stereo	= 0;
	long 	data 	= 0;
	long 	total	= 0;
} statistic_t;
extern statistic_t statistic;


class Statistic_class :
	public virtual Logfacility_class
{
	string className = "Statistic_class";

public:

	void Show_Statistic( string );

	Statistic_class();
	virtual ~Statistic_class();
};

#endif /* DATA_STATISTIC_H_ */
