/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"


/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

ScopeList scopes[SIZE];
int nScope = 0;

ScopeList stack[SIZE];
int nStack = 0;

int location[SIZE];

static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only thse
 * first time, otherwise ignored
 */
void st_insert( char * name, TreeNode * node, int lineno, int loc ){ 
  int h = hash(name);
  ScopeList currScope = curr_scope();
  BucketList l =  currScope->bucket[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;

  if (l == NULL) /* variable not yet in table */
  { 
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->node = node;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = currScope->bucket[h];
    currScope->bucket[h] = l;   
  }
}

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name )
{
  BucketList l = bucket_lookup(name);
  if(l == NULL) return -1;
  else return l->memloc;
}

BucketList lookup ( char * name, ScopeList scope )
{  
  int h = hash(name);
  ScopeList currScope = scope;
  BucketList l = currScope->bucket[h];
  while((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if(l != NULL) return l;
  return NULL;
}

void add_line( char * name, int lineno )
{
  ScopeList currScope = curr_scope();
  BucketList l = NULL;
  while(currScope != NULL)
  {
    l = lookup(name, currScope);
    if(l != NULL)
    {
      LineList ll = l->lines;
      while(ll->next != NULL)
        ll = ll->next;
      
      ll->next = (LineList) malloc(sizeof(struct LineListRec));
      ll->next->lineno = lineno;
      ll->next->next = NULL;
    }
    currScope = currScope->parent;
  }
}

int st_lookup_curr_scope ( char * name )
{ 
  ScopeList currScope = curr_scope();
  BucketList l = lookup(name, currScope);
  if(l != NULL) return l->memloc;
  return -1;
}

BucketList bucket_lookup (char * name)
{   
  int h = hash(name);
  ScopeList currScope = curr_scope();

  while(currScope != NULL){ 
    BucketList l = currScope->bucket[h];

    while((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;

    if(l != NULL) return l;

    currScope = currScope->parent;
  }
  return NULL;
}

ScopeList make_scope (char * name)
{
  ScopeList newScope;
  newScope = (ScopeList)malloc(sizeof(struct ScopeListRec));
  newScope->name = name;
  newScope->depth = nStack;
  newScope->parent = curr_scope();
  scopes[nScope++] = newScope;

  return newScope;
}

void push_scope(ScopeList scope)
{
  stack[nStack] = scope;
  location[nStack++] = 0;
}

void pop_scope()
{
  if(nStack>0)
  {
    nStack--;
  }
}

ScopeList curr_scope()
{
  if(nStack>0)
  {
    return stack[nStack-1];
  }
  return NULL;
}

int add_location ( void )
{ 
  return location[nStack - 1]++;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{ 
  int sc, bk;

  fprintf(listing,"< Symbol Table >\n");
  fprintf(listing,"Variable Name  Variable Type    Location      Scope Name    Line Numbers  \n");
  fprintf(listing,"-------------  -------------  -------------  -------------  ------------- \n");

  for(sc = 0; sc < nScope; sc++)
  {
    ScopeList currScope = scopes[sc];
    BucketList * l =  currScope->bucket;  

    for(bk =0; bk < SIZE; bk++)
    { 
      if (l[bk] != NULL){ 
    
        BucketList currBK = l[bk];

        while (currBK != NULL){
          fprintf(listing,"%-15s",currBK->name);
  
          TreeNode * node = currBK->node;
          switch (node->type)
          {
            case Void:
              fprintf(listing,"%-15s","Void");
              break;
            case Integer:
              fprintf(listing,"%-15s","Integer");
              break;
            default:
              break;
          }

          fprintf(listing,"%-15d",currBK->memloc);
          fprintf(listing,"%-15s",currScope->name);
          LineList linelist = currBK->lines;
          while(linelist != NULL){ 
            fprintf(listing,"%4d",linelist->lineno);
            linelist = linelist->next;
          }
          fprintf(listing,"\n");

          currBK = currBK->next;
        }
      }
    }
  }
} /* printSymTab */



void printFuncTable(FILE * listing){ 
  int sc,bk,sc_param,bk_param;

  fprintf(listing,"< Function Table >\n");
  fprintf(listing,"Function Name  Scope Name     Return Type    Parameter Name Parameter Type\n");
  fprintf(listing,"-------------  -------------  -------------  -------------  ------------- \n");

  for (sc = 0; sc < nScope; sc++){ 
  
    ScopeList currScope = scopes[sc];
    BucketList * l =  currScope->bucket;  

    for(bk =0; bk < SIZE; bk++){ 
      
      if (l[bk] != NULL){ 
    
        BucketList currBK = l[bk];
        TreeNode * node = currBK->node;

        while (currBK != NULL){
  
          switch (node->nodekind)
          {
            case DeclK:
              if (node->kind.decl == FunK) {

                fprintf(listing,"%-15s",currBK->name);
                fprintf(listing,"%-15s",currScope->name);
                switch (node->type)
                {
                  case Void:
                    fprintf(listing,"%-15s","Void");
                    break;
                  case Integer:
                    fprintf(listing,"%-15s","Integer");
                    break;
                  default:
                    break;
                }
                int no_param = 1;
                for(sc_param = 0; sc_param < nScope; sc_param++){
                  ScopeList paramSC = scopes[sc_param];
                  if (strcmp(paramSC->name, currBK->name) != 0) continue;
                  BucketList * param_l =  paramSC->bucket;  

                  for(bk_param = 0; bk_param < SIZE;bk_param++){

                    if (param_l[bk_param] != NULL){
                        BucketList paramBK = param_l[bk_param];
                        TreeNode * param_node = paramBK->node;

                      while (paramBK != NULL){
                        switch (param_node->nodekind)
                        {
                          case ParamK:
                            no_param = 0;
                            fprintf(listing,"\n");
                            fprintf(listing,"%-45s","");
                            fprintf(listing,"%-15s",paramBK->name);
                            switch (param_node->type)
                            {
                              case Integer:
                                fprintf(listing,"%-15s","Integer");
                                break;
                              case ArrayInteger:
                                fprintf(listing,"%-15s","ArrayInteger");
                                break;
                              default:
                                break;
                            }
                            break;
                          default:
                            break;
                        }
                        paramBK = paramBK->next;
                      }
                    }
                  }
                break;
              }
              if(no_param){
                fprintf(listing,"%-15s","");
                if (strcmp(currBK->name, "output") != 0) fprintf(listing,"%-14s","Void");
                else  fprintf(listing,"\n%-75s","Integer");
              }

              fprintf(listing,"\n");
            }
            break;
          default:
            break;
          }
          currBK = currBK->next;
        }
      }
    }
  }
}

void printFuncGlobalVar(FILE * listing){ 
  
  int sc,bk;

  fprintf(listing,"< Function and Global Variables >\n");
  fprintf(listing,"ID Name         ID Type        Data Type \n");
  fprintf(listing,"-------------  -------------  -------------\n");

  for(sc = 0; sc < nScope; sc++){

    ScopeList currScope = scopes[sc];

    if(strcmp(currScope->name, "global") != 0) continue;

    BucketList * l =  currScope->bucket;  

    for(bk =0; bk < SIZE; bk++){ 
      
      if (l[bk] != NULL){ 
    
        BucketList currBK = l[bk];
        while (currBK != NULL){
          TreeNode * node = currBK->node;
          switch (node->nodekind)
          { 
            case DeclK:
              fprintf(listing,"%-15s",currBK->name);
              switch (node->kind.decl)
              { 
                case FunK:
                  fprintf(listing,"%-15s","Function");
                  switch (node->type)
                  { 
                    case Void:
                      fprintf(listing,"%-15s","Void");
                      break;
                    case Integer:
                      fprintf(listing,"%-15s","Integer");
                      break;
                    default:
                      break;
                  }
                  break;
                case VarK:
                  fprintf(listing,"%-15s","Variable");
                  switch (node->type)
                  { 
                    case Void:
                      fprintf(listing,"%-15s","Void");
                      break;
                    case Integer:
                      fprintf(listing,"%-15s","Integer");
                      break;
                    default:
                      break;
                  }
                  break;
                case ArrVarK:
                  fprintf(listing,"%-15s","Variable");
                  fprintf(listing,"%-15s","ArrayInteger");
                  break;
                default:
                  break;
              }
              fprintf(listing,"\n");
              break;              
            default:
              break;
          }
          currBK = currBK->next;
        }
      }
    }
  }
}


void printFuncParamLocalVar (FILE * listing){ 
  
  int sc,bk;

  fprintf(listing,"< Function Parameter and Local Variables >\n");
  fprintf(listing,"Scope Name     Nested Level    ID Name        Data Type \n");
  fprintf(listing,"-------------  -------------  -------------  -------------\n");


  for(sc = 0; sc < nScope; sc++){

    ScopeList currScope = scopes[sc];

    if(strcmp(currScope->name, "global") == 0) continue;

    BucketList * l =  currScope->bucket;  

    int no_param = 1;

    for(bk =0; bk < SIZE; bk++){ 
      
      if (l[bk] != NULL){ 
    
        BucketList currBK = l[bk];
        while (currBK != NULL){
          TreeNode * node = currBK->node;
        
          switch (node->nodekind)
          { 
            case DeclK:

              no_param = 0;
              fprintf(listing,"%-15s",currScope->name);
              fprintf(listing,"%-15d",currScope->depth);

              switch (node->kind.decl)
              { 
                case VarK:
                  switch (node->type)
                  { 
                    case Void:
                      fprintf(listing,"%-15s",node->attr.name);
                      fprintf(listing,"%-15s","Void");
                      break;
                    case Integer:
                      fprintf(listing,"%-15s",node->attr.name);
                      fprintf(listing,"%-15s","Integer");
                      break;
                    default:
                      break;
                  }
                  break;
                case ArrVarK:
                  fprintf(listing,"%-15s",node->attr.arr.name);
                  fprintf(listing,"%-15s","ArrayInteger");
                  break;
                default:
                  break;
              }
              fprintf(listing,"\n");
              break;              
            case ParamK:
              no_param = 0;
              fprintf(listing,"%-15s",currScope->name);
              fprintf(listing,"%-15d",currScope->depth);
              switch (node->kind.param)
              { case ArrParamK:
                  fprintf(listing,"%-15s",node->attr.name);
                  fprintf(listing,"%-15s","ArrayInteger");
                  break;
                case SingleParamK:
                  fprintf(listing,"%-15s",node->attr.name);
                  fprintf(listing,"%-15s","Integer");
                  break;
                default:
                  break;
              }
              fprintf(listing,"\n");
              break;
            default:
              break;
          }
          currBK = currBK->next;
        }
      }
    }
    if (!no_param) fprintf(listing,"\n");
  }
}

