#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
char line[MAXLINE];

#define MAX_VESSELS 450
/* booty[id] contains amount of booty for that vessel */
unsigned int booty[MAX_VESSELS];
unsigned int num_vessels;
unsigned char *matrix = NULL;

typedef struct chain {
   unsigned int vessels[MAX_VESSELS];
   unsigned int chain_size;
   unsigned int booty;
} chain_t;

chain_t maxchain, tmpchain;

void add_vessel(chain_t *c, int v)
{
   unsigned int x;
   /* Check if v has connection to all vessels in chain */
   for (x = 0; x < c->chain_size; x++) {
      if (!matrix[v * num_vessels + c->vessels[x]]) {
         /* No connection to vessel x, don't add v */
         return;
      }
   }
   /* Didn't return in loop, add v */
   c->vessels[c->chain_size] = v;
   c->chain_size++;
   c->booty += booty[v];
}

void print_silver_coins(void)
{
   unsigned int x, y;

   memset(&maxchain, 0, sizeof(chain_t));

   for (y = 0; y < num_vessels; y++) {

      /* Calculate chain for vessel y */
      memset(&tmpchain, 0, sizeof(chain_t));

      /* First add y */
      add_vessel(&tmpchain, y);
      for (x = 0; x < num_vessels; x++) {
         if (matrix[y * num_vessels + x]) {
            /* Add vessel x (if connection to all others in chain) */
            add_vessel(&tmpchain, x);
         }
      }
      if (tmpchain.booty > maxchain.booty) {
         memcpy(&maxchain, &tmpchain, sizeof(chain_t));
      }
   }
   printf("%d\n", maxchain.booty);
}


int main(int argc, char *argv[])
{
   FILE *fp;
   char *ret;
   int state;
   unsigned int chains = 0;
   unsigned int vessel = 0;
   unsigned int c_start = 0, c_end = 0;

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

   state = 0;
   do {
      /* Read line from selected input */
      ret = fgets(line, MAXLINE - 1, fp);
      if (ret) {
         int val;
         char *endp;
         char *startp = line;

         do {
            val = strtoul(startp, &endp, 0);
            if (endp == startp) {
               val = -1;
            } else {

               startp = endp;

               switch (state) {

               case 0:
                  /* number of vessels */
                  num_vessels = val;
                  vessel = 0; /* Local vessel counter */
                  state = 1;
                  /* Alloc memory for connection graph matrix */
                  matrix = malloc(num_vessels * num_vessels);
                  memset(matrix, 0, num_vessels * num_vessels);
                  if (!matrix) {
                     fprintf(stderr, "Unable to alloc memory for graph matrix :(\n");
                     exit(EXIT_FAILURE);
                  }
                  break;

               case 1:
                  /* number of chains */
                  chains = val;
                  state = 2;
                  break;

               case 2:
                  /* store booty in booty array */
                  booty[vessel] = val;
                  vessel++;
                  if (vessel == num_vessels) {
                     state = 3;
                  }
                  break;

               case 3:
                  /* chain start */
                  c_start = val - 1;
                  state = 4;
                  break;

               case 4:
                  /* chain end */
                  c_end = val - 1;
                  /* store c_start - c_end in matrix */
                  matrix[c_start + c_end * num_vessels] = 1;
                  matrix[c_end + c_start * num_vessels] = 1;
                  chains--;
                  if (chains == 0) {
                     state = 0;

                     /* Test case complete, run */
                     print_silver_coins();
                  } else {
                     state = 3;
                  }
                  break;
               }
            }
         } while (val >= 0);
      }

   } while (ret);

   if (fp) {
      fclose(fp);
   }

   if (matrix) {
      free(matrix);
      matrix = NULL;
   }

   return EXIT_SUCCESS;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
