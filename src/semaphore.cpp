
#include <Semaphore.h>

Semaphore_class::Semaphore_class() :
Logfacility_class( "Semaphore")
{
	init();
}

Semaphore_class::~Semaphore_class()
{
	semctl( semid, IPC_RMID, 0 );
}

void Semaphore_class::init()
{
	Comment( INFO, "initializing the semaphore facility");
    semid = semget(SEM_KEY, 1, IPC_CREAT | S_IRUSR | S_IWUSR );
    if (semid < 0)
    {
        perror("semget");
        exit( 1 );
    }


    if (semctl(semid, SEMNUM, SETVAL, SEM_INIT) < 0)
    {
        perror("semctl");
        exit( 1 );
    }
    for ( int n = 0; n < SEMNUM_SIZE; n++ )
    {
    	sem_op.sem_num = n;
    	sem_op.sem_flg = OP_WAIT;
    	sem_op_vec.push_back( sem_op );
    }
}

void Semaphore_class::aquire( uint num )
{
	; // increase the semaphore ( OP_INC )
 //   semop_vec[num].sem_num 	= num; // operate on semaphore num
    sem_op_vec[num].sem_op 	= OP_INC; // set value
 //   semop_vec[num].sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op_vec[num], 1);
}
void Semaphore_class::release( uint num)
{
	;	// decrease the semaphore ( OP_DEC )
	//    semop_vec[num].sem_num 	= num; // operate on semaphore num
    sem_op_vec[num].sem_op 	= OP_DEC; // set value
    //    semop_vec[num].sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op_vec[num], 1);
}
void Semaphore_class::lock( uint num )
{
	;	// wait for release
	//    semop_vec[num].sem_num 	= num; // operate on semaphore num
    sem_op_vec[num].sem_op 	= OP_WAIT; // wait for zero
    //    semop_vec[num].sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op_vec[num], 1);
}

int Semaphore_class::getval( uint num )
{
	return semctl( semid, num, GETVAL, 0 );
}
