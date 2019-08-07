#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned int (*hash_function)(const void *data);
typedef struct bloom_filter * bloom_t;

// i copyed both these hash functions from the internet...
unsigned int hash1(const void *_str) //this is djb2 hahser
{
	const char *str = _str;
	unsigned int hash = 5381;
	char c;
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

unsigned int hash2(const void *_str)//this is jenkins hasher
 {
	const char *key = _str;
	unsigned int hash, i;
	while (*key) {
		hash += *key;
		hash += (hash << 10);
		hash ^= (hash >> 6);
		key++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}



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

void bloom_add(bloom_t filter, const void *item) // this takes the items hashes them and addes them to our bitmaps
 {
	uint8_t *bits = filter->bits;
	
	unsigned int h1 = hash1(item);
	unsigned int h2 = hash2(item);
	
	h1 = h1 % (filter->size * 8);
	bits[h1 / 8] |= 1 << h1 % 8;

	h2 = h2 % (filter->size * 8);
	bits[h2 / 8] |= 1 << h2 % 8;

}

bool bloom_test(bloom_t filter, const void *item) //lets us see is this in our bit array 
{
	uint8_t *bits = filter->bits;

	unsigned int h1 = hash1(item);
	unsigned int h2 = hash2(item);

	h1 = h1 % (filter->size * 8);
	if (!(bits[h1 / 8] & 1 << h1 % 8)) 
		return false;
	
	h2 = h2 % (filter->size * 8);
	if (!(bits[h2 / 8] & 1 << h2 % 8)) 
		return false;

	return true;
}

int main(int argc, char * argv[])
 {
    int lenght;
    char buffer[25];
    bloom_t bloom = bloom_create(256);//if we pick a larger number here we get less false postives. 

    //char filename = argv[1];
    FILE *fptr;

    if ((fptr = fopen("04.words.txt", "r")) == NULL)
    {
        printf("Error! opening file");
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


	int flase_postatives = 0;
	for (int i = 0; i<lenght; i++)
	{
		
		int tester = 0;
		for (int j = 0; j<i; j++)//this bit of code checks to see if the the items has come up prevously in the array... 
		{
			if (strcmp (items[i], items[j]) == 0)
			{
				tester = 1;	
			}	
		}

		if (tester == 1 && bloom_test(bloom, items[i]) == 1)
		{
			printf("%s:  IS in array,  IS probably in filter, adding item to filter\n",items[i]);
		}
		else if (tester == 1 && bloom_test(bloom, items[i]) == 0)
		{
			printf("hmmm should not be seing this at all ending program\n");
			return -1;
		}
		else if (tester == 0 && bloom_test(bloom, items[i]) == 0)
		{
			printf("%s:  is NOT in array,  Not in filter, adding item to filter\n",items[i]);
		}
		else if (tester == 0 && bloom_test(bloom, items[i]) == 1)
		{
			printf("%s:  is Not in array, is in Filter      FALSE POSATIVE\n\n",items[i]);
			flase_postatives = flase_postatives + 1;
		}
		// we now add item to filter
		bloom_add(bloom, items[i]);
	}
	printf("\nOut of %d words, %d false positives\n", lenght,flase_postatives);











	//printf("Should be 0: %d\n", bloom_test(bloom, "hello world"));
	//printf("Should be 1: %d\n", bloom_test(bloom, "hello world"));
	//printf("Should (probably) be 0: %d\n", bloom_test(bloom, "world hello"));
	return 0;
}
