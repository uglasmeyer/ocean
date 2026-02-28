#include <Appsymbols.h>

void exit_proc( int signal)
{
	cout << "Exit class " << endl;
	exit( signal );
}
#include <string.h>
void SharedData_class::test_reset_ifd()
{	// copy default values into sds memory

	Comment(INFO, "Reset shared data", sds_size, "Bytes" );
	*addr = ifd_data;
//	memcpy( addr	, &ifd_data		, sds_size );
//	addr->connect_arr	= initConnect_arr();
//	addr->StA_state_arr[ STA_INSTRUMENT ].play = true;
//	SharedData_class::selfTest();

	Dump_ifd( );
}
int main()
{
	Sds->test_reset_ifd();
//	Sds->Dump_ifd();
	Exit.Comment( INFO, "Do nothing" );
	exit( 0 );
}
