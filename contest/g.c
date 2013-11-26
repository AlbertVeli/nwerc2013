#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
char line[MAXLINE];

/* Number of pre-calculated primes */
#define NUM_FIRST_PRIMES 4096
unsigned int curr_prime_ix = 0;
unsigned int first_primes[NUM_FIRST_PRIMES];

/* Simple prime number "sieve" */
int is_prime(unsigned int num)
{
   unsigned int limit;
   unsigned int i;
   unsigned int *p;

   if (num < 4) {
      return 1;
   }

   /* Quickly sort out even numbers */
   if ((num & 1) == 0) {
      return 0;
   }

   /* Only need to test up to square root of num */
   limit = num / 2 + 1;

   /* Try primes in first_primes[] first */
   for (i = 0, p = first_primes; ((*p) < limit) && (i < curr_prime_ix); i++, p++) {
      if ((num % (*p)) == 0) {
         /* Not a prime */
         return 0;
      }
   }

   i = first_primes[curr_prime_ix - 1];
   if (i > limit) {
      return 1;
   }

   /* Try numbers above first_primes */
   for (; i < limit; i += 2) {
      if ((num % i) == 0) {
         /* Not a prime */
         return 0;
      }
   }

   /* This must be a prime */
   return 1;
}

void init_primes(void)
{
   int i;

   first_primes[0] = 2;
   first_primes[1] = 3;
   first_primes[2] = 5;
   curr_prime_ix = 3;

   i = first_primes[curr_prime_ix - 1] + 2;

   /* Fill in first_primes */
   for (; curr_prime_ix < NUM_FIRST_PRIMES; i += 2) {
      if (is_prime(i)) {
         first_primes[curr_prime_ix++] = i;
      }
   }
}

/* Maximum number of prime factors in number */
#define MAX_P_FAC 512
typedef struct pfac {
   unsigned int factors[MAX_P_FAC];
   unsigned char factor_active[MAX_P_FAC];
   int num_factors;
} pfac_t;

void insert_factor(unsigned int prime, pfac_t *f)
{
   if (f->num_factors < MAX_P_FAC) {
      f->factors[f->num_factors] = prime;
      f->factor_active[f->num_factors] = 1;
      f->num_factors++;
   } else {
      fprintf(stderr, "prime factors array full :(\n");
   }
}

void prime_factorize(unsigned int num, pfac_t *f)
{
   unsigned int limit;
   unsigned int i;

   memset(f, 0, sizeof(pfac_t));

   /* Get next prime factor */
   do {
      limit = num / 2 + 2;
      for (i = 2; i < limit; i++) {
         if (is_prime(i)) {
            if ((num % i) == 0) {
               insert_factor(i, f);
               num /= i;
               break;
            }
         }
      }
   } while (i < limit);

   /* Insert remaining num, which must be the last prime */
   insert_factor(num, f);
}

void reduce_frac(pfac_t *n, pfac_t *d)
{
   int i, j;

   for (i = 0; i < n->num_factors; i++) {
      for (j = 0; j < d->num_factors; j++) {
         if (n->factor_active[i] && d->factor_active[j]) {
            if (n->factors[i] == d->factors[j]) {
               n->factor_active[i] = 0;
               d->factor_active[j] = 0;
               break;
            }
         }
      }
   }
}

unsigned int get_pfac_num(pfac_t *f)
{
   int i;
   unsigned int res = 1;
   for (i = 0; i < f->num_factors; i++) {
      if (f->factor_active[i]) {
         res *= f->factors[i];
      }
   }

   return res;
}

void print_reduced_fraction(unsigned int num, unsigned int den)
{
   pfac_t f_num, f_den;

   prime_factorize(num, &f_num);
   prime_factorize(den, &f_den);

   /* reduce numerator/denominator */
   reduce_frac(&f_num, &f_den);

   /* print reduced fraction */
   printf("%u/%u\n", get_pfac_num(&f_num), get_pfac_num(&f_den));
}

int main(int argc, char *argv[])
{
   FILE *fp;
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
         unsigned int ab, ac, bd;
         char *endp;
         char *startp = line;

         ab = strtoul(startp, &endp, 0);
         if (ab == 0 || startp == endp) {
            exit(EXIT_FAILURE);
         }
         startp = endp;
         ac = strtoul(startp, &endp, 0);
         if (ac == 0 || startp == endp) {
            exit(EXIT_FAILURE);
         }
         startp = endp;
         bd = strtoul(startp, &endp, 0);
         if (bd == 0) {
            exit(EXIT_FAILURE);
         }

         /* Do calculation and reduce fraction */
         if (bd > ac) {
            print_reduced_fraction(ab * ac, bd - ac);
         } else {
            fprintf(stderr, "Error: bd must be larger than ac\n");
         }
      }

   } while (ret);

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
