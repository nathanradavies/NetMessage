#include "tcpsock.h"
#include <stdio.h>
#include <errno.h>
#include <iostream.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
// SERVERNAME..could be dotted quad, string or name or NULL for INADDR_ANY

// #define SERVERNAME localhost
// #define SERVERPORT 9999                   

#define EXIT_WITH_ERRNO(string) \
       fprintf (stderr, "%s : %s \n", string, strerror (errno) ); \
       fprintf (stderr, "%s : %d : in %s \n", __FILE__ , __LINE__, \
	   __PRETTY_FUNCTION__ ); \
       exit (1); 

char *ServerName;
int ServerPort;

int main(int argc,char *argv[])	
{
    int nb;

    float totaltime;
    int ntry;
        
    char buf[10000];
    struct timeval maxdifftv, difftv, stv, etv;

    if ( argc != 3 ) {
	    fprintf(stderr, "usage: echoclient servername port "); exit(1); 
    }
        
    ServerName = argv[1];
    ServerPort = atoi( argv[2] );

    tcpsockio echoio( NULL, 0 ); // any name, any port
    cout << "Trying to Make Connection to " << ServerName << " " << 
	    ServerPort << "...";
	
    if ( echoio.connect( ServerName, ServerPort ) != 0 ) {
	    EXIT_WITH_ERRNO( "\nCould Not Connect to Server " );
    }

    cout << "OK" << endl;
    timerclear( &maxdifftv );
    totaltime = 0;
    ntry = 0;
    while( echoio.good() ) {
	    sprintf( buf, "Hello World\n" );
	    nb = sizeof( buf );
//		strlen( buf ) + 1;
	    gettimeofday( &stv, NULL );

	    if ( echoio.send_all( buf, nb ) < 0 )
		    goto EXIT ;

	    if ( echoio.recv_all( buf, nb ) < 0 )
		    goto EXIT ;

	    gettimeofday( &etv, NULL );
	    usleep( 1000 );
	    // timersub,timeradd,timercmp are
	    //   defined in time.h
	    timersub( &etv, &stv, &difftv );

	    totaltime += ( difftv.tv_sec + 1.0e-6 * difftv.tv_usec );
	    ntry++;


        
	    if ( timercmp( &difftv, &maxdifftv, > ) ) {
		    maxdifftv = difftv;
		    printf( "Worst Case Echo = %ld sec %ld usec  Avg = %12.8f sec \n ",
			maxdifftv.tv_sec,
			maxdifftv.tv_usec,
			totaltime/ntry ) ;
	    }
    }

 EXIT:
    cout << "Connection Broken" << endl;
    printf( "%d echoes\n", ntry );
}
	








