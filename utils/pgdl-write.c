/* 
 * Simple example of how you can write data to the
 * socket when running Pcoords with the file 'test1.pcv'.
 *
 * The server should be started like this:
 * pcv -t test1.pcv -s local -Tpngcairo -o out.png
 *
 * Run the client in the same directory that the listening
 * socket.  
 */

#include <stdio.h>

int main(void)
{
	FILE *fp;

	fp = fopen("local", "a");
	fprintf(fp,"t=\"12:00\", i=\"100\", s=\"I write some stuff\";\n");
	close(fp);
}
