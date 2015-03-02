#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_CHUNK_SIZE 1024

/**
 * Function tries to find an existence of substring into some string
 * using Knuth–Morris–Pratt string searching algorithm.
 */
int kmp(const char * hstr, int hsize, const char * nstr, int nsize)
{
   int * prefix = (int *) calloc(nsize, sizeof(int));
   int q = 1, k = -1, i = 0, j = -1;
   int result = 0;

   prefix[0] = k;

   while (q < nsize)
   {
      while ((k > -1) && (nstr[k + 1] != nstr[q]))
         k = prefix[k];

      if (nstr[k + 1] == nstr[q])
         k++;

      prefix[q++] = k;
   }

   while (i < hsize)
   {
      while ((j > -1) && (nstr[j + 1] != hstr[i]))
         j = prefix[j];

      if (nstr[j + 1] == hstr[i])
         j++;

      if (j == (nsize - 1))
      {
         result = 1;
         break;
      }

      i++;
   }

   free((void *) prefix);

   return result;
}

/**
 * Function reads data from file and search substring
 */
int file_kmp_search(FILE * file, const char * str, int size)
{
   char data_chunk [DATA_CHUNK_SIZE];

   while (!feof(file))
   {
      if (fread(data_chunk, sizeof(char), DATA_CHUNK_SIZE, file) < 0)
      {
         if (!feof(file))
         {
            printf("Can't read data chunk from file!\n");
         }
         return -1;
      }
      if (kmp(data_chunk, DATA_CHUNK_SIZE, str, size) > 0)
      {
         return 1;
      }
      fseek(file, -size, SEEK_CUR);
   }
   return 0;
}

int main(int argc, char ** argv)
{
   FILE * file;

   if (argc <= 1)
   {
      printf("No file was selected!\n");
      return -1;
   }

   file = fopen(argv[1], "r");

   if (!file)
   {
      printf("Can't open file %s!\n", argv[1]);
      return -1;
   }

   while (1)
   {
      int result = 0;
      char * str = NULL;
      size_t len = 0;

      if (getline(&str, &len, stdin) <= 0) continue;
      if (strncmp(str, "exit", strlen(str) - 1) == 0) break;

      if ((result = file_kmp_search(file, str, strlen(str) - 1)) > 0)
         printf("YES\n");
      else if (result == 0)
         printf("NO\n");
      else
         break;

      rewind(file);
   }

   fclose(file);

   return 0;
}