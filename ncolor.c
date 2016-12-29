/*

Copyright 2016 Matt Arnold

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL,
DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BUFFSIZE 256

void sigproc();
void sigcnt();
void sigstp();
int main(int argc, char *argv[])
{
	char *pbuf;
	char tbuf[BUFFSIZE];
	const char *tf = "[%H:%M:%S]:";
	bool cln, cls, twn;
	int opt;
	int skip = 1;
	cln = cls = twn = false;

	while ((opt = getopt(argc, argv, "cst:a:")) != -1) {
		switch (opt) {
		case 'c':
			cls = true;
			break;
		case 't':
			cln = true;
			asprintf(&pbuf, "%s", optarg);
			// Even better than strlcpy this

			break;
		case 's':
			twn = true;
			break;

		case 'a':
			printf("%s\n", optarg);
			skip = atoi(optarg);

			if (skip < 1) {
				skip = 1;
				twn = true;
			}
			break;
		default:
			// all options optional
			fprintf(stderr, "Usage: %s [-t colorspec] [-c]",
				argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (cln) {
		setenv("PIPE_COLOR", pbuf, 1);
		printf("%s%s\n", "\x1b[", pbuf);

	} else {

		pbuf = getenv("PIPE_COLOR");
		if (pbuf != NULL) {
			printf("%s%s\n", "\x1b[", pbuf);

		} else {
			fprintf(stderr, "%s\n",
				"Must set ansi code in PIPE_COLOR");
			exit(1);
		}
	}

	if (cls) {
		printf("%s", "\x1b[2J");
	}

	char c;
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	signal(SIGCONT, sigcnt);
	signal(SIGSTOP, sigstp);

	time_t start = time(NULL);
	bool tsn = false;
	int lc = 1;
	while ((c = fgetc(stdin)) != EOF) {
		if (tsn && !twn) {
			time_t now = time(NULL);
			strftime(tbuf, sizeof tbuf, tf, localtime(&now));
			printf("%s ", tbuf);
		}
		fputc(c, stdout);
		lc++;
		tsn = (c == '\n' && lc % skip == 0) ? true : false;
	}
	puts("\x1b[0m");
}

void sigproc()
{
	fputs("\x1b[0m", stdout);
	exit(0);
}

void sigstp()
{
	fputs("\x1b[0m", stdout);
}

void sigcnt()
{
	char *pbuf = getenv("PIPE_COLOR");
	if (pbuf != NULL) {
		printf("%s%s\n", "\x1b[", pbuf);
	} else {
		fprintf(stderr, "%s\n", "Must set ansi code in PIPE_COLOR");
		exit(1);
	}
}
