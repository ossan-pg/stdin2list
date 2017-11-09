#include <stdio.h>
#include <stdlib.h>

#include "reader.h"

int main(void)
{
	char buff[20 + 1];
	int n = 1;
	for(;;) {
		printf("n = %d\n", n);
		int len = read_stdin(buff, sizeof(buff), &n);
		if(len <= 0) {
			break;
		}
	}

	return EXIT_SUCCESS;
}
