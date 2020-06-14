#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "ticketlock.h"

#define PGSIGE 4096
#define MAXT 32

typedef struct threads
{
  int busy;
  void *mem;
  void *stack;
} thtable;

thtable ths[MAXT];
static lock_t lock = {
    .ticket = 0,
    .turn = 0,
};

void lock_init(lock_t *lk)
{
  lk->ticket = 0;
  lk->turn = 0;
}

void lock_acquire(lock_t *lk)
{
  int myturn = fetch_and_add(&lk->ticket, 1);
  while (lk->turn != myturn)
    ;
}

void lock_release(lock_t *lk)
{
  lk->turn = lk->turn + 1;
}

char *
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while ((*s++ = *t++) != 0)
    ;
  return os;
}

int strcmp(const char *p, const char *q)
{
  while (*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint strlen(const char *s)
{
  int n;

  for (n = 0; s[n]; n++)
    ;
  return n;
}

void *
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char *
strchr(const char *s, char c)
{
  for (; *s; s++)
    if (*s == c)
      return (char *)s;
  return 0;
}

char *
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for (i = 0; i + 1 < max;)
  {
    cc = read(0, &c, 1);
    if (cc < 1)
      break;
    buf[i++] = c;
    if (c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if (fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int atoi(const char *s)
{
  int n;

  n = 0;
  while ('0' <= *s && *s <= '9')
    n = n * 10 + *s++ - '0';
  return n;
}

void *
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while (n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  void *m = malloc(PGSIGE * 2);
  void *stack;
  if ((uint)m % PGSIGE == 0) // is aligned
    stack = m;
  else
    stack = (uint)m + (PGSIGE - ((uint)m % PGSIGE)); // not aligned
  int i;
  for (i = 0; i < MAXT; i++)
  {
    int flag = 0;
    lock_acquire(&lock);
    if (ths[i].busy == 0)
    {
      ths[i].busy = 1;
      ths[i].mem = m;
      ths[i].stack = stack;
      flag = 1;
    }
    lock_release(&lock);
    if (flag == 1)
      break;
  }
  if (i == MAXT)
    return -1;
  int res = clone(start_routine, arg1, arg2);
  return res;
}

int thread_join()
{
  void *stack;
  int res = join(&stack);
  for (int i = 0; i < MAXT; i++)
  {
    if (ths[i].busy == 1 && ths[i].stack = stack)
    {
      free(ths[i].mem);
      ths[i].busy = 0;
      ths[i].stack = 0;
      break;
    }
  }
  return res;
}