#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define read_from read
#define write_to write
int is_prime(int num) {
   for (int i = 2; i <= num / 2; i++) 
      if (num % i == 0) 
         return 0;
   return 1; 
}

void get_number_from_pipe(int p[2], int *buf) {
   if (read(p[PIPE_READ], buf, sizeof(int)) == 0) {
      close(p[PIPE_READ]);
      exit(0);
   }
   exit(0);
}

void prime_proc_handler(int l2c[2]) {
   int prime, number;
   int c2r[2];
   int pid;
   get_number_from_pipe(l2c, &prime);
   printf("prime %d\n", prime);
   pipe(c2r);
   if ((pid = fork()) < 0) {
         fprintf(stderr, "fork err\n");
         close(c2r[PIPE_READ]);
         close(c2r[PIPE_WRITE]);
         close(l2c[PIPE_READ]);
         exit(1);
   } else if (pid > 0) { //child
         close(c2r[READ]);
         while(read_from(left[PIPE_READ], &number, sizeof(int))) {
            if (number % prime == 0) continue;
            write_to(l2c[PIPE_WRITE], &number, sizeof(int));
         }
         close(l2c[PIPE_WRITE]);
         exit(0);
   }  else {
      prime_proc_handler(l2c);
      exit(0);
   }
}
// write (Target, &content, Length)
// read (Src, &buffer, Length )
int
main(void)
{
   int pid;
   int l2c[2], c2r[2], p[2]; // >>>>>>
   int buf; //buf

   pipe(p);
   pid = fork();
   if (pid < 0) {
      fprintf(stderr, "fork err\n");
      exit(1);
   }

   if (pid > 0) {// children process.  
      close(p[READ]);
      for (int dig = 2; dig < 36; dig++) 
         write_to(p[WRITE], &dig, sizeof(int));
      close(p[WRITE]);
      exit(0);
   } else {
      prime_proc_handler(p);
      exit(0);

   }

   exit(0);
}