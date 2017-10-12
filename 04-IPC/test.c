#include <stdio.h>

int main() {
  for (int i = 0; i < 60; ++i) {
    printf("\x1B[%dm", i);
    printf("We're using color %d\n", i);
  }
  return 0;
}
