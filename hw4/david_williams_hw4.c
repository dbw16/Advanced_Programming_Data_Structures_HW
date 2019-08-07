#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
typedef char * LIST_ITEM;
#define LONG sizeof(long)


long xor ( const void *_str )// took this from the notes 
{
	const char *x = _str;
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
	longform = * alias_acc;
	return longform;
}

typedef unsigned int (*hash_function)(const void *data);
typedef struct bloom_filter * bloom_t;


struct bloom_filter 
{
    struct bloom_hash *func; // hash functions 
    void *bits;		//our bit array
    size_t size;	// our size so we know where the bit array ends 
};

bloom_t bloom_create(size_t size) // we want to creat our bit array of 0's 
{
	bloom_t x = calloc(1, sizeof(struct bloom_filter));
	x->size = size;
	x->bits = malloc(size);
	return x;
}

void bloom_add(bloom_t filter, const void *item,int num_hashes) // this takes the items hashes them and addes them to our bitmaps
 {
	uint8_t *bits = filter->bits;
	
	unsigned int hash_list[100];
	int bit_array_size = filter->size;
	srand48(xor(item));

	for (int i = 0; i < num_hashes; ++i)
	{	
		hash_list[i] = (drand48() * (bit_array_size));
		//printf("%d\n",hash_list[i] );
	}
	unsigned int h1;
	for (int i = 0; i < num_hashes; ++i)
	{
		h1 = hash_list[i] % (filter->size * 8);
		bits[h1 / 8] |= 1 << h1 % 8;
	}
}

bool bloom_test(bloom_t filter, const void *item, int num_hashes) //lets us see is this in our bit array 
{
	uint8_t *bits = filter->bits;
	unsigned int hash_list[100];
	int bit_array_size = filter->size;


	//printf("%s\n", item);
	srand48(xor(item));

	for (int i = 0; i < num_hashes; ++i)
	{
		hash_list[i] = (drand48() * (bit_array_size));
		//printf("%d\n",hash_list[i] );
	}
	unsigned int h1;
	for (int i = 0; i < num_hashes; ++i)
	{	
		h1 = hash_list[i] % (filter->size * 8);
		if (!(bits[h1 / 8] & 1 << h1 % 8)) 
			return false;
	}
	return true;
}

int main(int argc, char * argv[])
 {
    int lenght;
    char buffer[25];

    /*****************
   	We will start by reading in the data line by line and saveing it into 
   	a string array.
    *******************/

    FILE *fptr;
    if ((fptr = fopen("04.words", "r")) == NULL)
    {
        printf("Error! opening file, Im using fopen(04.words, r) to open file and read\n");
        printf("line by line. Please put a file called 04.words.txt in the same directory as program\n");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    // reading in the first line from data to get lenght
    fscanf(fptr, "%[^\n]\n", buffer);
    lenght = atoi(buffer);

    //creating a varible size(of lenght given) array of strings 
    char **items;
    items = malloc(lenght * sizeof(char*));

    for(int i = 0; i < lenght; i++) 
    {
  		items[i] = malloc((25+1) * sizeof(char));
	}

    int count = 0;
    while (EOF != fscanf(fptr, "%[^\n]\n", buffer))
    {
        strcpy(items[count], buffer);
        //printf("%s\n",items[count] );
        count = count + 1;
    }
    fclose(fptr);


    /*****************
    we want a FALSE POSTIVE rate of say 5 percent we know the number of items we have to store
    from the first line of the file.  we now use
    Buffersize = item_number(-ln[p]/ln[2]^2)
    and use this to fugure out the optimal number of hashes using
    hash_number = buffer_size/number_items * ln[2]
    *******************/

    int bitmap_size = lenght * -( log(.05) / 0.480453);
    int number_hashes = (bitmap_size*log(2) )/ lenght ;
    printf("optimal bit array size %d, optimal number of hashes %d for 5percent fp \n", bitmap_size,number_hashes);
   

    /*****************
  	Now we create a bloom filter of this size and check the number of false postive we get while adding items to it
    *******************/


    bloom_t bloom = bloom_create(bitmap_size);//if we pick a larger number here we get less false postives. 
	int flase_postatives = 0;
	for (int i = 0; i<lenght; i++)
	{
		
		int tester = 0;
		for (int j = 0; j<i; j++)//this bit of code checks to see if the the items has come up PREVIOUSLY in the array... 
		{
			if (strcmp (items[i], items[j]) == 0)
			{
				tester = 1;	//if it has already been sean then we know its been added to BF
			}	
		}

		if (tester == 1 && bloom_test(bloom, items[i],number_hashes) == 1)
		{
			printf("%s:  IS in array,  IS probably in filter, adding item to filter\n",items[i]);
		}
		else if (tester == 1 && bloom_test(bloom, items[i],number_hashes) == 0)
		{
			printf("%s: hmmm should not be seing this at all ending program\n",items[i]);
			return -1;
		}
		else if (tester == 0 && bloom_test(bloom, items[i],number_hashes) == 0)
		{
			printf("%s:  is NOT in array,  Not in filter, adding item to filter\n",items[i]);
		}
		else if (tester == 0 && bloom_test(bloom, items[i],number_hashes) == 1)
		{
			printf("%s:  is Not in array, is in Filter      FALSE POSATIVE\n",items[i]);
			flase_postatives = flase_postatives + 1;
		}
		// we now add item to filter
		bloom_add(bloom, items[i],number_hashes);
	}
	float percent = (1000*flase_postatives/(lenght))/10.0;
	printf("\nOut of %d words, %d false positives, %f percent FP\n", lenght,flase_postatives, percent);

	return 0;
}
