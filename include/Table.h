/*
 * Table.h
 *
 *  Created on: Mar 10, 2025
 *      Author: sirius
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <Logfacility.h>

typedef struct tableOpt_struct
{
	uint 		Ident 		= 0;
	string		Titel 		= "";
	fstream* 	FILE 		= nullptr;
	char 		Separator 	= ' ';
	char		Crlf		= '\n';
} tableopt_t;
const tableopt_t	defaultopt	= tableOpt_struct();


class Table_class :
	virtual Logfacility_class
{
	string		 		className 	= "";

	struct header_struct
	{
		string 	txt;
		uint 	width;
	};
	typedef header_struct header_t;
	vector<header_t> header_v {};

public:
	tableopt_t 			opt 		= tableOpt_struct();

	Table_class( string title = "", uint ident = LOGINDENT );
	Table_class( tableopt_t _opt );
	virtual ~Table_class();

	void AddColumn( string name, uint width );

	template< typename... Args>
	string AddRow( Args... args)
	{
		uint argc = sizeof...(args);
		if ( argc > header_v.size() )
		{
			Comment(ERROR, "Invalid number of columns: ", argc, " expected: ", header_v.size());
			return "";
		}
		string color 	= GetColor( TABLE );
		string endc		= GetendColor();
		stringstream strs {};
		uint n	= 0;

		strs << setw( opt.Ident ) << dec << left << "" ;
		(( strs << setw(header_v[n++].width)
				<< args
				<< opt.Separator
				), ...);

		string tableline =strs.str();
		string colorline = color + tableline + endc;

		cout << colorline << opt.Crlf;

		if( opt.FILE )
		{
			if( not opt.FILE->is_open() )
			{
				Comment(WARN, "file not open");
				return strs.str();
			}

			*opt.FILE << tableline << "\n";
		}
		return tableline;

	};

	void PrintHeader();
	void TestTable();

private:
};



#endif /* TABLE_H_ */
