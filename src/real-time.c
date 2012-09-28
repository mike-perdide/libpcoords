/*
 * Pcoords - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <sebastien@honeynet.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: real-time.c 603 2009-05-24 19:55:36Z toady $
 */

#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pcoords.h>
#include <ev.h>


#define MAX_PCOORDS_MESSAGE_LEN 1024
#ifndef SSIZE_MAX
#define SSIZE_MAX 2147483647
#endif


static ev_io fifo_watcher;
PcoordsImage *image;
void (*fifo_read_callback)(PcoordsImage *image);

static int __pcoords_socket_create(char *fifo)
{
	struct stat st;
	int socket;

	if (lstat (fifo, &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFREG) {
			errno = EEXIST;
			perror("lstat");
			return -1;
		}
	}

	unlink (fifo);
	if (mkfifo (fifo, 0600) == -1) {
		perror("mkfifo");
		return -1;
	}

	socket = open (fifo, O_RDWR | O_NONBLOCK, 0);
	if (socket == -1) {
		perror("open");
		return -1;
	}

	return socket;
}


static void pcoords_fifo_cb(EV_P_ struct ev_io *w, int revents)
{
	ssize_t msglen;
	unsigned char buf[MAX_PCOORDS_MESSAGE_LEN];
	int bufptr = 0; /* What we do not need to get */
	int i = 0;
	unsigned int read_counter = 0;
	char *linebuf;
	PcoordsLine *line;

read_more:
	fprintf(stdout, ".");
	fflush(stdout);

	msglen = read( (struct ev_io *)w->fd, buf + bufptr, MAX_PCOORDS_MESSAGE_LEN - bufptr);
	if ((msglen <= 0) || (msglen > SSIZE_MAX)) {
		perror("read");
		return;
	}
	if (msglen <= 0) {
		fprintf(stderr, "msglen <= 0; Nothing to catch\n");
		return;
	}
	buf[msglen-1] = '\0';

	/* No more to get */
	if (( ! msglen) && (read_counter > 0)) return;

	if ( msglen <= 0 ) {
		fprintf(stderr, "Unexpected closed socket\n");
		ev_io_stop (EV_A_ w);
		ev_unloop (EV_A_ EVUNLOOP_ALL);
		return;
	}

	read_counter++;

more_messages:
	linebuf = malloc(msglen-1);

	while (buf[i] != ';') {
		linebuf[i] = buf[i];
		i++;
	}
	linebuf[i] = ';';
	linebuf[i+1] = '\0';

	line = pcoords_parse_line(linebuf);
	pcoords_image_line_append(image,line);
	fifo_read_callback(image);

/* 	goto read_more; */
}

int pcoords_fifo_data_read(PcoordsImage *template, char *filename, void (*fifo_cb)(PcoordsImage *image))
{
	int sockfd;
	struct ev_loop *pcoords_loop = ev_default_loop (0);

	sockfd = __pcoords_socket_create(filename);
	if ( socket < 0 ) {
		fprintf(stderr, "Cannot create socket!\n");
		return -1;
	}

	image = template;
	fifo_read_callback = fifo_cb;

	ev_io_init(&fifo_watcher, pcoords_fifo_cb, sockfd, EV_READ);
	ev_io_start(pcoords_loop, &fifo_watcher);

	ev_loop(pcoords_loop, 0);

	return 0;
}
