
/* ======================================================================
   QUADRATIC KNAPSACK PROBLEM  Alberto Caprara, David Pisinger, Polo Toth
   ====================================================================== */

/* This code solves the quadratic knapsack problem, which
 * asks to maximize a quadratic objective function subject
 * to a single weight constraint. 
 *
 *   maximize   \sum_{i=1}^{n} \sum_{j=1}^{n} p_{i,j} x_{i} x_{j}
 *   subject to \sum_{j=1}^{n} w_{j} x_{j} \leq c
 *              x_{j} \in \{0,1\}, j = 1,\ldots,n
 *
 * It is assumed that all coefficients are nonnegative integers.
 * A description of the code is found in the following paper:
 *
 *   A. Caprara, D. Pisinger, P. Toth, 
 *   "Exact solution of the Quadratic Knapsack Problem", 
 *   INFORMS Journal on Computing, 11, 125-137 (1999). 
 *
 * The present code is written in ANSI-C, and has been compiled with
 * the GNU-C compiler using option "-ansi -pedantic" as well as the
 * HP-UX C compiler using option "-Aa" (ansi standard).
 *
 * This file contains the callable routine quadknap with prototype
 *
 *   int quadknap(int no, int cap, int *ptab, int *wtab, int *xtab);
 * 
 * the meaning of the parameters is the following:
 *   no        Size of problem, i.e. number of items.
 *   ptab      A pointer to an integer matrix of profits p[i][j].
               It is very important to declare the matrix as
                 int ptab[MSIZE][MSIZE];
               in order to be interpreted correctly. The constant
               MSIZE is defined below.
 *   wtab      A table of weights w[i] of length at least no.
 *   xtab      A table of solutions variables x[i] of length at least no.
 * the procedure returns the optimal objective value.
 *
 * (c) Copyright, August 2000,
 *   David Pisinger                     Alberto Caprara, Paolo Toth
 *   DIKU, University of Copenhagen     DEIS, University of Bolgona 
 *   Universitetsparken 1               Viale Risorgimento 2
 *   Copenhagen, Denmark                Bologna, Italy
 *
 * This code can be used free of charge for research and academic purposes
 * only.
 */         

#define MSIZE      400   /* maximum number of 0-1 variables */
#define EPSILON  1E-6    /* small tolerance used in subgradient optimization */
#define INFTY    INT_MAX  /* very large int  used in subgradient optimization */

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include <stdarg.h>
#include <limits.h>

/* ======================================================================
				   macros
   ====================================================================== */

#define ABS(a)                  ((a) < 0 ? -(a) : (a))
#define DET(a1, a2, b1, b2)     ((a1) * (ptype) (b2) - (a2) * (ptype) (b1))
#define SWAP(a, b)  { register lpitem t; t = *(a); *(a) = *(b); *(b) = t; }


/* ======================================================================
				 type declarations
   ====================================================================== */

typedef int     boolean;
typedef int     ntype;   /* number of items          */
typedef int     itype;   /* item profits and weights */
typedef int     stype;   /* sum of profit or weight  */
typedef double  ptype;   /* product type             */
typedef float   etype;   /* efficiency type          */

typedef struct {               /* item for solving lp-relaxation in row */
  etype e; /* profit to weight ratio */
  etype p; /* profit */
  itype w; /* weight */
  etype *x; /* pointer to solution variable */
} lpitem;

typedef struct ilist {
  struct ilist *prev;
  struct ilist *next;
  struct ilist *b;/* break item, before this item was changed */
  itype  psumb;   /* psumb before this item was changed */
  itype  wsumb;   /* wsumb before this item was changed */
  itype  p;       /* the items profit */
  itype  w;       /* the items weight */
  ntype  no;      /* the items position in a p/w ordering */
} itemlist;

typedef struct rk {
  struct ilist *b;/* current break item */
  itype  psumb;   /* current profit sum up to break item */
  itype  wsumb;   /* current weight sum up to break item */
  itemlist it[MSIZE];
  itemlist head;
  itemlist tail;
} rowknap;
 
typedef int (*funcptr) (const void *, const void *);


/* ======================================================================
 			    global variables
   ====================================================================== */

static ntype n;
static itype p[MSIZE][MSIZE];
static itype w[MSIZE];
static itype x[MSIZE];
static itype pover[MSIZE];
static itype minw[MSIZE];
static boolean xstar[MSIZE];
static rowknap rowk[MSIZE];
static stype c, z, z0, ptot, wtot, heur, impr, zbrute, lagbound, inibound;
static stype fixp, fixw;
static long maxlagr;
static long iterations;


/* =======================================================================
				  error
   ======================================================================= */

void error(char *str)
{
  printf("%s\n", str);
  printf("PROGRAM IS TERMINATED !!!\n\n");
  exit(-1);
}


/* ======================================================================
				 comparisons
   ====================================================================== */

int povercomp(ntype *a, ntype *b) {return DET(pover[*b],w[*b],pover[*a],w[*a]);}
int icomp(ntype *a, ntype *b) { return *a - *b; }
int ecomp(lpitem *a, lpitem *b) { 
  if (a->e == b->e) return 0; 
  return (a->e < b->e ? 1 : -1); 
}


/* ======================================================================
				  palloc
   ====================================================================== */

void pfree(void *p)
{
  if (p == NULL) error("freeing null");
  free(p);
}


void *palloc(long size, long no)
{
  long *p;

  size *= no;
  if (size == 0) size = 1;
  if (size != (size_t) size) error("Alloc too big");
  p = malloc(size);
  if (p == NULL) error("No memory");
  return p;
}


/* ======================================================================
				partsort
   ====================================================================== */

void partsort(lpitem *f, lpitem *l, stype c)
{
  register lpitem *i, *j, *m;
  register etype me;
  register stype ws;
  register int d;

  for (;;) {
    d = l - f + 1;
    if (d > 1) {
      m = f + d / 2;
      if (f->e < m->e) SWAP(f, m);
      if (d > 2) {
        if (m->e < l->e) {
          SWAP(m, l); if (f->e < m->e) SWAP(f, m);
        }
      }
    }
    if (d <= 3) break;
    me = m->e; i = f; j = l; ws = 0;
    for (;;) {
      do { ws+=i->w; i++; } while (i->e > me);
      do {           j--; } while (j->e < me);
      if (i > j) break;
      SWAP(i, j);
    }
    if (ws <= c) { f = i; c -= ws; } else l = i-1;
  }
}


/* ======================================================================
                                  lpsolve
   ====================================================================== */

stype lpsolve(lpitem *a, stype c, int n)
{
  register lpitem *k, *m;
  register stype r;
  register etype ps;

  if (c <= 0) return 0;
  m = a+n;
  partsort(a, m-1, c);
  for (k = a, ps = 0, r = c; ; k++) {
    if (k == m) return ps;
    if (k->w > r) break;
    r -= k->w; ps += k->p;
    if (k->x != NULL) *(k->x) = 1.0;
  }
  if (k->x != NULL) *(k->x) = r / (etype) k->w;
  return ps + r * k->e;
}


/* ======================================================================
				findpover
   ====================================================================== */

void findpover(void)
{
  register int i, j;
  register lpitem *k;
  lpitem a[MSIZE];

  for (i = 0; i < n; i++) {
    /* copy profits with reference to the original item */
    for (j = 0, k = a; j < n; j++) {
      if (j == i) continue;
      k->p = p[j][i]; k->w = w[j];
      k->e = k->p / (etype) k->w; k->x = NULL;
      k++;
    }
    pover[i] = p[i][i] + lpsolve(a, c-w[i], n-1);
  }
}


/* ======================================================================
                                findorder
   ====================================================================== */

void findorder(ntype *t)
{
  int i;

  /* find upper planes */
  findpover();

  /* sort according to pover2 */
  for (i = 0; i < n; i++) t[i] = i;
  qsort(t, n, sizeof(ntype), (funcptr) povercomp);
}


/* ======================================================================
				  findminw
   ====================================================================== */

void findminw(void)
{
  int i;
  itype mw;

  mw = w[n-1];
  for (i = n-1; i >= 0; i--) {
    if (w[i] < mw) mw = w[i];
    minw[i] = mw;
  }
}


/* ======================================================================
                                  improve
   ====================================================================== */

void improve(int *xprime)
{
  register int i, j, gaini, gainj;
  register stype tot, gain, bgain, res;
  itype q[MSIZE];

  res = c;
  /* printf("improve: n %d res %d c %d\n", n, res, c); */
  for (i = 0; i < n; i++) if (xprime[i]) res -= w[i];
  for (;;) {
    for (i = 0; i < n; i++) {
      for (tot = p[i][i], j = 0; j < n; j++) {
        if ((j != i) && (xprime[j] != 0)) tot += p[i][j] + p[j][i];
      }
      q[i] = tot;
    }
    bgain = gaini = gainj = 0;
    for (i = 0; i < n; i++) {
      if (xprime[i] == 0) {
        if (w[i] <= res) {
          gain = q[i];
          if (gain > bgain) { bgain = gain; gaini = i; gainj = -1; }
        } else {
          for (j = 0; j < n; j++) {
            if (j == i) continue;
            if (xprime[j] == 0) continue;
            if (w[i] - w[j] <= res) {
              gain = q[i] - q[j] - (p[i][j] + p[j][i]);
              if (gain > bgain) { bgain = gain; gaini = i; gainj = j; }
            }
          }
        }
      }
    }
    /* printf("best gain %d i %d j %d res %d\n", bgain, gaini, gainj, res); */
    if (bgain == 0) break;
    xprime[gaini] = 1;
    if (gainj != -1) xprime[gainj] = 0; 
    if (gainj != -1) res += w[gainj] - w[gaini]; else res -= w[gaini];
    if (res < 0) error("Negative res");
  } /* end of while loop */
  for (gain = 0, res = c, i = 0; i < n; i++) {
    if (xprime[i] == 0) continue;
    res -= w[i];
    for (j = 0; j < n; j++) {
      if (xprime[j]) gain += p[i][j];
    }
  }
  if (res < 0) error("Excess capacity");
  /* printf("possible solution %d\n", gain); */
  if (gain > z) {
    /* printf("improved solution %d to %d\n", z+fixp, gain+fixp); */
    z = gain;
    for (i = 0; i < n; i++) xstar[i] = xprime[i];
  }
}


/* ======================================================================
 			  	greedy
   ====================================================================== */

void greedy(void)
{
  int i, j, mini;
  stype psum, wsum, pi, minp;
  double eff, mineff;

  /* find global sums, and initialize solution vector */
  psum = 0; wsum = 0;
  for (i = 0; i < n; i++) {
    x[i] = 0; wsum += w[i];
    for (j = 0; j < n; j++) psum += p[i][j];
  }
  ptot = psum;
  wtot = wsum;

  /* run greedy heuristic */
  for (i = 0; i < n; i++) x[i] = 1;
  psum = ptot; wsum = wtot;
  for (;;) {
    mineff = ptot; mini = -1;
    for (i = 0; i < n; i++) {
      if (!x[i]) continue;
      pi = -p[i][i];
      for (j = 0; j < n; j++) if (x[j]) pi += p[j][i] + p[i][j];
      eff = pi / (double) w[i];
      if (eff < mineff) { mineff = eff; mini = i; minp = pi; }
    }
    if (mini == -1) error("No item found\n");
    i = mini; x[i] = 0;
    psum -= minp; wsum -= w[i];
    if (wsum <= c) break;
  }
  z = heur = psum;
  memcpy(xstar, x, MSIZE*sizeof(boolean));
  /* printf("greedy psum %d wsum %d c %d\n", psum, wsum, c); */
  improve(x);
}



/* ======================================================================
                                  reorder
   ====================================================================== */

void reorder(ntype *new, ntype *old)
{
  /* makes an ordering according to indices in new, returns a table */
  /* old, which when applied to reorder, brings back the table to origin */
  int i, j, i1, j1;
  itype p1[MSIZE][MSIZE];
  itype w1[MSIZE];
  itype x1[MSIZE];
  ntype o1[MSIZE];
  itype pover1[MSIZE];
  itype xstar1[MSIZE];

  /* copy to new table */
  for (i = 0; i < n; i++) {
    i1 = new[i];
    o1[i] = old[i1]; 
    /* o1[i1] = old[i]; */
    x1[i] = x[i1];
    w1[i] = w[i1];
    pover1[i] = pover[i1];
    xstar1[i] = xstar[i1];
    for (j = 0; j < n; j++) {
      j1 = new[j];
      p1[i][j] = p[i1][j1];
    }
  }

  /* copy new table back */
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) p[i][j] = p1[i][j];
    xstar[i] = xstar1[i]; pover[i] = pover1[i]; 
    x[i] = x1[i]; w[i] = w1[i];
    old[i] = o1[i];
  }
}


/* ======================================================================
                                invertorder
   ====================================================================== */

static void invertorder(int *rev, int *org)
{
  int i;

  for (i = 0; i < n; i++) {
    rev[org[i]] = i;
  }
}


/* ======================================================================
                                  iterate
   ====================================================================== */

void iterate(int loopno)
{
  register int i, j, pij;
  register lpitem *k;
  register etype d, da, stp, dp, sumd, mu, *pi, *xi;
  register itype zlpmin, h, count;
  itype zlp;
  lpitem a[MSIZE];
  etype pp[MSIZE][MSIZE];
  etype pbest[MSIZE][MSIZE];
  etype xover[MSIZE];
  etype x[MSIZE][MSIZE];
  etype delta[MSIZE][MSIZE];
  itype xprime[MSIZE], pover[MSIZE];

  /* copy to floats */
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) pp[i][j] = p[i][j];
  }

  /* iterate multipliers */
  zlpmin = INFTY; count = 0; mu = 1.0;
  for (h = 0; h < maxlagr; h++) {

    /* solve continuous knapsack for each row */
    for (i = 0; i < n; i++) {
      pi = pp[i]; xi = x[i]; xi[i] = 1.0; /* diagonal always chosen */
      for (j = 0, k = a; j < n; j++) {
        if (j == i) continue;
        k->p = pi[j]; k->w = w[j]; k->e = k->p / k->w;
        k->x = &xi[j]; xi[j] = 0.0;
        k++;
      }
      pover[i] = p[i][i] + lpsolve(a, c-w[i], n-1);
    }

    /* solve column knapsack problem */
    for (i = 0, k = a; i < n; i++, k++) {
      k->p = pover[i]; k->w = w[i]; k->e = k->p / k->w;
      k->x = &xover[i]; xover[i] = 0.0;
    }
    zlp = lpsolve(a, c, n);
    /* if (h % 50 == 0) printf("%d: bound %d\n", h, zlp+fixp);  */
    if ((h == 0) & (loopno == 1)) inibound = zlp+fixp; /* initial ub */

    /* find gradient */
    sumd = 0;
    for (i = 0; i < n; i++) {
      for (j = 0; j < i; j++) {
        d = x[i][j] * xover[i] - x[j][i] * xover[j];
        delta[i][j] = d; sumd += d * d;
      }
    }

    /* iterate pp[i][j] */
    for (i = 0; i < n; i++) {
      for (j = 0; j < i; j++) {
        d = delta[i][j];
        if (ABS(d) < EPSILON) continue;
        stp = mu * (zlp - z) / sumd;
        dp  = d * stp;
        if (dp > 0) {
          if (dp > pp[i][j]) dp = pp[i][j];
        } else {
          if (-dp > pp[j][i]) dp = -pp[j][i];
        }
        pp[i][j] -= dp; pp[j][i] += dp;
      }
    }
    count++;
    if (zlp < zlpmin) {
      zlpmin = zlp; count = 0;
      for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) pbest[i][j] = pp[i][j];
      }
    }
    if (count == 20) { mu /= 2; count = 0; }
    if (h % 2 == 0) {
      /* printf("heuristic for h %d\n", h); */
      for (i = 0; i < n; i++) xprime[i] = (int) xover[i]; 
      improve(xprime);
    }
  } /* end of one iteration */

  /* copy to integer table */
  for (i = 0; i < n; i++) {
    for (j = 0; j < i; j++) {
      pij = p[i][j] + p[j][i];
      p[i][j] = pbest[i][j] + 0.5;
      p[j][i] = pij - p[i][j];
    }
  }
  lagbound = zlpmin+fixp; z0 = z+fixp;
  /* printf("best bound %d greedy z %d impr z %d\n", lagbound,heur,z+fixp); */
}


/* ======================================================================
                                  findbound
   ====================================================================== */

stype findbound(int f, int c, int h, int val)
{
  /* find continuous bound for items i >= t,   */
  /* capacity c, and item h fixed at value val */

  register int i, j;
  register lpitem *k;
  register itype *pi;
  register stype c1, p1, n1;
  lpitem a[MSIZE];
  itype pover[MSIZE];

  /* solve continuous knapsack for each row */
  for (i = f; i < n; i++) {
    pi = p[i]; c1 = c; p1 = 0; n1 = 0;
    for (j = f, k = a; j < n; j++) {
      if (j == h) { /* fixed variable */
        if (val == 1) { c1 -= w[j]; p1 += pi[j]; } continue;
      }
      if (j == i) { /* diagonal value always chosen */
        c1 -= w[j]; p1 += pi[j]; continue;
      }
      k->p = pi[j]; k->w = w[j]; k->e = k->p / (etype) k->w; k->x = NULL;
      k++; n1++;
    }
    pover[i] = p1 + lpsolve(a, c1, n1);
  }

  /* solve column knapsack problem */
  c1 = c; p1 = 0; n1 = 0;
  for (i = f, k = a; i < n; i++) {
    if (i == h) {
      if (val == 1) { c1 -= w[i]; p1 += pover[i]; } continue;
    }
    k->p = pover[i]; k->w = w[i]; k->e = k->p / (etype) k->w; k->x = NULL;
    k++; n1++;
  }
  return p1 + lpsolve(a, c1, n1);
}


/* ======================================================================
                                   reduce
   ====================================================================== */

ntype reduce(ntype *ord)
{
  register int i, j;
  stype u, r1, r0;

  r1 = r0 = 0;
  for (i = 0; i < n; i++) {
    ord[i] = i;
    if (xstar[i] == 1) {
      u = findbound(0, c, i, 0);
      if (u < z+1) {
        ord[i] = i+n; xstar[i] = 1; r1++;
        /* printf("item %d fixed at 1\n", i); */
      }
    } else {
      u = findbound(0, c, i, 1);
      if (u < z+1) {
        ord[i] = i+n; xstar[i] = 0; r0++;
        /* printf("item %d fixed at 0\n", i); */
      }
    }
  }
  qsort(ord, n, sizeof(ntype), (funcptr) icomp);
  for (i = 0; i < n; i++) if (ord[i] >= n) ord[i] -= n;
  /* printf("fixed %d at 1, %d at 0 out of %d\n", r1, r0, n); */
  return r0 + r1;
}


/* ======================================================================
                                   decrease
   ====================================================================== */

void decrease(ntype nf, stype *ps, stype *ws)
{
  register int i, j;
  stype psum, wsum;

  psum = wsum = 0;
  for (i = n-1; i >= n-nf; i--) {
    if (xstar[i] == 1) {
      for (j = 0; j < i; j++) p[j][j] += p[i][j] + p[j][i];
      psum += p[i][i]; wsum += w[i];
    }
  }
  *ps = psum; *ws = wsum;
}


/* ======================================================================
                                   increase
   ====================================================================== */

void increase(int nf)
{
  register int i, j;

  for (i = n; i < n+nf; i++) {
    if (xstar[i] == 1) {
      for (j = 0; j < i; j++) p[j][j] -= p[i][j] + p[j][i];
    }
  }
}


/* ======================================================================
				  initsets
   ====================================================================== */

void initsets(void)
{
  register int i, j, l;
  register lpitem *k;
  register itemlist *u, *h, *g;
  register stype psum, wsum, c1;
  lpitem a[MSIZE];
  rowknap *row;

  for (i = 0; i < n; i++) {
    row = rowk + i;
    for (j = 0, k = a, h = row->it; j < n; j++, h++) {
      if (j == i) continue;
      h->p = k->p = p[i][j];
      h->w = k->w = w[j];
      k->e = k->p / (etype) k->w;
      k->x = (etype *) j;
      k++;
    }
    qsort(a, (int) (k-a), sizeof(lpitem), (funcptr) ecomp);
    g = &(row->head);
    for (j = 0, l = 1, k = a; j < n; j++) {
      if (j == i) continue;
      h = row->it + (int) (k->x);
      h->no = l; g->next = h;
      g = h; k++; l++;
    }
    g->next = &(row->tail);

    for (h = &(row->head), g = &(row->tail); h != g; h = h->next) {
      h->next->prev = h;
    }

    /* now finally find break item */
    psum = wsum = 0; c1 = c - w[i];
    for (h = row->head.next, g = &(row->tail); h != g; h = h->next) {
      if (wsum + h->w > c1) break;
      psum += h->p; wsum += h->w;
    }
    row->psumb = psum; row->wsumb = wsum; row->b = h;
    row->head.no = 0; row->head.prev = NULL;
    row->tail.no = n; row->tail.next = NULL;
    row->head.p = 0; row->head.w = 0;
    row->tail.p = 0; row->tail.w = c;
    /* printf("row kp (%d,%d) c %d b (%d,%d)\n", psum,wsum,c1,h->p,h->w); */
  }
}


/* ======================================================================
				 removeitem 
   ====================================================================== */

void removeitem(int t)
{
  register itemlist *h;
  register rowknap *row;
  register int i;

  for (i = t+1, row = rowk+t+1; i < n; i++, row++) {
    h = &(row->it[t]);
    /* printf("%d: removing (%d,%d) no %d\n", i, h->p, h->w, h->no); */
    h->next->prev = h->prev;
    h->prev->next = h->next;
    h->psumb = row->psumb;
    h->wsumb = row->wsumb;
    h->b     = row->b;
    if (row->b == h) { /* equals b, move to next */
      row->b = h->next; continue;
    }
    if (h->no < row->b->no) { /* before b, subtract */
      row->psumb -= h->p; row->wsumb -= h->w;
    }
  }
}


/* ======================================================================
				 insertitem
   ====================================================================== */

void insertitem(int t)
{
  register itemlist *h;
  register rowknap *row;
  register int i;

  for (i = t+1, row = rowk+t+1; i < n; i++, row++) {
    h = &(row->it[t]);
    /* printf("inserting (%d,%d)\n", h->p, h->w); */
    h->next->prev = h;
    h->prev->next = h;
    row->b     = h->b; 
    row->psumb = h->psumb; 
    row->wsumb = h->wsumb;
  }
}


/* ======================================================================
                                 breakbound
   ====================================================================== */

stype breakbound(stype ps, stype ws, int t)
{
  register itemlist *h;
  register stype psumb, wsumb, c1;
  register rowknap *row;
  register lpitem *k;
  register int i;
  lpitem d[MSIZE];
  stype u, psum;

  /* printf("breakbound ps %d, ws %d, c1 %d t %d\n", ps, ws, c-ws, t); */
  for (i = t, k = d, row = rowk+t; i < n; i++, row++) {
    c1 = c - ws - w[i]; if (c1 < 0) continue; /* no space for row */
    h = row->b; psumb = row->psumb; wsumb = row->wsumb;
    if (wsumb > c1) { /* moving b bacward */
      do { 
        h = h->prev; psumb -= h->p; wsumb -= h->w;
      } while (wsumb > c1);
    } else { /* moving b forward */
      while (wsumb + h->w <= c1) {
        psumb += h->p; wsumb += h->w; h = h->next;
      }
    }
    row->b = h; row->psumb = psumb; row->wsumb = wsumb; 
    /* printf(" %d: found c1 %d b (%d,%d)\n", i, c1, h->p, h->w); */
    k->p = p[i][i] + psumb + (c1 - wsumb) * (double) h->p / h->w;
    k->w = w[i];
    k->e = k->p / (etype) k->w;
    k->x = NULL;
    k++;
  }
  psum = lpsolve(d, c-ws, (k-d));
  u = psum + ps;   /* fprintf(trace,"derived u %d = %d\n", u, u+fixp); */
  return u;
}


/* ======================================================================
                                quadbranch
   ====================================================================== */

void quadbranch(stype ps, stype ws, int t)
{
  register int j;
  stype u;

  iterations++;
  if (ws > c) error("Excess weight");

  if (ps > z) {
    z = ps; 
    /* printf("improved solution to %d\n", z); */
    for (j = 0; j < n; j++) xstar[j] = x[j];
  }

  if ((t != n) && (ws + minw[t] <= c)) {
    u = breakbound(ps, ws, t);
    if (u > z) {
      removeitem(t);
      if (ws + w[t] <= c) {
        for (j = t+1; j < n; j++) p[j][j] += p[j][t] + p[t][j];
        x[t] = 1;
        quadbranch(ps + p[t][t], ws + w[t], t+1);
        x[t] = 0;
        for (j = t+1; j < n; j++) p[j][j] -= p[j][t] + p[t][j]; 
      }
      quadbranch(ps, ws, t+1);
      insertitem(t);
    }
  }
}


/* ======================================================================
				quadknap
   ====================================================================== */

int quadknap(int no, int cap, int *ptab, int *wtab, int *xtab)
{
  int i, j;
  long time, ltime;
  stype ps, ws;
  ntype nf, fix;
  ntype ord[MSIZE], o1[MSIZE];

  /* initialize global variables */
  n    = no;         /* number of items */
  c    = cap;        /* capacity */
  z    = 0;          /* incumbent solution */
  nf   = 0;          /* number of variables fixed to some value */
  fixp = 0;          /* quadratic profit sum of variables fixed to 1 */
  fixw = 0;          /* weight sum of variables fixed to 1 */
  iterations = 0;    /* number of b&b nodes */
  maxlagr = 200 + n; /* max number of lagrange iterations */

  /* copy profits and weights to internal arrays */
  if (no > MSIZE) error("too many items");
  memcpy(p, ptab, sizeof(int) * MSIZE * MSIZE);
  memcpy(w, wtab, sizeof(int) * MSIZE);

  /* find initial solution */
  greedy();

  /* repeat: tighten bound, reduce */
  for (j = 0; j < n; j++) o1[j] = j;
  for (i = 1;; i++) {
    /* iterate multipliers */
    iterate(i);

    /* perform reduction */
    fix = reduce(ord);
    /* printf("%d: NOW REDUCED %d TOTAL REDUCED %d\n", i, fix, nf+fix); */
    if (fix == 0) break;
    reorder(ord, o1);
    decrease(fix, &ps, &ws);
    nf += fix; n -= fix; z -= ps; c -= ws; fixp += ps; fixw += ws;
  }
  /* printf("inibound %d, lagbound %d, greedy z %d\n", inibound,lagbound,z); */

  /* reorder according to descending values of pover */
  findorder(ord);
  reorder(ord, o1);

  /* construct table which makes it possible to derive bounds fast */
  initsets();
  findminw();

  /* branch-and-bound algorithm */
  for (j = 0; j < n; j++) x[j] = 0;
  quadbranch(0, 0, 0); 

  /* restore table */
  /* printf("increase n by %d, z by %d, c by %d\n", nf, fixp, fixw); */
  increase(nf);
  n += nf; z += fixp; c += fixw;
  invertorder(ord, o1);
  reorder(ord, o1);

  /* copy solution vector to xtab */
  memcpy(xtab, xstar, sizeof(int) * MSIZE);
  return z;
}


