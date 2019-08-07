#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char * HEAP_ITEM;

typedef struct
{
    int capacity, count;
    HEAP_ITEM * item;
    //H
} HEAP;

int compare ( int i, int j, char * key[] )
{
    int result = strcmp(key[i],key[j]);
    return result;
}

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

void sift_up ( HEAP * heap, int k )
{
    int i,j;
    char * temp;
    int finished, compar;
    
    i = k;
    temp = heap->item[i];
    finished = 0;
    while( i > 0 && ! finished )
    {
        j = (i-1)/2;
        compar = strcmp ( heap->item[j], temp );
        if ( compar <= 0 )
        {
            heap->item[i] = temp;
            finished = 1;
        }
        else
        {
            heap[i] = heap[j];
            i = j;
        }
    }
}

void sift_down ( HEAP * heap, int k, int n )
{
    int i,j;
    char * temp;
    int finished, compar;
    
    i = k;
    temp = heap->item[i];
    finished = 0;
    while( 2 * i + 1 < n && ! finished )
    {
        j = 2 * i + 1;
        
        if ( j < n-1 && strcmp ( heap->item[j+1], heap->item[j] ) < 0 )
            ++j;
        
        
        compar = strcmp ( temp, heap->item[j] );
        
        if ( compar <= 0 )
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
    heap->item = key;
    
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
    
    return heap-> item[0];
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
    
    -- heap->count;
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

void show_heap ( HEAP * heap )
{
    int i;
    printf("heap count %d\n", heap->count);
    for (i=0; i<heap->count; ++i)
        printf("%s\n", heap->item[i]);
}

int main ( int argc, char * argv[] )
{
    char word[500];
    int i, count, number_of_lines;
    FILE * file;
    char ** key;
    HEAP * heap;
    
    file = fopen ( argv[1], "r" );
    
    
    count = count_words ( file );
    number_of_lines = count / 3 ;
    printf("%d \n",count);
    key = ( char ** ) calloc ( count, sizeof( char * ) );
    
    rewind ( file );
    
    for (i=0; i<number_of_lines; ++i )
    {
        fgets(word, sizeof(word), file);
        //fscanf(file, "%s", word );
        key[i] = copy_string ( word );
    }
    
    heap = build_heap ( number_of_lines, key );
    
    printf("Sorted:\n");
    while (! heap_is_empty ( heap ) )
    {
        printf("%s\n", minimum ( heap ) );
        delete_min (heap);
    }
    return 0;
}
