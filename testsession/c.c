#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096
char line[MAXLINE];

#define NUM_PRIMES 1000000

unsigned int curr_prime_ix = 3;
char *primes = NULL;

/* Simple prime number "sieve" */
int is_prime(unsigned int num)
{
   unsigned int limit;
   unsigned int i;
   char *p;

   if (num < curr_prime_ix) {
      return primes[num];
   }

   /* Quickly sort out even numbers */
   if ((num & 1) == 0) {
      return 0;
   }

   /* Check if divisible by primes we have so far.
    *
    * If num is composite, one of the composites muste be less than
    * square root of num.  Only need to loop up to square root.
    *
    * For now, just loop to half (which is always bigger than square
    * root of num)
    */
   limit = (num >> 1) + 1;
   for (i = 3, p = &primes[3]; i < limit && i < curr_prime_ix; i += 2, p += 2) {
      if ((*p) && ((num % i) == 0)) {
         /* Not a prime */
         return 0;
      }
   }

   /* Test all numbers above curr_prime_ix */
   for (; i < limit; i += 2) {
      if ((num % i) == 0) {
         /* Not a prime */
         return 0;
      }
   }

   /* This must be a prime */
   return 1;
}

void cleanup_primes(void)
{
if (primes) {
  free(primes);
  primes = NULL;
 }
}


/* Set each primes[i] to:
 * 1 if i is prime
 * 0 if i is composite
 */
void init_primes(void)
{
   char *p;

   if (primes) {
      cleanup_primes();
   }
   primes = malloc(NUM_PRIMES);
   if (!primes) {
      fprintf(stderr, "Could not init primes\n");
      exit(EXIT_FAILURE);
   }

   /* Start at 3 */
   primes[0] = 0; /* Invalid, just set to 0 */
   primes[1] = 1;
   primes[2] = 1;

   curr_prime_ix = 3;
   p = &primes[curr_prime_ix];

   /* Fill in prime/composite array */
   for (; curr_prime_ix < NUM_PRIMES / 8; curr_prime_ix++) {
      if (is_prime(curr_prime_ix)) {
         *p = 1;
      } else {
         *p = 0;
      }
      p++;
   }
}

void find_n(unsigned int z)
{
   while (z < NUM_PRIMES - 2) {
      if ((!is_prime(z)) && (!is_prime(z + 2))) {
         printf("%d\n", z);
         return;
      }
      z++;
   }
   printf("z not found below %d\n", NUM_PRIMES);
}


int main(int argc, char *argv[])
{
   FILE *fp = NULL;
   unsigned int z;
   char *ret;

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

   init_primes();

   do {
      /* Read line from selected input */
      ret = fgets(line, MAXLINE - 1, fp);
      if (ret) {
         /* Get exactly one number from line */
         z = strtoul(line, NULL, 0);
         find_n(z);
      }
   } while (ret);

   cleanup_primes();

   if (fp) {
      fclose(fp);
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
