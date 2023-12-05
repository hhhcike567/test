// sample.c
#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <unistd.h>

int fn(void *arg)
{
   printf("\nINFO: This code is running under child process.\n");

   int i = 0;
   
   int n = atoi(arg);

   for ( i = 1 ; i <= 10 ; i++ )
      printf("%d * %d = %d\n", n, i, (n*i));

   printf("\n");

   return 0;
}

void main(int argc, char *argv[])
{
   printf("Hello, World!\n");

   void *pchild_stack = malloc(1024 * 1024);
   if ( pchild_stack == NULL ) {
      printf("ERROR: Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS;

   int pid = clone(fn, pchild_stack + (1024 * 1024), flags, argv[1]);
   if ( pid < 0 ) {
        printf("ERROR: Unable to create the child process.\n");
        exit(EXIT_FAILURE);
   }

   wait(NULL);

   free(pchild_stack);

   printf("INFO: Child process terminated.\n");
}
