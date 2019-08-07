#include <stdio.h>
#include <stdlib.h>

void sort ( int *array[], int count, int width,
                int field[], int field_count );

		/*
		 * this is the single public routine
	 	 * in binsort.c, which can be compiled
		 * together with this main program.
		 */
	 	 
main( int argc, char * argv[])
{
  int count, width;

  int field[100];
  int i;
  for ( i=1; i<argc; ++i )
    field[i-1] = atoi( argv[i] );

  int field_count = argc-1;

  scanf ( "%d %d", & count, & width );

/*
for (i=0; i<field_count; ++i)
printf("field[%d] %d\n", i, field[i]);
printf("count %d width %d\n", count, width);
*/

  int ** line = (int**) calloc(count, sizeof(int*));

  int * linepool = calloc ( count, width * sizeof ( int ) );
  for (i=0; i<count; ++i)
    line[i] = linepool + width * i;

  int x,j;

  for ( i=0; i<count; ++i)
    for ( j=0; j<width; ++j)
      scanf("%d", & ( line[i][j] ) );

  sort (line, count, width, field, field_count);

  for (i=0; i<count; ++i)
  {
    for (j=0; j<width; ++j)
      printf(" %4d", line[i][j]);
    printf("\n");
  }
}
