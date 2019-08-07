#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef char * KEY_TYPE;

typedef struct RBST_NODE
{
  KEY_TYPE key;
  struct RBST_NODE * left, * right, * parent;
  int is_red;
  int depth, inorder_rank;
} RBST_NODE;

typedef struct
{
  int rb_rank;
  RBST_NODE * root;
} RBST;
  
void decr ( char * x )
{
  while ( *x != '\0' )
    if ( *x == '\n' )
      *x = '\0';
    else
      ++x;
}

int all_blank ( char * x )
{
  while ( *x != '\0' )
    if ( isspace ( *x ) )
      ++x;
    else
      return 0;

  return 1;
}

char * copy_string ( char * s )
{
  int size = strlen(s) + 1;
  char *copy;

  copy = (char*) malloc ( size );
  snprintf(copy, size, "%s", s );
  return copy;
}

RBST_NODE * make_rbst_node ( char * s )
{
  RBST_NODE * result = (RBST_NODE*) calloc ( 1, sizeof (RBST_NODE) );
  result->key = copy_string ( s );
  result ->depth = result->inorder_rank = -1;
  return result;
}

RBST * make_rbst()
{
  RBST * bst = (RBST*) calloc (1, sizeof (RBST) );
  bst->rb_rank = -1;
  return bst;
}

RBST_NODE * find_in_bst ( KEY_TYPE x, RBST * bst )
{
  RBST_NODE * p;
  int found, compar;
  
  p = bst->root;
  found = 0;

  while ( (p != NULL) && ! found )
  {
    compar = strcmp ( x, p->key );
    if ( compar == 0 )
      found = 1;
    else if ( compar < 0 )
      p = p->left;
    else
      p = p->right;
  }
  return p;
}

void rotate ( RBST_NODE * p, RBST * tree )
{
  // first we are going to find out if we need to left or right rotate
  // add parant 
  //grand parents not a thing?
  // check the if conditions if program isnt working
  RBST_NODE *dad = p->parent;
  RBST_NODE *grand_dad;
  RBST_NODE *great_grand_dad;

  if (dad != NULL)
  {
    grand_dad = dad->parent;
  }  
  RBST_NODE *temp;


  if(dad->left == p && grand_dad == NULL) //right rotaion root
  {
   temp = p->right;
   p->right = dad; 
   dad->parent = p; 
   dad->left = temp;
   if(temp!=NULL)
   {
    temp->parent = dad;
   }
   p->parent = NULL;
   tree->root = p;
   return;
  }
    
  else if(dad->right == p && grand_dad == NULL )  //left rotation root
  {
    temp = p->left;
    p->left = dad;
    dad->parent = p;
    p->parent = NULL;
    dad->right = temp;
   if(temp!=NULL)
   {
    temp->parent = dad;
   }
   p->parent = NULL;
   tree->root = p;
   return;
  }

  else if (dad->left == p && grand_dad->left == dad) //right rotaion zig_zig
  {
   temp = p->right;
   p->right = dad;
   dad->parent = p;  
   dad->left = temp;
   if(temp!=NULL)
   {
    temp->parent = dad;
   }
   grand_dad->left = p;
   p->parent = grand_dad;
   return;
  }
  else if(dad->left == p && grand_dad->right == dad)  //right rotation zig_zag
  {
   temp = p->right;
   p->right = dad;
   dad->parent = p;  
   dad->left = temp;
   if(temp!=NULL)
   {
    temp->parent = dad;
   }   grand_dad->right = p;
   p->parent = grand_dad;
   return;
  }

  else if(dad->right == p && grand_dad->right == dad)  //left rotation zig_zig
  {
    // this is the one that apears to work
    temp = p->left;
    p->left = dad;
    dad->parent = p;
    dad->right = temp;
    if (temp != NULL)
    {
    temp->parent = dad;
    }
    grand_dad->right = p;
    p->parent = grand_dad;
    return;

  }

  else if(dad->right == p && grand_dad->left ==  dad)  //left rotation zig_zag
  {
    temp = p->left;
    p->left = dad;
    dad->parent = p;
    dad->right = temp;
    if (temp != NULL)
    {
    temp->parent = dad;
    }    grand_dad->left = p;
    p->parent = grand_dad;
    return;
  }

}

void fix_double_red ( RBST_NODE * p, RBST * tree )
{

  //not sure about rank look at this
  RBST_NODE *dad = p->parent;
  RBST_NODE *grand_dad;
  if (dad != NULL)
  {
    grand_dad = dad->parent;
  } 
  RBST_NODE *sibling; 
  
  if (dad == NULL) //if it is the root make it black and increase rank 
  {
      p->is_red = 0;
      p->inorder_rank = (p->inorder_rank) +1;
      return;
  }
  else if (dad->is_red == 0) // if dad is black stop 
   {
     return;
   }

  if (grand_dad == NULL) // if dad is the root make it black and stop 
  {
    dad->is_red = 0;
    dad->inorder_rank = (dad->inorder_rank) +1;
    return;
  }
  else
  {
    //we want to know if it dad is left or right child to find where its sibling is?
    if (grand_dad->left == dad)
    {
      sibling = grand_dad->right;
    }
    else
    {
      sibling = grand_dad->left;
    }  
  }


  if (sibling != NULL && sibling->is_red==1)
  {
    sibling->is_red = 0;
    dad->is_red = 0;
    grand_dad->is_red = 1;
    //probbly need to call fix double red again now with grand dad??
    fix_double_red(grand_dad,tree);
    return;
  }
  if (dad->left == p && grand_dad->left == dad) //left child left child zig_zig
  {
    dad->is_red = 0;
    grand_dad->is_red = 1;
    rotate(dad, tree);
    return;
  }
  else if(dad->left == p && grand_dad->right == dad)  //left child right child zig_zag
  {
    rotate(p,tree);
    rotate(p,tree);
    grand_dad->is_red = 1;
    p->is_red = 0;
    return;
  }
  else if(dad->right == p && grand_dad -> right == dad)  //right child right child zig_zig
  {
    dad->is_red = 0;
    grand_dad->is_red = 1;
    rotate(dad, tree);
    return;
  }
  else if(dad -> right == p && grand_dad -> left ==  dad)  //left rotation zig_zag
  {
    grand_dad->is_red = 1;
    p->is_red = 0;
    rotate(p,tree);
    rotate(p,tree);
    return; 
  }

}



void insert ( KEY_TYPE x, RBST * bst )
{
  RBST_NODE * p, *q, *newnode;
  int found, compar;
  
  q = NULL;
  p = bst->root;
  found = 0;

  while ( (p != NULL) && ! found )
  {
    compar = strcmp ( x, p->key );
    if ( compar == 0 )
      found = 1;
    else
    {
      q = p;
       if ( compar < 0 )
        p = p->left;
      else
        p = p->right;
    }
  }

  if ( found )
  {
    fprintf(stderr,"ERROR key %s already stored, don't add\n", x);
    return;
  }
  else
  {
    newnode = make_rbst_node ( x );
    if ( q == NULL )
      bst->root = newnode;
    else if ( strcmp ( x, q->key ) < 0 )
      q->left = newnode;
    else
      q->right = newnode;

    newnode->parent = q;

    newnode->is_red = 1;
    fix_double_red ( newnode, bst );

  }
}

RBST_NODE * inorder_successor ( RBST_NODE * p )
{
  RBST_NODE * q, *r;

  if ( p == NULL )
  {
    fprintf ( stderr,"inorder succ of null, abort\n");
    exit(-1);
  }

  if ( p->right != NULL )
  {
    q = p->right;
    while ( q->left != NULL )
      q = q->left;

    return q;
  }
  else
  {
    q = p;
    r = q->parent;
    while ( r != NULL && q != r->left )
    {
      q = r;
      r = q->parent;
    }
    return r;
  }
}


void delete_immediate ( RBST_NODE * p, RBST * bst )
{
  if ( p==NULL )
  {
    fprintf(stderr,"ERROR deleting null, abort\n");
     exit(-1);
  }
  else if (p->left != NULL && p->right != NULL)
  {
    fprintf(stderr,"ERROR delete imm %s, two children, abort\n",
	p->key);
    exit(-1);
  }
  else if ( p->left == NULL )
  {
    if ( p->parent == NULL )
      bst->root = p->right;
    else if ( p->parent->left == p )
      p->parent->left = p->right;
    else
      p->parent->right = p->right;

    if ( p->right != NULL )
      p->right->parent = p->parent;
  }
  else // if ( p->right == NULL )
  {
    if ( p->parent == NULL )
      bst->root = p->left;
    else if ( p->parent->left == p )
      p->parent->left = p->left;
    else
      p->parent->right = p->left;

    if ( p->left != NULL )
      p->left->parent = p->parent;
  }
}

void delete_from_bst ( KEY_TYPE x, RBST * bst )
{
  RBST_NODE * p, * q;

  p = find_in_bst ( x, bst );
  if ( p == NULL )
  {
    fprintf(stderr,"ERROR delete %s, not in tree, ignore\n", x);
    return;
  }

  if ( p->left == NULL || p->right == NULL )
    delete_immediate ( p, bst );
  else
  {
    q = inorder_successor ( p );
    if ( q->left != NULL )
    {
      fprintf(stderr,"ERROR inorder successor has left child, abort\n");
      exit(-1);
    }

    delete_immediate ( q, bst );
    q->parent = p->parent;
    if ( q->parent != NULL )
      if ( p->parent->left == p )
        q->parent->left = q;
      else
      {
        q->parent->right = q;
      }
    q->left = p->left;
    q->left->parent = q;
    q->right = p->right;
    q->right->parent = q;
  }
}

void install_ranks ( int * inorder, int depth, RBST_NODE * p )
{
  if ( p != NULL )
  {
    if ( p->left != NULL )
      install_ranks ( inorder, depth+1, p->left );
    p->inorder_rank = * inorder;
    p->depth = depth;
    ++ * inorder;
    if ( p->right != NULL )
      install_ranks ( inorder, depth+1, p->right );
  }
}

void recursive_show ( RBST_NODE * p )
{
  RBST_NODE * anc [ 1000 ];
  if ( p != NULL )
  {
    RBST_NODE * q = p;
    while ( q != NULL )
    {
      anc[ q->depth ] = q;
      q = q->parent;
    }
    recursive_show ( p->left );
    int i;

    for ( i=0; i< p->depth-1; ++i )
    {
      q = anc[i];
      int x,y;

      if ( q -> left != NULL )
        x = q->left->inorder_rank;
      else
        x = q->inorder_rank;
      if ( q -> right != NULL )
        y = q->right->inorder_rank;
      else
        y = q->inorder_rank;

      if ( p->inorder_rank > x && p->inorder_rank < y )
        printf("| ");
      else
        printf("  ");
    }

    char col = p->is_red ? 'R' : 'B';

    if ( p->parent == NULL )
      printf("%c %s\n", col, p->key );
    else
      printf("+-%c %s\n", col, p->key);
    recursive_show ( p->right );
  }
}

int rb_rank_check ( RBST_NODE * p )
{
  if ( p == NULL )
    return 0;
  else
  {
    if ( p->is_red && p->parent != NULL && p->parent->is_red )
      fprintf(stderr,"ERROR double red\n");
    int left = rb_rank_check ( p->left );
    int right = rb_rank_check ( p->right );
    if ( left != right )
      fprintf(stderr, "ERROR rank imbalance %d %d at %s\n",left, right,p->key);
    int rk = left;
    if ( left < right ) rk = right;

    if ( p->is_red )
      return rk;
    else
      return rk + 1;
  }
}


void show_rbst ( RBST * bst )
{
  printf("showing tree:\n");
  int inorder_rank = 0;


  int i;

  int inorder = 0;
  install_ranks ( &inorder, 0, bst->root );

  recursive_show ( bst->root );

  printf("Checking rank balance and double red....\n");
  int rbr = rb_rank_check ( bst->root );
  printf("Red-black rank of tree: %d\n", rbr);
}

int main()
{
  char buffer[200];  
  char op;
  char * x;

  RBST * bst;

  bst = make_rbst();

  while ( fgets(buffer,200,stdin) != NULL )
  {
    decr ( buffer );
    op = buffer[0];
    x = buffer+2;
    switch ( buffer[0] )
    {
      case '+':
	insert (x, bst );
      break;
      case '-':
	fprintf(stderr,"deleting %s\n", x);
	if ( find_in_bst ( x, bst ) == NULL )
	  fprintf(stderr,"WARNING:%s: not in bst\n", x);
	else
	  delete_from_bst ( x, bst );
      break;
      case '?':
	if ( all_blank ( x ) )
	  show_rbst ( bst );
	else if ( find_in_bst ( x, bst ) != NULL )
	  printf ( "%s is stored\n", x);
        else
	  printf ( "%s is not stored\n", x);
      break;
      case 'q':
	exit(0);
      break;

      default:
	fprintf(stderr, "unexpected line:%s:\n", buffer);
      break;
    }
  }
  show_rbst ( bst );
  return 0;
}