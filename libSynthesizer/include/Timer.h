/*
 * Timer.h
 *
 *  Created on: Feb 7, 2024
 *      Author: sirius
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>
#include <unistd.h>
#include <functional>
#include <chrono>

using namespace std;

class Timer
{

public:
	uint interval;
	typedef function<void(void)> Worker;
	Worker workfunction;
	bool running = false;
		Timer( Worker, uint );
	virtual ~Timer();
	void start();
	void stop();
	void Loop(int);
};

#endif /* TIMER_H_ */
