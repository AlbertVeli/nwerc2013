#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096*16
char line[MAXLINE];

typedef struct vote {
   int id; /* id of candidate */
   int votes;
} vote_t;

#define MAX_CANDIDATES 4096
vote_t votes[MAX_CANDIDATES];

void init_votes(void)
{
   vote_t *vp = &votes[0];
   int i;

   for (i = 0; i < MAX_CANDIDATES; i++) {
      vp->id = -1;
      vp->votes = 0;
      vp++;
   }
}

void count_votes(void)
{
   int max = -1;
   int index = -1;
   vote_t *vp = &votes[0];
   int i;
   int winner = 0;
   for (i = 0; i < MAX_CANDIDATES; i++) {
      if (vp->id == -1) {
         break;
      }
      if (vp->votes > max) {
         max = vp->votes;
         index = i;
         winner = 1;
      } else if (vp->votes == max) {
         /* More than 1 id has the same num of votes */
         winner = 0;
      }
      vp++;
   }
   if (winner) {
      printf("%d\n", votes[index].id);
   } else {
      printf("reelection\n");
   }
}

void add_vote(int id)
{
   int i;
   vote_t *vp = &votes[0];

   for (i = 0; i < MAX_CANDIDATES; i++) {
      if (vp->id == -1) {
         /* Add new candidate to array */
         vp->id = id;
         vp->votes = 1;
         break;
      }
      if (vp->id == id) {
         /* Increase vote num */
         vp->id = id;
         vp->votes++;
         break;
      }
      vp++;
   }
   if (i == MAX_CANDIDATES) {
      fprintf(stderr, "Array full, voting aborted\n");
      exit(EXIT_FAILURE);
   }
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

   do {
      int num_votes;
      int tot_votes;

      init_votes();

      /* Read line from selected input */
      num_votes = 0;
      ret = fgets(line, MAXLINE - 1, fp);
      if (ret) {
         int id;
         char *endp;
         char *startp = line;

         /* First number is total number of votes */
         tot_votes = strtoul(startp, &endp, 0);
         startp = endp;
         if (tot_votes > 0) {
            /* Loop through numbers in line */
            do {
               id = strtoul(startp, &endp, 0);
               if (endp == startp) {
                  id = -1;
               } else {
                  num_votes++;
                  add_vote(id);
                  startp = endp;
               }
            } while ((id >= 0) && (num_votes < tot_votes));

            /* num_votes should be equal to tot_votes here */
            if (num_votes > 0) {
               count_votes();
            }
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
