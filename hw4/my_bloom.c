#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned int (*hash_function)(const void *data);
typedef struct bloom_filter * bloom_t;

//Stucters
struct bloom_hash 
{
    hash_function func;  //a pointer to a hash function 
    struct bloom_hash *next; // a pointer to the next hash function in our linked list of hashfucntiopns
};


struct bloom_filter 
{
    struct bloom_hash *func; // hash functions 
    void *bits;		//our bit array
    size_t size;	// our size so we know where the bit array ends 
};



//functions 

bloom_t bloom_create(size_t size) // we want to creat our bit array of 0's 
{
	bloom_t x = calloc(1, sizeof(struct bloom_filter));
	x->size = size;
	x->bits = malloc(size);
	return x;
}

void bloom_add_hash(bloom_t filter, hash_function func)
 {
	struct bloom_hash *h = calloc(1, sizeof(struct bloom_hash));
	h->func = func;
	struct bloom_hash *last = filter->func;
	while (last && last->next) {
		last = last->next;
	}
	if (last) {
		last->next = h;
	} else {
		filter->func = h;
	}
}//we add a hash function to our bloom filter 

void bloom_add(bloom_t filter, const void *item) // this takes the hashes and addes them to our bitmaps
 {
	struct bloom_hash *h = filter->func;
	uint8_t *bits = filter->bits;
	while (h) {
		unsigned int hash = h->func(item);
		hash %= filter->size * 8;
		bits[hash / 8] |= 1 << hash % 8;
		h = h->next;
	}
}



bool bloom_test(bloom_t filter, const void *item) //lets us see is this in our bit array 
{
	struct bloom_hash *h = filter->func;
	uint8_t *bits = filter->bits;
	while (h) {
		unsigned int hash = h->func(item);
		hash %= filter->size * 8;
		if (!(bits[hash / 8] & 1 << hash % 8)) {
			return false;
		}
		h = h->next;
	}
	return true;
}

unsigned int djb2(const void *_str) {
	const char *str = _str;
	unsigned int hash = 5381;
	char c;
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

unsigned int jenkins(const void *_str) {
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

int main() {
	bloom_t bloom = bloom_create(8);
	bloom_add_hash(bloom, djb2);
	bloom_add_hash(bloom, jenkins);
	printf("Should be 0: %d\n", bloom_test(bloom, "hello world"));
	bloom_add(bloom, "hello world");
	printf("Should be 1: %d\n", bloom_test(bloom, "hello world"));
	printf("Should (probably) be 0: %d\n", bloom_test(bloom, "world hello"));
	return 0;
}


