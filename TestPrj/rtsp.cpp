#include <Semaphore.h>


string				Module				= "rtsp";
Logfacility_class	Log( Module );
Semaphore_class		Sem;

void Start_synthesizer()
{
    Log.Comment( INFO, "Start_Synthesizer" );
}

void Stop_synthesizer()
{
	Log.Comment( INFO, "Receiver signal to stop");
}


int main()
{
	Sem.Init();
	while( true )
	{
		Sem.Lock( SEMAPHORE_START );
		Start_synthesizer();
		Sem.Lock( SEMAPHORE_EXIT );
		Stop_synthesizer();
	}
//	exit_thread.join();
	return 0;
}
