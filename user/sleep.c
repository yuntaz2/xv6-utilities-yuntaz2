// lab-1 activity
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
	// check if there is any argument except program arg itself
	if (argc < 1)
	{
		printf("Not enough num of args for func sleep");
		exit(1);
	}
	else if (argc > 2)
	{
		printf("Too many args for func sleep");
		exit(1);
	}
	else
	{
		char* arg = argv[1];
		int i = atoi(arg);
		sleep(i);
	}
	exit(0);
}
