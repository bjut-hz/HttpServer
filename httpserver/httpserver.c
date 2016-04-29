#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <assert.h>
#include "threadpool/threadpool.h"
#include "httpcore/httpcore.h"

int startup( u_short* );
void error_die( const char* );

#define THREAD 64
#define QUEUE  2000

int main( void )
{
	int server_sock = -1;
	u_short port = 0;
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof( client_name );
	pthread_t newthread;

	server_sock = startup( &port );
	printf( "httpd running on port %d\n", port );

	threadpool_t* pool = NULL;
	assert( ( pool = threadpool_create( THREAD, QUEUE, 0 ) ) != NULL );
	fprintf( stderr, "Pool started with %d threads and "
			"queue size of %d\n", THREAD, QUEUE );
	while ( 1 )
	{

		client_sock = accept( server_sock,
			( struct sockaddr * )&client_name,
			&client_name_len );
		if ( client_sock == -1 ){
			printf("error_die(accept)");
			error_die( "accept" );
		}

		if ( threadpool_add( pool, &accept_request, (void*)&client_sock, 0 ) != 0 ){
			printf( "Job add error." );
		}
	}
	assert( threadpool_destroy( pool, 0 ) == 0 );
	close( server_sock );
	printf("server_sock close");
	return( 0 );
}

/**********************************************************************/
/* This function starts the process of listening for web connections
* on a specified port.  If the port is 0, then dynamically allocate a
* port and modify the original port variable to reflect the actual
* port.
* Parameters: pointer to variable containing the port to connect on
* Returns: the socket */
/**********************************************************************/
int startup( u_short *port )
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( httpd == -1 )
		error_die( "socket" );
	memset( &name, 0, sizeof( name ) );
	name.sin_family = AF_INET;
	name.sin_port = htons( *port );
	name.sin_addr.s_addr = htonl( INADDR_ANY );
	if ( bind( httpd, ( struct sockaddr * )&name, sizeof( name ) ) < 0 )
		error_die( "bind" );
	if ( *port == 0 )  /* if dynamically allocating a port */
	{
		int namelen = sizeof( name );
		if ( getsockname( httpd, ( struct sockaddr * )&name, &namelen ) == -1 )
			error_die( "getsockname" );
		*port = ntohs( name.sin_port );
	}
	if ( listen( httpd, 5 ) < 0 )
		error_die( "listen" );
	return( httpd );
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
* on value of errno, which indicates system call errors) and exit the
* program indicating an error. */
/**********************************************************************/
void error_die( const char *sc )
{
	printf( sc );
	perror( sc );
	exit( 1 );
}
