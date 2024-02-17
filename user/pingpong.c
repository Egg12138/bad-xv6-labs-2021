#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// const char* ANSWER = " received poing";
#define PARENT 0
#define CHILD 1

#define READ 1
#define WRITE 0



// make qemu CPUS=1
int
main(void)
{
   int p2c[2], c2p[2];
   char c;

   pipe(p2c);
   pipe(c2p);
   // fork after pipe 
   int pid = fork();

   if (pid < 0) {
      fprintf(2, "fork error\n");
      exit(1);
   } 
   
   else if (pid > 0) {// children
      
      int ppid = getpid();
      close(p2c[READ]);
      close(c2p[WRITE]);

      write(p2c[WRITE], &c, 1);
      close(p2c[WRITE]);

      read(c2p[READ], &c, 1);
      close(c2p[READ]);
      
      printf("%d: received pong\n", ppid);
      exit(0);
      
   } else {
      // parent
      int cpid = getpid();
      close(p2c[WRITE]);
      close(c2p[READ]);

      read(p2c[READ], &c, 1);
      close(p2c[READ]);
      printf("%d: received pong\n", cpid);

      write(c2p[WRITE], &c, 1);
      close(c2p[WRITE]);
      exit(0);
      
   }  
  
   exit(0);
}