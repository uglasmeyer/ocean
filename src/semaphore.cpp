
#include <Semaphore.h>

Semaphore_class::Semaphore_class() :
Logfacility_class( "Semaphore"),
Config_class( "Semaphore" )
{
	this->SEM_KEY = Config.Sem_key;
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
        semid = semget( SEM_KEY, SEMNUM_SIZE, 0 );
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
	if ( not ( Getval(num, GETVAL) > 0 )) return;
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
