//  tcpsock.h. -- header for socket class
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

#ifndef __TCPSOCK_H
#define __TCPSOCK_H

#ifndef _WIN32
    #include <unistd.h>
#endif

class tcpsockio
{
public:
	enum statebits {
		goodbit=0,
		eofbit=1,
		failbit=2,
		badbit=4
	};

	tcpsockio ();
	tcpsockio (int sockfd);
	tcpsockio (const char *my_name, int port, int dont_bind =0);

	void init();

	void setsocket(int sockfd);
	void setsocket(const char *my_name, int port, int dont_bind =0);
	void closesocket();
	
	virtual	~tcpsockio ();

	int connect(const char *remote_name, int remote_port);
	// returns the name (0 terminated and truncated if necessary 
	// in remote-name ..NULL can be used..);
	int isconnected(char *remote_name, int len);

	int bind(const char *name, int port);   // non-zero if error 
        // localhostname=NULL for wildcard..port=0 for wildcard
	
	int reuseaddr( int opt);

	int listen(int que_size);
	int accept();

	int recv_all(void *buf, size_t n, int flags=0);   
	int send_all(void *buf, size_t n, int flags=0);   
	int recv_avail(void *buf, size_t n, int flags=0);
	
	int read_ready(int sec, int usec);
	
	int good() const { return state == 0; }
	int eof() const { return state & tcpsockio::eofbit; }
	int fail() const { return state & (tcpsockio::badbit|tcpsockio::failbit); }
	int bad() const { return state & tcpsockio::badbit; }

	void clear() { state=0; lst_errno=0; }
	
	unsigned wcount;   // count of sent characters in last send()
	unsigned rcount;   // count of read characters in last read()
	
	unsigned state;
	int lst_errno;     // last errno
	int     sd;        // socket-descriptor (-1 for error);
};

#endif
