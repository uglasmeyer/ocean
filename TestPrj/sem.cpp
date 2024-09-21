#include <Ocean.h>
#include <Interface.h>
#include <Semaphore.h>

// https://primerpy.medium.com/linux-basics-semaphores-6b1853c02744
#define SEM_KEY 0x1234
#define BUFFER_SIZE 10

Interface_class SDS;
Semaphore_class SEM;

int semval	= 0;
int in 		= 3;
int out 	= 3;
int Count 	= 13;
const uint 	SEMNUM	=  0;
const int	OP_DEC 	= -1;
const int	OP_INC 	=  1;
const int 	OP_WAIT	=  0;
const int	SEM_INIT=  1; // initial semaphore value

void paddr( void* p )
{
	printf( "buffer addr: %p\n", p );
}
void psemval( string who, int id, int semid )
{
    semval = SEM.getval();
    cout << who << " " << id << " ,sem value: " << semval << endl;

}

void produce(int semid, int id)
{
    psemval("producer", id, semid );
    psemval("producer", id, semid );
    SEM.release( );
    SDS.addr->Audio_pid = id;
    SEM.aquire(  );
    SEM.lock(  );
}

void consume(int semid, int id)
{
	psemval("consumer", id, semid );
    SEM.release( );
	printf("%d\n", SDS.addr->Audio_pid );
    SEM.aquire();
    psemval("consumer", id, semid );
    SEM.lock(  );

}

int main()
{
	/*
    if (fork() == 0)
    {
        cout << "start producer" << endl;
        for (int i = 0; i < 5; i++)
        {
            produce(0, i);
        }
    }
    else
    { // start the consumer first
        cout << "start consumer" << endl;
//    	this_thread::sleep_for(2s);
        for (int i = 0; i < 5; i++)
        {
            consume( 0, i);
        }
    }
    return 0;
    */
	if ( fork() == 0 )
	{
		this_thread::sleep_for(2s);
		cout << "release lock" << endl,
		SEM.release();
	}
	else
	{
		SEM.aquire();
		cout << "wait  for release" << endl;
		SEM.lock();


	}
	return 0;
}
