#include <stdio.h>
#include <stdlib.h>

typedef struct INT_NODE
{
  int * item;
  struct INT_NODE * next;
} INT_NODE;


typedef struct INT_LIST
{
 INT_NODE * first, * last;
} INT_LIST;

static void append ( INT_NODE * x, INT_LIST * list )
	/* append node to list, not item */
{
  /* to be completed */
}


static INT_LIST * make_int_list ( int *array[], int count, int width )
{
  int i;
  INT_LIST * list = (INT_LIST * ) calloc(1, sizeof( INT_LIST ) );
  INT_NODE * p = NULL;

  for (i=0; i<count; ++i)
  {
    INT_NODE * q = (INT_NODE *) calloc (1, sizeof (INT_NODE) );
    q->item = array[i];
    
    if ( p == NULL )
      list->first = q;
    else
      p->next = q;
    list->last = q;
    p = q;
  }


  return list;
}

void print_list(INT_LIST * list,int k)
{
      INT_NODE *test = list->first;
      printf("first columnof array\n");
      for (int j = 0; j<10 ;j++ )
      {
        printf("%d\n", (test->item)[k]);
        test = test->next;
      }
      printf("\n\n");


}

static void free_int_list ( INT_LIST * list )
{
  /*
   * this code is complete
   */

  INT_NODE * p = list->first;
  while ( p != NULL )
  {
    INT_NODE * q = p->next;
    free(p);
    p = q;
  }
  free ( list );
}

static void sort_on_column ( INT_LIST * list, int count, int k )
{
  /******************************************************************
  a->B-c->D->e
  if we want to swap postions of B and D in general we need to save
  what goes in and out of each. so i will creat 4 variblea called 
  1.before_marker = a
  2. after_maker = c
  3. before_moved = c
  4. after_moved =e
  using these we can sort our list using an insert sort method
 4 cases  in genral
  b<a
  c<a
  c<b
  d<b
  ********************************************************************/
  fprintf(stderr,"sort on column %d\n", k);
  int i=0;
  INT_NODE *pos = list->first; 

  //empty or list of lenght one
  if (pos==NULL || pos->next == NULL)
    return;

  while(pos->next != NULL)
  {
    i = i+1;
    if (i>15)
    {
      printf("ended earl;y*********\n");
      return;
    }
    INT_NODE *after_moved;
    INT_NODE *before_moved;
    INT_NODE *moved;
    
    moved = pos->next;
    after_moved = moved->next;
    before_moved = pos;



    if ( (moved->item)[k] < (pos->item)[k])
    {
      //printf("we are going to move %d in front of %d\n", (moved->item)[k],(pos->item)[k]);
      
      if ( list->first == pos && (list->first->item)[k] > (moved->item)[k] )
      {
        INT_NODE *old_head;
        old_head = list->first; 
        after_moved = moved->next;
        list->first = moved;
        moved->next = old_head;
        old_head->next = after_moved;
      }
      else if ((list->first->item)[k] > (moved->item)[k])
      {
        INT_NODE *old_head;
        old_head = list->first; 
        moved->next = old_head;
        before_moved->next = after_moved;
        list->first = moved;
      }
      else
      {
        INT_NODE *marker;
        marker = list->first->next;
        
        INT_NODE *after_marker;
        INT_NODE *before_marker;
        before_marker = list->first; 
        after_marker = marker->next;
       
        while(marker->next != NULL)
        {
          
          if (after_marker == moved && (marker->item)[k] > (moved->item)[k])
          {
            before_marker->next = moved;
            moved->next = marker;
            marker->next = after_moved;
            break;
          }
          else if ( (marker->item)[k] > (moved->item)[k]) 
          {

            before_marker->next = moved;
            moved->next = marker;
            before_moved->next = after_moved;
            //print_list(list,k);
            break;
          }
          else
          {
            before_marker = marker;           
            marker = marker->next;
            after_marker = marker->next;
          }

         }
        }
      }
    
    else
    {
      pos = pos ->next;
    }
  }

  //this bit isnt actully need to achive the resultes but i thought i
  //may as well keep it for completteness sake
  INT_NODE *search = list->first;
  while(search!=NULL )
  {
    if (search->next==NULL )
    {
      list->last = search;
    }
    search = search->next;
  }
}






void sort ( int *array[], int count, int width,
		int field[], int field_count )
{
  int i;
  //first we are going to store rows into nodes(INT_NODE) of a singley linked list(INT_LIST)
  INT_LIST *my_list = make_int_list(array, count, width);
  
  /*
  for(i=0; i < field_count; i++) ****need to make this decend not accending*****
  {
    sort_on_column(my_list, count, field[i]);
  }
  */

  for(i = field_count-1; i != -1; --i) 
  {
    sort_on_column(my_list, count, field[i]);
  }

  INT_NODE * pos;
  pos = my_list->first;
  for(int i = 0; i<count; i++)
  {
      array[i] = pos->item;
      pos = pos->next;
  }
  return;
  //gives the same as the sample output!!!!
}
