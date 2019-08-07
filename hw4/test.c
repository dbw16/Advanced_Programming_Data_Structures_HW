#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

//#define TABLE_SIZE 256
#define LONG sizeof(long)
// LONG can be 4 or 8, depending on machine
typedef char * LIST_ITEM;


typedef struct listss
{
	int item;
}listss;


long xor ( LIST_ITEM x )
{
	unsigned char acc[LONG];
	//unsigned char: suppresses sign extension
	//when converting to int.
	long * alias_acc = (long*) acc;
	long longform;
	int i,j;
	int finished;
	finished = 0;
	for (i=0; i<LONG; ++i)
		acc[i] = '\0';
	for (i=0; ! finished; ++i)
	{
		if ( x[i] == '\0' )
			finished = 1;
		else
		{
			j = i % LONG;
			acc[j] = (unsigned char)(acc[j] ^ (unsigned char) x[i]);
		}
	// NOTE: ^ is exclusive or
	}

	fprintf(stderr,"'%s': seed ", x);
	for (j=0; j<LONG; ++j)
		fprintf(stderr,"%02x", x[j]);
	fprintf(stderr,"\n");
	longform = * alias_acc;
	fprintf(stderr,"long form %ld\n", longform);
	return longform;
}

unsigned int hasher(char str[],int num_hashes,int bit_array_size)
{
	unsigned hash_list[500];
	listss *man;
	man->*item = 3;
	printf("here\n");
	printf("%d\n", man->item );
	printf("%s\n", str);
	srand48(xor(str));

	for (int i = 0; i < num_hashes; ++i)
	{
		hash_list[i] = (drand48() * (bit_array_size));
		//printf("%d\n",hash_list[i] );
	}
	return 0;
}

int main()
{
	hasher("hello",3,200);
	return 0;
}