/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211


// #define MAX_BUCKET 100

// #define MAX_SC 500

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec{ 
    int lineno;
    struct LineListRec * next;
} * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec { 
    char * name;
    TreeNode * node;  
    LineList lines;
    int memloc ;
    struct BucketListRec * next;
} * BucketList;

typedef struct ScopeListRec { 
    char * name;
    BucketList bucket[SIZE]; /* the hash table */
    int depth;
    int memidx;
    struct ScopeListRec * parent;
} * ScopeList;


/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only thse
 * first time, otherwise ignored
 */
void st_insert( char * name, TreeNode * node, int lineno, int loc);
int st_lookup ( char * name );
int st_lookup_curr_scope ( char * name );
BucketList bucket_lookup ( char * name );

void push_scope(ScopeList scope);
void pop_scope();
ScopeList curr_scope();
int add_location ();
void add_line ( char * name, int lineno );

ScopeList make_scope (char * name);

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);
void printFuncTable (FILE * listing);
void printFuncGlobalVar(FILE * listing);
void printFuncParamLocalVar (FILE * listing);

#endif
