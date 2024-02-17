#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/stat.h"
#include "user/user.h"




int
main(int argc, char *argv[])
{
	if (argc <= 1) {
      fprintf(2, "sleep: sleep requires time argument!\n");
		exit(0);
	}
	
	if (argc > 2) 
		printf("only the firstone argument is needed.\n");
	int tick = atoi(argv[1]);
	sleep(tick * 10);
	exit(0);
	// return 0;
}