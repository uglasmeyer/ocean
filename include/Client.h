/*
 * Client.h
 *
 *  Created on: Sep 6, 2024
 *      Author: sirius
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <Interface.h>
#include <keys.h>

class Client_class : virtual Interface_class
{
public:
	Client_class(){};
	~Client_class(){};

	void toggle_Mute()
	{
	    bool mute_flag 	= not addr->mixer_status.mute ;
	    Set( addr->KEY , MASTERAMP_MUTE_KEY);
	}

};



#endif /* CLIENT_H_ */
