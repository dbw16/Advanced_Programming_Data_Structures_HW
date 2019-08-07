#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int HEAP_ITEM;

typedef struct
{
  int capacity, count;
  HEAP_ITEM * item;
  //HEAP * key;

} HEAP;

int compare ( int i, int j, char * key[] )
{
  int result = strcmp(key[i],key[j]);
  return result;
}


int heap_is_empty ( HEAP * heap )
{
  return heap->count <= 0;
}

void sift_up ( HEAP * heap, int k )
{
  int i,j;
  HEAP_ITEM temp;
  int finished, compar;

  i = k;
  temp = heap->item[i];
  finished = 0;
  while( i > 0 && ! finished )
  {
    j = (i-1)/2;
    compar = heap->item[j] - temp;
    if ( compar <= 0 )
    {
      finished = 1;
    }
    else // Once the else-branch has been taken,
	 // heap order has been restored at the
	 // expense of duplicating one of the heap
	 // elements.  The parent is copied to i.
    {
      heap->item[i] = heap->item[j];
      i = j;
    }
    heap->item[i] = temp;
  }
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
	// i has at least one child
  {
    j = 2 * i + 1;

    if ( j < n-1 && heap->item[j+1] - heap->item[j] < 0 )
     
      ++j;
	// Always raise the lighter child


    compar = temp - heap->item [j];

	// if compar <= 0, then temp is <= both
	// children of i, and will overwrite i.

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

HEAP * build_heap ( int count, int key[] )
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

	/*
	 * minimum and delete_min
	 */

HEAP_ITEM minimum ( HEAP * heap )
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
  HEAP_ITEM temp;

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

void show_heap ( HEAP * heap )
{
  int i;
  printf("heap count %d\n", heap->count);
  for (i=0; i<heap->count; ++i)
    printf("%d\n", heap->item[i]);
}

int main ( int argc, char * argv[] )
{
  char nums[1000];
  int i,count;
  FILE * file;
  HEAP * heap;

  file = fopen ( argv[1], "r" );
  //file = stdin;
  
  count = 0;
  int line_len = 0;

  int n;
  long elapsed_seconds;
  char line[80];
  clrscr();

   static const char filename[] = "file.txt";
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      //char line [ 128 ]; /* or other suitable maximum line size */

      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
         fputs ( line, stdout ); /* write the line */
      }
      fclose ( file );
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
   }


/*
  while ( fscanf (file, "%s", & ( nums[ count ] ) ) == 1 )
  {
    char buf[100];
    snprintf ( buf, 100, " %c", nums[count] );
    int len = strlen(buf) + 1;
    if ( line_len + len > 70 )
    {
      line_len = 0; printf("\n");
    }
    printf("%s", buf);
    line_len += len;
    ++count;
  }
  printf("\n");
*/
  heap = build_heap ( count, nums );

  printf("Sorted:\n");
  line_len = 0;
  while (! heap_is_empty ( heap ) )
  {
    char buf[100];
    snprintf(buf, 100, " %d", minimum ( heap ) );

    int len = strlen(buf) + 1;
    if ( line_len + len > 70 )
    {
      line_len = 0; printf("\n");
    }
    printf("%s", buf);
    line_len += len;

    delete_min (heap);
  }
  printf("\n");
  return 0;
}
