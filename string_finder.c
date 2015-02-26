#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int kmp(const char * hstr, int hsize, const char * nstr, int nsize)
{
   int * prefix = (int *) calloc(nsize, sizeof(int));
   int q = 1, k = -1, i = 0, j = -1;
   int result = 0;

   // Calculate prefix.
   // ToDo: Add description

   while (q < nsize)
   {
      while ((k > -1) && (nstr[k + 1] != nstr[q]))
         k = prefix[k];

      if (nstr[k + 1] == nstr[q])
         k++;

      prefix[q++] = k;
   }

   // Find substring using Knuth–Morris–Pratt string searching algorithm.

   while (i < hsize)
   {
      if ((j > -1) && (nstr[j + 1] != hstr[i]))
         j = prefix[j];

      printf("%c -> %c \n", nstr[j + 1], hstr[i]);

      if (nstr[j + 1] == hstr[i])
         j++;

      if (j == (nsize - 1))
      {
         result = 1;
         break;
      }

      i++;
   }

   printf("%d %d %d %d %d %d\n", hsize, nsize, q, k, j, i);

   free((void *) prefix);

   return result;
}

int main(int argc, char ** argv)
{
   FILE * file;

   if (argc <= 1)
   {
      printf("No file was selected!");
      return -1;
   }

   file = fopen(argv[1], "r");

   if (!file)
   {
      printf("Can't open file %s!", argv[1]);
      return -1;
   }

   while (1)
   {
      char * nstr = NULL;
      size_t nlen = 0;

      printf("\n> ");

      if (getline(&nstr, &nlen, stdin) <= 0) continue;
      if (strncmp(nstr, "exit", 4) == 0) break;

      while (!feof(file))
      {
         char * hstr = NULL;
         size_t hlen = 0;

         if (getline(&hstr, &hlen, file) < 0)
         {
            printf("Can't read line from file %s!", argv[1]);
            return -1;
         }
         else if (!hlen) continue;

         printf("%s : ", hstr);

         if (kmp(hstr, strlen(hstr), nstr, strlen(nstr) - 1))
            printf("YES\n");
         else
            printf("NO\n");
      }
      rewind(file);
   }


   fclose(file);

   return 0;
}