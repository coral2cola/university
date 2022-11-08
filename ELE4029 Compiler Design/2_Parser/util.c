/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

static int var_array = 0;
static int par_array = 0;

static int arr_size = 0;
static int check_type = 0;

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { 

    case IF:
    case ELSE:
    case WHILE:
    case RETURN:
    case INT:
    case VOID: fprintf(listing,"reserved word: %s\n",tokenString); break;

    case ASSIGN: fprintf(listing,"=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case NE: fprintf(listing,"!=\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case LE: fprintf(listing,"<=\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GE: fprintf(listing,">=\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LBRACE: fprintf(listing,"[\n"); break;
    case RBRACE: fprintf(listing,"]\n"); break;
    case LCURLY: fprintf(listing,"{\n"); break;
    case RCURLY: fprintf(listing,"}\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
    case COMMA: fprintf(listing,",\n"); break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

TreeNode * newDeclNode(DeclKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
  }
  return t;
}

TreeNode * newParamNode(ParamKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ParamK;
    t->kind.param = kind;
    t->lineno = lineno;
  }
  return t;
}


/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    if (tree->nodekind==StmtK)
    { printSpaces();
      switch (tree->kind.stmt) {
        case IfNEK:
          fprintf(listing,"If Statement:\n");
          break;
        case IfEK:
          fprintf(listing,"If-Else Statement:\n");
          break;
        case WhileK:
          fprintf(listing,"While Statement:\n");
          break;
        case ReturnK:
          fprintf(listing,"Return Statement:\n");
          break;
        case CompK:
          fprintf(listing, "Compound Statement:\n");
          break;
        default:
          fprintf(listing,"Unknown StmtNode kind\n");
          break;
      }
    }

    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case AssignK:
          printSpaces();
          fprintf(listing,"Assign:\n");
          break;
        case OpK:
          printSpaces();
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          if(check_type == 1)
          {
            if(var_array == 1)
            {
              if(arr_size != 0)
              {
                printSpaces();
                fprintf(listing,"Const: %d\n",arr_size);
                arr_size = 0;
                if(var_array == 1) var_array = 0;
              }
            }
            check_type = 0;
          }
          else
          {
            printSpaces();
            fprintf(listing,"Const: %d\n",tree->attr.val);
          }
          break;
        case CallK:
          printSpaces();
          fprintf(listing,"Call: function name = %s\n",tree->attr.name);
          break;
        case IdK:
          printSpaces();
          fprintf(listing,"Variable: name = %s\n",tree->attr.name);
          break;
        case ArrIdK:
          printSpaces();
          fprintf(listing,"Variable: name = %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    
    else if (tree->nodekind==DeclK)
    { switch (tree->kind.decl) {
        case VarK:
          printSpaces();
          fprintf(listing,"Variable Declaration: name = %s, ",tree->attr.name);
          break;
        case ArrVarK:
          printSpaces();
          fprintf(listing,"Variable Declaration: name = %s, ", tree->attr.arr.name);
          arr_size = tree->attr.arr.size;
          var_array = 1;
          break;
        case FunK:
          printSpaces();
          fprintf(listing,"Function Declaration: name = %s, return ", tree->attr.name);
          break;
        case VoidK:
          if(tree->attr.check_void != 1)
          {
            if(par_array == 1 || var_array == 1)
            {
              fprintf(listing,"type = void[]\n");
              if(par_array == 1) par_array = 0;
              //if(var_array == 1) var_array = 0;
            }
            else
              fprintf(listing,"type = void\n");
          }
          else
          {
            printSpaces();
            fprintf(listing,"Void Parameter\n");
          }
          check_type = 1;
          break;
        case IntK:
          if(par_array == 1 || var_array == 1)
          {
            fprintf(listing,"type = int[]\n");
              if(par_array == 1) par_array = 0;
              //if(var_array == 1) var_array = 0;
          }
          else
            fprintf(listing,"type = int\n");
          check_type = 1;
          break;
        default:
          fprintf(listing,"Unknown DeclNode kind\n");
          break;
      }
    }

    else if (tree->nodekind==ParamK)
    { printSpaces();
      switch (tree->kind.param) {
        case ArrParamK:
          fprintf(listing,"Parameter: name = %s, ", tree->attr.name);
          par_array = 1;
          break;
        case NArrParamK:
            fprintf(listing,"Parameter: name = %s, ",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ParamNode kind\n");
          break;
      }
    }
    
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }

  UNINDENT;
}
