#include <stdio.h>      // provides printf()
#include <stdlib.h>     // provides malloc() and free() for dynamic memory,
                        // and rand()/srand() for selecting random alleles
#include <time.h>       // provides time() to seed srand so each run produces different random genetics


#define GENERATIONS 3    // how many generations to build
#define INDENT_LENGTH 4  // spaces per indent level

// person type (node in the family tree)
typedef struct person
{
    struct person *parents[2]; // pointers to parents (NULL at oldest gen)
    char alleles[2];           // two blood type alleles: each is 'A','B','O'
}
person;

// function prototypes
person *create_family(int generations); // build and return the youngest person
void    print_family(person *p, int generation); // print tree with indents
void    free_family(person *p);        // free all allocated nodes
char    random_allele(void);           // return 'A','B', or 'O'

// main: make family, print it, then free memory
int main(void)
{
    srand((unsigned) time(NULL));            // seed random once

    person *you = create_family(GENERATIONS); // build family tree

    print_family(you, 0);                    // show the tree

    free_family(you);                        // clean up all mallocs
    return 0;                                // done
}

// build a family of `generations` (returns pointer to youngest person)
person *create_family(int generations)
{
    // allocate memory for one person
    person *p = malloc(sizeof(person));      // new node
    if (p == NULL)
    {
        return NULL;                         // out of memory (very unlikely here)
    }

    if (generations <= 1)                    // base case: oldest generation
    {
        p->parents[0] = NULL;                // no parents at the top
        p->parents[1] = NULL;

        p->alleles[0] = random_allele();     // random allele 1
        p->alleles[1] = random_allele();     // random allele 2
    }
    else                                     // recursive case: make parents first
    {
        p->parents[0] = create_family(generations - 1); // build left parent
        p->parents[1] = create_family(generations - 1); // build right parent

        // child gets one random allele from each parent
        int i0 = rand() % 2;                 // pick 0 or 1 from parent 0
        int i1 = rand() % 2;                 // pick 0 or 1 from parent 1
        p->alleles[0] = p->parents[0]->alleles[i0]; // inherit from parent 0
        p->alleles[1] = p->parents[1]->alleles[i1]; // inherit from parent 1
    }

    return p;                                // give back the new person
}

// print the family tree (child first, then parents), with indentation
void print_family(person *p, int generation)
{
    if (p == NULL)
    {
        return;                              // nothing to print
    }

    // print indentation spaces (generation * INDENT_LENGTH)
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");                         // add spaces before the line
    }

    // label for this generation
    if (generation == 0)
    {
        printf("Child (blood type %c%c)\n", p->alleles[0], p->alleles[1]); // you
    }
    else if (generation == 1)
    {
        printf("Parent (blood type %c%c)\n", p->alleles[0], p->alleles[1]); // your parent
    }
    else
    {
        printf("Grandparent (blood type %c%c)\n", p->alleles[0], p->alleles[1]); // older gens
    }

    // recursively print parents (next generation, more indent)
    print_family(p->parents[0], generation + 1); // left branch
    print_family(p->parents[1], generation + 1); // right branch
}

// free the whole tree (post-order: free children first)
void free_family(person *p)
{
    if (p == NULL)
    {
        return;                              // nothing to free
    }

    free_family(p->parents[0]);              // free left subtree
    free_family(p->parents[1]);              // free right subtree
    free(p);                                 // free this node
}

// return one random allele: 'A', 'B', or 'O' (roughly equal chance)
char random_allele(void)
{
    int r = rand() % 3;                      // 0, 1, or 2
    if (r == 0) return 'A';                  // case 0 → 'A'
    if (r == 1) return 'B';                  // case 1 → 'B'
    return 'O';                              // case 2 → 'O'
}
