
#include <data/Semaphore.h>

Semaphore_class::Semaphore_class( key_t key) :
Logfacility_class( "Semaphore"),
Time_class()
{
	this->SEM_KEY = key;//Cfg->Config.Sem_key;
	init();
}

Semaphore_class::~Semaphore_class()
{
}

void Semaphore_class::Semop( const unsigned short& num, const short int& sop )
{
	sembuf op =
	{
		.sem_num 	= num,
		.sem_op 	= sop,
		.sem_flg 	= 0
	};

    int 		ret = semop(semid, &op, N_OPS );
    string 		txt	= ( sop == 0 ) ? "lock" : "Release ";
    if ( sop > 0 )
    			txt = "Aquire  ";
    Comment( DEBUG, txt + State( num ))
    ;
    if ( ret < 0 )
    {
    	Comment( ERROR, to_string( ret ) + " " + Error_text( errno )  );
        Comment( ERROR, State( num ));
    }
//    assert( ret == 0 );
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
    Comment( DEBUG, "Reset   ", State( num ));
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
    State( SEMNUM_SIZE );
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

void ReleaseProxy_fnc( 	Semaphore_class* 	sem,
						uint 				semaphore,
						uint 				wait_timeout )
{
	sem->Comment( TEST, "proxyfnc started");
	sem->Locktimer.Wait( wait_timeout );
	sem->Comment( TEST, "proxyfnc released");
	sem->Release( semaphore );
}


bool Semaphore_class::Lock( uint8_t num, uint timeout )
{
	;	// wait for release
	thread ReleaseProxy_thread (	ReleaseProxy_fnc, this, num, timeout );
	ReleaseProxy_thread.detach();
	Aquire( num );
	Semop( num, OP_WAIT );
//    ReleaseProxy_thread.join();
    return true;
}

int Semaphore_class::Getval( uint8_t num, int op )
{
	assert( num < SEMNUM_SIZE );
	return semctl( semid, num, op, UNUSED );
}
string Semaphore_class::State(uint8_t num)
{

	if( not LogMask[ TEST ] ) return "";
	Table_class Table { };
	Table.AddColumn("num", 4);
	Table.AddColumn("Name", 20);
	Table.AddColumn("pid", 8);
	Table.AddColumn("val", 4);
	Table.AddColumn("ncn", 4);
	Table.AddColumn("zcn", 4);

	auto cout_semstate = [ & ](uint num)
	{
		if ( LogMask[TEST] )
			Table.AddRow(	num,
							semnum_map[ num ],
							Getval(num, GETPID),
							Getval(num, GETVAL),
							Getval(num, GETNCNT),
							Getval(num, GETZCNT));
	};

	if ( LogMask[ TEST ] )
		Table.PrintHeader();

	if (num == SEMNUM_SIZE)
	{
		for (uint n = 0; n < SEMNUM_SIZE; n++)
			cout_semstate(n);
		return "";
	}
	else
	{
		cout_semstate(num);
		return "";
	}
}

void Semaphore_class::Test()
{
	TEST_START( className );

	Comment( TEST, "Semaphore with timeout" );
	Reset( SEMAPHORE_TEST );
	Time_class t{};

	t.Start();
	this->Lock( SEMAPHORE_TEST, 2 );
	long tel = t.Time_elapsed();

	Info( "time elapsed ", to_string(tel), " [ms]" );
	ASSERTION( tel - 2001 < 50, "timeout", (long)tel, "<2050" );

	TEST_END( className );
}

