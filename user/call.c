#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  unsigned int i = 0x00646c72;
  // 57616 = 0xe110, 
  // printf("H%x, Wo%s", 57616, &i);
  printf("H%x, Wo%s(%x)\n", 0xe110, &i, &i);
  printf("%d %d\n", f(8)+1, 13);
  printf("x=%d,y=%d\n", 3);
  exit(0);
}
