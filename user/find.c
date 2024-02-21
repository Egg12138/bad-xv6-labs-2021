#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"

#define FDMOD_EQ 0
#define FDMOD_CONTAIN 1
#define IS_DIR(s) ((s).type == T_DIR)
#ifndef BUFSZ
#define BUFSZ (512 * sizeof(char))
#endif
#define TOO_LONG(path) (strlen((path)) + DIRSIZ + 1 + 1 > BUFSZ)

void match_contains(char *cur_file, char *target);
void path_join(char *buf,  char *parent,  char *child);
void match_eq(char *cur_filem, char *target);
void match(char *, char *, int);

void match_contains(char *cur_file, char *target) {
   printf("testing...path_join\n");
   char empty[BUFSZ];
   path_join(empty, cur_file, target);
   printf("[%s]\n", empty);
   return ;
}

void path_join(char *buf, char *parent,  char *child) {
   while ((*buf++ = *parent++) != 0);
   *buf++ = '/';
   strcpy(buf, child);
   return;
}

void match_eq(char *cur_file, char *target) {
   struct dirent de;
   struct stat st;
   char pathbuf[BUFSZ], *p;
   int fd = open(cur_file, 0);


   p = pathbuf + strlen(pathbuf);
   *p++ = '/';
   // for valid_file in cur_dir
   while ( read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0) // empty
         continue; 
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if (stat(pathbuf, &st) < 0) {
         fprintf(stderr, "find: cannot stat %s\n", pathbuf);
         continue;
      }
      
      if (IS_DIR(st) && strcmp(p, ".") != 0 && strcmp(p, "..") ) 
         match_eq(de.name, target);
      else if (strcmp(target, p) == 0)
         printf("%s\n", pathbuf);
   } 
   close(fd);
}

void match(char *search_dir,  char *target, int mode) {

   int fd = open(search_dir, 0);
   struct stat st;
   if ((fd = open(search_dir, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", search_dir);
    return;
  }

   if (fstat(fd, &st) < 0) {
    fprintf(stderr, "find: cannot stat %s\n", search_dir);
    close(fd);
    return;
  }

  if (!IS_DIR(st)) {
   fprintf(stderr, "usage: find <DIR> <pattern>\n");
   return;
  }

   if (TOO_LONG(search_dir)) {
    fprintf(stderr, "find: search path too long\n");
    return;
   }

   close(fd);

   switch (mode) {
      case FDMOD_EQ:
         match_eq(search_dir, target);
         break;
      case FDMOD_CONTAIN:
         match_contains(search_dir, target);
         break;
      default:
         printf("MODE err\n");

   }
}


int
main(int argc, char *argv[])
{

   if (argc != 3 && argc != 4) {
      fprintf(stderr, "argument error.\n");
      exit(1);
   }

   char *search_dir = argv[1];
   char *target_pattern = argv[2];
   int mode = atoi(argv[3]);
   
   match(search_dir, target_pattern, mode);

   exit(0);
}