
#include <data/Semaphore.h>

Semaphore_class::Semaphore_class( Config_class* Cfg) :
Logfacility_class( "Semaphore"),
Time_class()
{
	this->SEM_KEY = Cfg->Config.Sem_key;
//	Set_Loglevel( DEBUG, true);
	init();

}

Semaphore_class::~Semaphore_class()
{
}

void Semaphore_class::Init()
{
	for ( uint8_t n = 0; n < SEMNUM_SIZE ; n++ )
    {
    	if ( semctl(semid, n, SETVAL, 0 ) < 0) // release semaphore set
        {
            perror("semctl");
            exit( 1 );
        }
    	struct sembuf op =
    	{
    		.sem_num 	= n,
    		.sem_op 	= OP_WAIT,
    		.sem_flg 	= 0
    	};
        int ret = semop(semid, &op, N_OPS );

        assert( ret == 0 );
    }
}
void Semaphore_class::init()
{
	Comment( INFO, "initializing the semaphore facility");
	Comment( INFO, "using semaphore key " + to_string( SEM_KEY));
    semid = semget( SEM_KEY, SEMNUM_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if( errno == EEXIST )
    {
        semid = semget( SEM_KEY, SEMNUM_SIZE, SEM_INIT );
    }
    else
    {
    	Comment( INFO, "creating semaphore set " + to_string( semid ));
    }
    if (semid < 0)
    {
        perror("semget");
        exit( 1 );
    }
    Stat( SEMNUM_SIZE );
	Comment( INFO, "attached to semaphore set " + to_string( semid ));
}

void Semaphore_class::Aquire( uint8_t num )
{
	; // increase the semaphore ( OP_INC )
	struct sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= OP_INC,
		.sem_flg 	= IPC_NOWAIT
	};
    int ret = semop(semid, &op, N_OPS );
    if ( ret )
   	{
    	Comment( ERROR, to_string( ret ) + " " + Error_text( errno )  );
        Comment( ERROR, Stat( num ));
   	}
    assert( ret == 0 );
    Comment( DEBUG, "aquire  " + Stat( num ));

}
void Semaphore_class::Release( uint8_t num)
{
	;	// decrease the semaphore ( OP_DEC )
	if ( not ( Getval(num, GETVAL) > 0 ) ) return;
	struct sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= OP_DEC,
		.sem_flg 	= OP_WAIT
	};
    int ret = semop(semid, &op, N_OPS );
    assert( ret == 0 );
    Comment( DEBUG, "release " + Stat( num ));
}


void Semaphore_class::Lock( uint8_t num )
{
	;	// wait for release
	Aquire( num );
    struct sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= OP_WAIT,
		.sem_flg 	= OP_WAIT
	};

    int ret = semop(semid, &op, N_OPS );
    assert( ret == 0 );
    Comment( DEBUG, "lock    " + Stat( num ));

}

void RelwaseProxy_fnc( 	Semaphore_class* sem,
				uint semaphore,
				uint timeout )
{
	sem->Comment( TEST, "proxyfnc started");
	sem->lock_timer.Wait( timeout );
	sem->Release( semaphore );
	sem->Comment( TEST, "proxyfnc released");

}
void Semaphore_class::Lock( uint8_t num, uint timeout )
{
	;	// wait for release
    Comment( DEBUG, "lock    " + Stat( num ));
    thread ReleaseProxy_thread (	RelwaseProxy_fnc, this, num, timeout );
	Aquire( num );
    struct sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= OP_WAIT,
		.sem_flg 	= OP_WAIT
	};

    int ret = semop(semid, &op, N_OPS );
    assert( ret == 0 );
    ReleaseProxy_thread.join();

}

int Semaphore_class::Getval( uint8_t num, int op )
{
	assert( num < SEMNUM_SIZE );
	return semctl( semid, num, op, UNUSED );
}

string Semaphore_class::Stat( uint8_t num )
{
	auto stat = [ this ]( uint num )
	{
		stringstream strs;
		strs << setw(4) << (int) num 	<< " " <<
				Getval( num, GETPID ) 	<< " " <<
				Getval( num, GETVAL ) 	<< " " <<
				Getval( num, GETNCNT ) 	<< " " <<
				Getval( num, GETZCNT ) 	;
		return strs.str();
	};

	if ( num == SEMNUM_SIZE )
	{
		for ( uint n = 0; n < SEMNUM_SIZE; n++ )
			Comment( INFO,  stat( n ) );
		return "";
	}
	else
	{
		return stat( num ) + "\n";
	}

}

void Semaphore_class::Test()
{
	TEST_START( className );
	Set_Loglevel( DEBUG, true);

	Comment( TEST, "Semaphore with rimeout" );
	Time_class t{};
	t.Start();
	this->Lock( SEMAPHORE_TEST, 2 );
	long tel = t.Time_elapsed();
	cout << "time elapsed " << tel << " ms" << endl;
	TEST_END( className );
}

