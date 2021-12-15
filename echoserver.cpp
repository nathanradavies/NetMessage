#include "tcpsock.h"
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

// SERVERNAME..could be dotted quad,string or name or NULL for INADDR_ANY

#define SERVERNAME NULL
#define SERVERPORT 9999                   

#define EXIT_WITH_ERRNO(string) \
       fprintf (stderr, "%s : %d : in %s \n", __FILE__ , __LINE__,\
	   __PRETTY_FUNCTION__ ); \
       fprintf (stderr, "%s : %s \n", string, strerror (errno) ); \
           exit (1); 

int main()
{
    int nread;
    tcpsockio echoio( NULL, 0, 1 );  // name,port,dont-bind=true
    echoio.reuseaddr( 1 );
    if ( echoio.bind( SERVERNAME, SERVERPORT ) != 0 ) {
	    EXIT_WITH_ERRNO( " Could Not Bind to MASTER_PORT " );
    }

    echoio.listen( 2 );						// que-size=2

 LOOP:
    int sd = echoio.accept();
    {
	    tcpsockio clientio( sd );
	    char dummybuf[10000];
	    while ( clientio.good() ) {
		    nread=clientio.recv_avail( dummybuf, sizeof( dummybuf ) );
		    if ( nread > 0 )
			    clientio.send_all( dummybuf, nread );
	    }
    }

    cout << "Slave Connection Broken" << endl;
    goto LOOP;	
}

