#include "include/table.h"
#include <string.h>


int main() {
   struct HashTable * table = createTable(1);
   char * key;
   char * value;

   for (int i = 0; i < 5; i++) {
      key = (char *)malloc(4*sizeof(char *)+1);
      value = (char *)malloc(4*sizeof(char *)+1);
      int res = sprintf(key, "foo%d", i);
      res = sprintf(value, "bar%d", i);
      if (res);
      insert(table, key, value);
   }
   printTable(table);
   printf("--------------\n");
   for (int i = 2; i < 4; i++) {
      key = (char *)malloc(4*sizeof(char *)+1);
      int res = sprintf(key, "foo%d", i);
      if (res);
      delete(table, key);
      free(key);
   }
   printTable(table);

   freeHashTable(table);
   return 0;
}