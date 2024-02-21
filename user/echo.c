#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
nmain(int argc, char *argv[])
{
  int i;
//just_echo:
  for(i = 1; i < argc; i++){




    write(1, argv[i], strlen(argv[i]));
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}


int 
main(int argc, char *argv[]) {

  char bufarr[512], *bufp;
  bufp = bufarr;
  int bufsz = 0;
  argc--;
  argv++;
  while (argc > 0) {
    char const *s = argv[0];
    unsigned char c;

    // parse argv[0]
    while ((c = *s++)) 
    {
      if ((c == '\\' && *s)) {
        switch (c = *s++) 
        {
          case 'b': c = '\n'; break;
          case 't': c = '\t'; break;
          case 'n': c = '\n'; break;
          case '\\': break;
          default: 
            c = '\\';
            break;
        }
      }
      *bufp++ = c;
      bufsz++;
    }

    // to argv[next]
    argc--;
    argv++;
    *bufp++ = '\n';
    bufsz++;
    if (bufsz == 512)  {
        fprintf(stderr, "cannot handle more than 512 bytes\n");
        write(stdout, bufarr, bufsz);
        break;
      }

  }
  
  
  write(stdout, bufarr, bufsz);
  exit(0);
}