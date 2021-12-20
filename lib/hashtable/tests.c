#include "include/table.h"
#include <string.h>


int main() {
   struct HashTable * table = createTable(40000);
   char * key;
   char * value;

   for (int i = 0; i < 20; i++) {
      key = (char *)malloc(4*sizeof(char *)+1);
      value = (char *)malloc(4*sizeof(char *)+1);
      int res = sprintf(key, "foo%d", i);
      res = sprintf(value, "bar%d", i);
      if (res);
      insert(table, key, value);
   }
   printTable(table);
   printf("--------------\n");
   for (int i = 5; i < 15; i++) {
      key = (char *)malloc(4*sizeof(char *)+1);
      int res = sprintf(key, "foo%d", i);
      if (res);
      delete(table, key);
   }
   printTable(table);

   key = (char *)malloc(4*sizeof(char *)+1);
   int res = sprintf(key, "foo0");
   if(res);
   char * val = (char *)get(table, key);
   printf("%s", val);

   freeHashTable(table);
   return 0;
}