#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int HEAP_ITEM;

typedef struct
{
    int capacity, count;
    HEAP_ITEM * item;
    char **key;
} HEAP;

void decr ( char * s )
{
    while ( *s != '\0' )
        if ( *s == '\n' )
            *s = '\0';
        else
            ++ s;
}

char * copy_string ( char * s )
{
    int size = strlen (s) + 1;
    char * copy = (char*) malloc ( size );
    snprintf(copy,size,"%s",s);
    return copy;
}

int heap_is_empty ( HEAP * heap )
{
    return heap->count <= 0;
}

int compare(int i, int j, char *key[]) {
		/*the sample data had two spaces bettwen the first and second elements
		i assumed we werent allowed us the 3rd column to help us sort so this was 
		the only way i could think of to ignore the last eleemnts in the list
		could just use strncmp() to compare whole string but i thought this was cheating??
		*/

		if(key[i][0] < key[j][0])
		{
			return -1;
		}

		if(key[i][0] > key[j][0])
		{

			return 1;
		}

		if(key[i][2] < key[j][2])
		{
			return -1;
		}	

		if(key[i][2] > key[j][2])
		{
			return 1;
		}
		
		if(key[i][3] < key[j][3])
		{
			return -1;
		}	

		if(key[i][3] > key[j][3])
		{
			return 1;
		}

    return 0;
}

void sift_down ( HEAP * heap, int k, int n )
{
    int i,j;
    HEAP_ITEM temp;
    int finished, compar;
    
    i = k;
    temp = heap->item[i];
    finished = 0;
    while( 2 * i + 1 < n && ! finished )
    {
        j = 2 * i + 1;
        
        if ( j < n-1 && compare ( heap->item[j+1], heap->item[j], heap->key) < 0 )
            ++j;
        
        compar = compare ( temp, heap->item[j], heap->key );
        
        if ( compar <
         0 )
            finished = 1;
        else
        {
            heap->item[i] = heap->item[j];
            i = j;
        }
    }
    heap->item[i] = temp;
}
/*
 * Build heap uses the array supplied,
 * does not copy it.
 */

HEAP * build_heap ( int count, char * key[] )
{
    HEAP * heap = (HEAP*) malloc(sizeof(HEAP));
    int i;
    int finished;
    
    heap->capacity = count;
    heap->count = count;
    heap->key = key;
    heap->item = malloc(count * sizeof(int));
    
    for(i=0;i<count; i++)
    {
        heap->item[i] = i;
    }

    if ( heap->count > 0 )
    {
        for (i=(heap->count-1)/2; i>=0; --i)
        {
            sift_down ( heap, i, heap->count );
        }
    }
    
    return heap;
}

char * minimum ( HEAP * heap )
{
    if ( heap_is_empty ( heap ) )
    {
        fprintf(stderr,"ERROR min called on empty heap, abort\n");
        exit(-1);
    }
    
    return heap->key[heap->item[0]];
}

int count_words ( FILE * file )
{
    int count;
    char word[500];  // Dangerous!
    
    rewind ( file );
    count = 0;
    while ( fscanf ( file, "%s", word ) == 1 )
        ++ count;
    
    return count;
}

void delete_min ( HEAP * heap )
{
    char * temp;
    
    if ( heap_is_empty ( heap ) )
    {
        fprintf(stderr,"ERROR min called on empty heap, abort\n");
        exit(-1);
    }
    
    if ( heap->count > 1 )
    {
        heap->item[0] = heap->item[ heap-> count - 1 ];
        sift_down ( heap, 0, heap->count -1 );
    }
    
    --heap->count;
}

int main ( int argc, char * argv[] )
{
    char word[10];
    int i = 0,count = 0;
    FILE * file;
    char **key;
    HEAP * heap;
    
    file = fopen ( argv[1], "r" );
    
    count = count_words ( file );
    key = ( char ** ) calloc ( count, sizeof( char * ) );
    rewind ( file );
    
    while (fgets( word, 10, file) != NULL )
    {
        decr(word);
        key[i++] = copy_string ( word );
    }

    //printf("%s",key[1]);
    heap = build_heap ( i, key );
    
    printf("Sorted:\n");
    while (! heap_is_empty ( heap ) )
    {
        printf("%s\n", minimum ( heap ) );
        delete_min (heap);
    }
    
    return 0;
}