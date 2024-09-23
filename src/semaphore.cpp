
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
}

void Semaphore_class::aquire()
{
	; // increase the semaphore ( OP_INC )
    sem_op.sem_num 	= SEMNUM; // operate on semaphore 0
    sem_op.sem_op 	= OP_INC; // set value
    sem_op.sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op, 1);
}
void Semaphore_class::release()
{
	;	// decrease the semaphore ( OP_DEC )
    sem_op.sem_num 	= SEMNUM; // operate on semaphore 0
    sem_op.sem_op 	= OP_DEC; // set value
    sem_op.sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op, 1);
}
void Semaphore_class::lock()
{
	;	// wait for release
    sem_op.sem_num 	= SEMNUM; // operate on semaphore 0
    sem_op.sem_op 	= OP_WAIT; // wait for zero
    sem_op.sem_flg 	= OP_WAIT; // IPC_NOWAIT or SEM_UNDO
    semop(semid, &sem_op, 1);

}

int Semaphore_class::getval()
{
	return semctl( semid, SEMNUM, GETVAL, 0 );
}
