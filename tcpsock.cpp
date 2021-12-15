//  tcpsock.cpp -- socket io class
//  Copyright (C) 1998 Larry Winiarski 
 
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#define __USE_GNU
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include "tcpsock.h"

tcpsockio::tcpsockio() 
{
	init();
}

tcpsockio::tcpsockio(int sockfd)
{
	init();
	setsocket(sockfd);
}

tcpsockio::tcpsockio (const char *name, int port, int dont_bind)
{
	init();
	setsocket(name, port, dont_bind);
}

tcpsockio::~tcpsockio () 
{
	closesocket();
}

void tcpsockio::init()
{
	sd = -1;
	lst_errno=0;
	state=tcpsockio::badbit;
	wcount=rcount=0;
}

void tcpsockio::setsocket(int sockfd)
{
	// if old socket..close it first..
	if (sd >= 0) closesocket();

	if (sockfd < 0) {
		lst_errno=errno;
		state|=tcpsockio::badbit;
	} else {
		lst_errno=0;
		state=0;
	}
	wcount=rcount=0;
	sd=sockfd;
}

void tcpsockio::setsocket(const char *name, int port, int dont_bind )
{
	if (sd >= 0) closesocket();

	sd = socket (PF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		cerr << "Cannot create socket " << endl;
		lst_errno=errno;
		state|=tcpsockio::badbit;
		return;
	}
	if (dont_bind) goto EXIT;

	if (bind (name, port) < 0) {
		cerr << "Cannot Bind to " << name << endl;
		lst_errno=errno;
		state|=tcpsockio::badbit;
		return;
	}
EXIT:
	lst_errno=0;
	state=0;
}

void tcpsockio::closesocket()
{
	if (sd >= 0) {
		if (close(sd) < 0) {
			perror("Close Error");
		}
	}
	init();
}

// localhostname=NULL for wildcard..port=0 for wildcard

int tcpsockio::bind(const char *localhostname, int port)
{
	struct sockaddr_in addr;
	struct hostent *hostinfo;
		
	bzero( &addr,  sizeof (addr) );
 
	addr.sin_family = AF_INET;
	addr.sin_port = htons (port);
		
	if (localhostname == NULL) { // use inaddr_any
		addr.sin_addr.s_addr = htonl (INADDR_ANY);  // use localhost
	} else {
		hostinfo = ::gethostbyname (localhostname);
       
		if (hostinfo == NULL)  {
			cerr << "Unknown host: " << localhostname << endl;
			lst_errno=errno;
			state|=tcpsockio::badbit;
			return (-1);
		}
       

		addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	}
	
	if (::bind (sd, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
		cerr << "Cannot Bind to " << localhostname << endl;
		lst_errno=errno;
		state|=tcpsockio::failbit;
		return (-1);
	}
	lst_errno=0;
	state=0;
	return(0);
}
	
int tcpsockio::connect(const char *remote_name, int remote_port)
{
        struct sockaddr_in addr;
	struct hostent *hostinfo;

	addr.sin_family = AF_INET;
	addr.sin_port = htons (remote_port);
	hostinfo = ::gethostbyname (remote_name);
       
	if (hostinfo == NULL)  {
		cerr << "Unknown host: " << remote_name << endl;
		lst_errno=errno;
		state|=tcpsockio::failbit;
		return -1;
	}
       
	addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
//        addr.sin_addr.s_addr = htonl (INADDR_ANY);	

	
	if (::connect (sd, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
		cerr << "Cannot Connect to " << remote_name << endl;
		lst_errno=errno;
		state|=tcpsockio::failbit;
		return (-1);
	}
	return(0);
}

// returns the name (0 terminated and truncated if necessary 
// in remote-name ..NULL can be used if you really don't want the name);
int tcpsockio::isconnected(char *remote_name, int len)
{
	struct sockaddr_in peer;
	size_t peerlen;
	struct hostent *hostinfo;

	peerlen=sizeof(peer);
	if (getpeername(sd, (struct sockaddr *) &peer, &peerlen)) {
		if (remote_name && len > 0) remote_name[0]='\0';
		return (0);
	}
	if (!remote_name || len <= 0) return(1); // name not wanted
	// some peer found..do dotted quad.
	strncpy(remote_name,inet_ntoa(peer.sin_addr),len);
	
	hostinfo = ::gethostbyaddr (remote_name, strlen(remote_name),
				    AF_INET);
	if (hostinfo == NULL)  {   // error..just return dotted quad
		return(1);
	}
	strncpy(remote_name,hostinfo->h_name,len);
	return(1);
}

int tcpsockio::listen(int que_size)
{
	if (::listen (sd, que_size) < 0) {
		cerr << "Listen Call Failed" << endl;
		state|=(tcpsockio::failbit|tcpsockio::badbit);
		lst_errno=errno;
		return (errno);
	}
	return (0);
}

int tcpsockio::accept()
{
	int new_sd;
	
	struct sockaddr_in clientname;
	unsigned int size;
	size=sizeof(clientname);
       
	while ( (new_sd = ::accept(sd,  (struct sockaddr *) &clientname,  &size) ) < 0 &&
		errno == EINTR) ;  // keep going if EINTR  
	if (new_sd < 0) {
		cerr << "Accept Error: " << strerror(errno) << endl;
		state |=(tcpsockio::failbit);
		lst_errno=errno;
	}
	return (new_sd);
}

// if opt==-1 then just return setting..

int tcpsockio::reuseaddr( int opt)
{
	int old_opt=0;
	unsigned int opt_size;


	opt_size = sizeof(old_opt);
	if (getsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &old_opt, &opt_size ) < 0) {
		state|=failbit;
		cerr << "getsockopt: " << strerror(errno) << endl;
		lst_errno=errno;
	}

	if (opt !=-1) {
		opt_size=sizeof (opt);
		if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &opt, opt_size ) < 0) {
			state|=failbit;
			lst_errno=errno;
			cerr << "setsockopt: " << strerror(errno) << endl;
		}
		
	}
	return old_opt;
}

// return n for num-read; -1 for error..
int tcpsockio::recv_all(void *buf, size_t n, int flags)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = (char *) buf;
	rcount=0;
	nleft = n;

	while (nleft > 0) {
		if (flags == 0) nread = ::read(sd, ptr, nleft);
		else  nread = ::recv(sd, ptr, nleft, flags);
		if ( nread < 0 ) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else {
				state|=failbit;
				lst_errno=errno;
				return(-1);
			}
			
		} else if (nread == 0) {
			state|=eofbit;
			break;				/* EOF */
		}
		

		nleft -= nread;
		ptr   += nread;
		rcount += nread;
		
	}
	return(rcount);		/* return >= 0 */
}
/* end recv_all */

int tcpsockio::send_all(void *buf, size_t n, int flags)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = (char *) buf;
	wcount=0;
	nleft = n;

	while (nleft > 0) {
		if (flags == 0) nwritten = ::write(sd, ptr, nleft);
		else  nwritten = ::send(sd, ptr, nleft, flags);
		if (nwritten <= 0) {
			if (errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else {
				state|=failbit;
				lst_errno=errno;
				return(-1);			/* error */
			}
		}
			
		nleft -= nwritten;
		ptr   += nwritten;
		wcount += nwritten;
	}
	return(wcount);
}
/* end writen */

// return n for num-read; -1 for error..
int tcpsockio::recv_avail(void *buf, size_t n, int flags)
{
	ssize_t	nread;
	rcount=0;
	if ( (nread = ::recv(sd, buf, n, flags)) < 0) {
		if (errno == EINTR)
			nread = 0;		/* and call read() again */
		else {
			state|=failbit;
			lst_errno=errno;
			return(-1);
		}
		
	} else if (nread == 0) {
		state|=eofbit;
	}
	rcount=nread;
	return(nread);
}

/* end recv_avail*/


// -1 on err..0 on timeout..>0 data ready
int tcpsockio::read_ready(int sec, int usec)
{
	fd_set rfds,wfds,errfds;
	timeval tv;
	int retval;
	
	FD_ZERO(&rfds); FD_ZERO(&wfds); FD_ZERO(&errfds);
	FD_SET( sd, &rfds );

	tv.tv_sec = sec;
	tv.tv_usec = usec;
	
	if ((retval = select (sd+1, &rfds, NULL, NULL, &tv)) < 0) {
		cerr << "select error" << endl;
		return (-1);
	}
	
	return (retval);
	
	/* FD_ISSET(0, &rfds) will be true. */
}

	
	






