#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_line(FILE *fp)
{
   int c;
   int ret = 0;
   int linelen = 0;
   /* Left/right balance
    * if 0 at end of line then
    * number of left/right shoes matches.
    */
   int lr = 0;

   do {
      c = fgetc(fp);
      switch (c) {
      case 'L':
      case 'l':
         lr++;
         linelen++;
         break;
      case 'R':
      case 'r':
         lr--;
         linelen++;
         break;

      case '\n':
      case EOF:
         if (c == '\n') {
            ret = 1;
         }
         if (linelen > 0) {
            printf("%s\n", lr == 0 ? "ok" : "missing");
         }
         return ret;
         break;
      }
   } while (!ret);

   /* Will never get here */
   return ret;
}

int main(int argc, char *argv[])
{
   FILE *fp;

   /* Assign fp to either stdin or argv[1] */
   if (argc == 1) {
      /* No arguments = stdin */
      fp = stdin;
   } else {
      /* - also means stdin */
      if(!strcmp(argv[1], "-")) {
         fp = stdin;
      } else {
         /* Open argv[1] */
         fp = fopen(argv[1], "r");
         if (!fp) {
            perror(argv[1]);
            exit(EXIT_FAILURE);
         }
      }
   }

   /* Run run_line until EOF */
   while (run_line(fp));

   return EXIT_SUCCESS;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
