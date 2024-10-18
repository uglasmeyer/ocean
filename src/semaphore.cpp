
#include <data/Semaphore.h>

Semaphore_class::Semaphore_class( Config_class* Cfg) :
Logfacility_class( "Semaphore"),
Time_class()
{
	this->SEM_KEY = Cfg->Config.Sem_key;
	Set_Loglevel( DEBUG, false);
	init();
}

Semaphore_class::~Semaphore_class()
{
}

void Semaphore_class::Semop( const unsigned short& num, const short int& sop )
{
	struct sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= sop,
		.sem_flg 	= 0
	};
    int ret = semop(semid, &op, N_OPS );
    string text = "";
    if ( sop < 0 ) text = "Release ";
    if ( sop ==0 ) text = "Lock    ";
    if ( sop > 0 ) text = "Aquire  ";
    Comment( DEBUG, text + Stat( num ));
    if ( ret < 0 )
    {
    	Comment( ERROR, to_string( ret ) + " " + Error_text( errno )  );
        Comment( ERROR, Stat( num ));
    }
    assert( ret == 0 );
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
    	Semop( n, OP_WAIT );
    }
}

void Semaphore_class::Reset( uint8_t num )
{
//	short int val = -abs(  Getval( num , GETVAL ) );
	int ret = semctl(semid, num, SETVAL, 0 );
	assert( ret == 0 );
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
	Semop( num, OP_INC );
}
void Semaphore_class::Release( uint8_t num)
{
	;	// decrease the semaphore ( OP_DEC )
	if ( not ( Getval(num, GETVAL) > 0 ) ) return;
	Semop( num, OP_DEC );
}

void Semaphore_class::Lock( uint8_t num )
{
	;	// wait for release
	Aquire( num );
	Semop( num, OP_WAIT );
}

void ReleaseProxy_fnc( 	Semaphore_class* sem,
				uint semaphore,
				uint timeout )
{
	sem->Comment( TEST, "proxyfnc started");
	sem->lock_timer.Wait( timeout );
	sem->Comment( TEST, "proxyfnc released");
	sem->Release( semaphore );
}
void Semaphore_class::Lock( uint8_t num, uint timeout )
{
	;	// wait for release
    thread ReleaseProxy_thread (	ReleaseProxy_fnc, this, num, timeout );
	Aquire( num );
	Semop( num, OP_WAIT );
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
		strs <<
				"num " << setw(4) << (int) num 				<< " " <<
				"pid " << setw(8) << Getval( num, GETPID ) 	<< " " <<
				"val " << setw(4) << Getval( num, GETVAL ) 	<< " " <<
				"ncn " << setw(4) << Getval( num, GETNCNT ) 	<< " " <<
				"zcn " << setw(4) << Getval( num, GETZCNT ) 	;
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
		return stat( num ) ;
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
	stringstream strs;
	strs << "time elapsed " << tel << " ms" ;
	Info( strs.str());
	TEST_END( className );
}

