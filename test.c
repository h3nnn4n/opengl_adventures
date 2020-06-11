#include <stdio.h>

#include "src/stb_ds.h"

typedef struct { int x,y; } pair;

int main(int argc, char *argv[])
{
   int i = 2;
   pair p;
   pair p2 = { 123, 4456 };
   struct { pair key; pair *value; } *hash = NULL;

   p = (pair){2,3}; hmput(hash, p, &p2);
   p = (pair){7,4}; hmput(hash, p, &p2);
   p = (pair){1,1}; hmput(hash, p, &p2);
   p = (pair){5,5}; hmput(hash, p, &p2);
   p = (pair){3,5}; hmput(hash, p, &p2);
   p = (pair){5,5}; hmput(hash, p, &p2);

   for (int i=0; i < hmlen(hash); ++i)
     printf("%p ", hash[i].value);

  return 0;
}
