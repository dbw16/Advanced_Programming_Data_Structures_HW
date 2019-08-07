#include <stdio.h>
#include <stdlib.h>

	// Edges must be kept in a circular list.

typedef struct D_EDGE 
{
  struct D_EDGE * next, * prev;
  int from, to;
} D_EDGE;

typedef struct
{
  int vertex_count, edge_count;
  D_EDGE ** an_outedge;        // This is an array  of
				// pointers to edges. Any
				// edge coming out of vertex 
				// i can be used to access all.
  int * pre, * earlier;
} DIGRAPH;

//An array on_stack[] is needed.  There are various
//places where it might be stored....


D_EDGE * make_dir_edge ( int from, int to )
{
  D_EDGE *result = (D_EDGE *) calloc(1, sizeof (D_EDGE));
  result->from = from; result->to = to;
  return result;
}

	/*
	 * read_graph.
	 * Input format n m <list0> .. <list n-1>
	 * where <list i> is <#edges(=k), v0, .., v_{k-1}
	 */

DIGRAPH * read_graph ( FILE * f )
{
  DIGRAPH * graf;
  int n, m;  // vertices and edges
  int i, j, ii, jj, k;
  D_EDGE * first, * edge, * last;

  graf = (DIGRAPH *) calloc ( 1, sizeof ( DIGRAPH ));

  fscanf (f,"%d %d", &n, &m );

  graf -> vertex_count = n;
  graf -> edge_count = m;

  graf -> an_outedge = (D_EDGE **) calloc ( n, sizeof ( D_EDGE *) );
  graf -> pre = (int*) calloc(n, sizeof(int));
  graf -> earlier = (int*) calloc(n, sizeof(int));

  for (i=0; i<n; ++i)
  {
    graf->pre[i] = graf->earlier[i] = -1;
  }

  for ( i=0; i<n; ++i )
  {
    int size;
    fscanf (f, "%d %d", &ii, &size );

    if ( ii != i )
    {
      fprintf(stderr,"ERROR read graph expected i %d got %d abort\n",
	i, ii);
      exit(-1);
    }

    first = NULL;
    for (k=0; k<size; ++k)
    {
      fscanf (f, "%d", &j);
      edge = make_dir_edge ( i, j );
      if ( first == NULL )
      {
        graf->an_outedge[i] = first = edge;
        edge -> next = edge -> prev = edge;
      }
      else
      {
        last = first->prev;
        edge->next = first;
        first->prev = edge;
        edge->prev = last;
        last->next = edge;
      }
    }
  }   
  return graf;
}


int outdegree ( DIGRAPH * graf, int i )
{
  D_EDGE * e, * ee;
  ee = e = graf->an_outedge[i];

  if ( e == NULL )
    return 0;

  int count = 0;
  int timeout = 100;

  do
  {
    ++ count;
    ee = ee->next;
  }
  while ( timeout-- > 0 && ee != e );

  if ( timeout <= 0 )
  {
    fprintf(stderr,"timed out\n");
    exit(-1);
  }

  return count;
}


void show_graph ( DIGRAPH * graf )
{
  int size, i, j;
  D_EDGE * e;

  printf("show graph %d vertices %d edges\n",
	graf->vertex_count, graf->edge_count);

  for (i=0; i<graf->vertex_count; ++i)
  {
    printf("%3d:", i);
    e = graf->an_outedge[i];
    size = outdegree( graf, i);
    printf( " %d", size);
    for (j=0; j<size; ++j)
    {
      printf(" %d", e->to);
      e = e->next;
    }
    printf("\n");
  }
  printf("\n");
}

typedef struct
{
  int * array;
  int capacity;
  int topindex;
} STACK;

STACK * make_stack ( int  n )
{
  STACK * stack = (STACK *) calloc(1,sizeof(STACK));
  stack->array = (int*) calloc( n, sizeof(int) );
  stack->topindex = -1;
  stack->capacity = n;
  return stack;
}

int top ( STACK * s )
{
  return s->array[ s->topindex ];
}

int stack_height ( STACK * s )
{
  return s->topindex + 1;
}

int is_empty ( STACK * s )
{
  return s->topindex < 0;
}

void pop ( STACK * s )
{
  int i = s->array[s->topindex];
  -- ( s->topindex );
}

void push ( int x, STACK * s )
{
  ++ ( s->topindex );
  s->array[ s->topindex ] = x;
}

int min(int x, int y)
{
  if (x>y)
  {
    return y;
  }
  return x;
}



void scc ( int u, DIGRAPH * graf, STACK * st, int stackMember[])
{
  static int time = 0;
  D_EDGE * e;
  e = graf->an_outedge[u];
  graf->pre[u] = graf->earlier[u]= time++;
  
  int size = outdegree( graf, u);

  //should probly add somthing to the stack here????
    push(u,st);
    stackMember[u] = 1;



  //This for loop is wrong... it should look thoug every edge or something?
  for(int i =0; i<size; i++ )
  {
    if(graf->pre[e->to] < 0)
    {
      scc(e->to , graf, st ,stackMember);
      
      graf->earlier[u]= min(graf->earlier[u], graf->earlier[e->to]);
    }
    else if(stackMember[e->to] == 1)
    {
      graf->earlier[u]= min(graf->earlier[u], graf->pre[e->to]);   
    }

  }


    // head node found, pop the stack and print an SCC
    int w = 0;  // To store stack extracted vertices
    if (graf->earlier[u] == graf->pre[u])
    {
        while (top(st) != u)
        {
            w = top(st);
            //cout << w << " ";
            printf("%d ", w );
            stackMember[w] = 0;
            pop(st);
        }
        w = top(st);
        printf("%d\n", w );
        stackMember[w] = 0;
        pop(st);
    }
}

void traverse ( DIGRAPH * graf )
{
  STACK * s = make_stack ( graf->vertex_count );

  int stackMember[100];
  
  for(int i=0; i<100; i++)
    {
      stackMember[i]=-1;
    }

  int pre=0, scc_count = 0;

  for (int i=0; i < graf->vertex_count; ++i )
  {
    if ( graf->pre[i] < 0 )
      scc ( i , graf, s, stackMember );
  }
}



int main(int argc, char *argv[])
{
  FILE *file = stdin;
  int i, pre, post, j, k, ell;
  DIGRAPH * graf;

  graf = read_graph ( file );
  if ( file != stdin )
    fclose ( file );

  show_graph ( graf );
  traverse ( graf );
  return 0;
}
