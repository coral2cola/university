/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

static ScopeList globalScope = NULL;
static char * functionName;
static int isInScope = 0;


/* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

static void typeError(TreeNode* t, char* message)
{ 
  fprintf(listing,"Error: Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

static void invalidError(TreeNode* t, char* message)
{
  fprintf(listing,"Error: Invalid array Indexing at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

static void symbolError(TreeNode* t, char* message)
{ 
  fprintf(listing,"Error: Symbol error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

static void undeclaredError(TreeNode* t)
{ 
  if (t->kind.exp == CallK)
    fprintf(listing,"Error: Undeclared Function \"%s\" at line %d\n",t->attr.name,t->lineno);
  else if (t->kind.exp == IdK || t->kind.exp == ArrIdK)
    fprintf(listing,"Error: Undeclared Variable \"%s\" at line %d\n",t->attr.name,t->lineno);
  Error = TRUE;
}

static void redefinedError(TreeNode* t)
{ 
  if (t->kind.decl == VarK)
    fprintf(listing,"Error: Redefined Variable \"%s\" at line %d\n",t->attr.name,t->lineno);
  else if (t->kind.decl == ArrVarK)
    fprintf(listing,"Error: Redefined Variable \"%s\" at line %d\n",t->attr.arr.name ,t->lineno);
  else if (t->kind.decl == FunK)
    fprintf(listing,"Error: Redefined Function \"%s\" at line %d\n",t->attr.name,t->lineno);
  Error = TRUE;
}

static void funcDeclNotGlobal(TreeNode* t)
{ 
  fprintf(listing,"Error: Func declaration not on Global! at line %d (name : %s)\n",t->lineno,t->attr.name);
  Error = TRUE;
}

static void voidVarError(TreeNode* t, char* name)
{ 
  fprintf(listing,"Error: Variable Type cannot be Void at line %d (name : %s)\n",t->lineno,name);
  Error = TRUE;
}


/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode* t )
{ 
  switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { 
        case CompK:
            if(isInScope) isInScope = FALSE;
            else {
              ScopeList scope = make_scope(functionName);
              push_scope(scope);
              location++;
            }
            t->scope = curr_scope();
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { 
        case IdK:
        case ArrIdK:
        case CallK:
          if (st_lookup(t->attr.name) == -1) undeclaredError(t);
          else add_line(t->attr.name,t->lineno);
          break;

        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case FunK:
          functionName = t->attr.name;
          if (st_lookup_curr_scope(t->attr.name) >= 0) { 
            redefinedError(t);
            break;
          }
          if (curr_scope() != globalScope){ 
            funcDeclNotGlobal(t);
            break;
          }

          st_insert(functionName, t, t->lineno, add_location());   
          ScopeList tmp = make_scope(functionName);
          push_scope(tmp); 
          isInScope = TRUE;
          
          switch (t->child[0]->attr.type)
          { 
            case INT:
              t->type = Integer;
              break;
            case VOID:
            default:
              t->type = Void;
              break;
          }        
          break;
        case VarK:
        case ArrVarK:
          { char * name;            
            if (t->kind.decl == VarK)
            { name = t->attr.name;
              t->type = Integer;
            }
            else
            { name = t->attr.arr.name;
              t->type = ArrayInteger;
            }
            
            if (st_lookup(name) < 0) st_insert(name, t, t->lineno, add_location());    
            else redefinedError(t);
          }
          break;
        default:
          break;
      }
      break;
    case ParamK:
        if (t->child[0]->attr.type == VOID) break;
        
        if (st_lookup(t->attr.name) == -1){ 
          st_insert(t->attr.name, t, t->lineno, add_location());    

          if(t->kind.param == SingleParamK) t->type = Integer;
          else t->type = ArrayInteger;
        }
        break;
    default:
      break;
  }
}

static void popScope(TreeNode * t){ 
  if (t->nodekind == StmtK && t->kind.stmt == CompK)
    pop_scope();
}

static void insertIOFunction()
{
  TreeNode * function;
  TreeNode * type;
  TreeNode * parameter;
  TreeNode * parameter_child;
  TreeNode * comp;

  function = newDeclNode(FunK);
  function->type = Integer;
  function->lineno = 0;
  function->attr.name = "input";
  function->child[0] = type;
  function->child[1] = NULL;
  function->child[2] = comp; 

  type = newDeclNode(TypeK);
  type->attr.type = INT;

  comp = newStmtNode(CompK);
  comp->child[0] = NULL;
  comp->child[1] = NULL;

  st_insert("input", function, 0, add_location());

  function = newDeclNode(FunK);
  function->type = Void;
  function->lineno = 0;
  function->attr.name = "output";
  function->child[0] = type;
  function->child[1] = parameter;
  function->child[2] = comp; 

  type = newDeclNode(TypeK);
  type->attr.type = VOID;

  parameter = newParamNode(SingleParamK);
  parameter->attr.name = "arg";
  parameter->type = Integer;
  parameter->child[0] = parameter_child;

  parameter_child = newDeclNode(TypeK);
  parameter_child->attr.type = INT;
  
  comp = newStmtNode(CompK);
  comp->child[0] = NULL;
  comp->child[1] = NULL;

  st_insert("output",function,0,add_location());
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ 
  globalScope = make_scope("global");
  push_scope(globalScope);
  insertIOFunction();
  traverse(syntaxTree,insertNode,popScope);
  pop_scope();
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { 
        case IfK:
        case IfEK:
          if (t->child[0] == NULL)
            typeError(t,"expected expression");
          else if (t->child[0]->type == Void)
            typeError(t->child[0],"invalid if condition type");
          break;
        case WhileK:
          if (t->child[0] == NULL)
            typeError(t,"expected expression");
          else if (t->child[0]->type == Void)
            typeError(t->child[0],"invalid loop condition type");
          break;
        case ReturnK:
        { 
          TreeNode * ret = bucket_lookup(functionName)->node;
          if(ret->type == Void && t->child[0] != NULL)
            typeError(t,"invalid return type");
          else if ( ret->type == ArrayInteger &&  (t->child[0] == NULL || t->child[0]->type == Void || t->child[0]->type == Integer))
            typeError(t,"invalid return type");
          else if (ret->type == Integer && (t->child[0] == NULL || t->child[0]->type == Void || t->child[0]->type == ArrayInteger))
            typeError(t,"invalid return type");
          break;
        }
        case CompK:
          pop_scope();
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { 
        case AssignK:
          if (t->child[0]->type == Void || t->child[1]->type == Void)
            typeError(t->child[0],"invalid variable type");
          else if (t->child[0]->type == ArrayInteger && t->child[0]->child[0] == NULL)
            typeError(t->child[0],"invalid variable type");
          else if (t->child[1]->type == ArrayInteger && t->child[1]->child[0] == NULL)
            typeError(t->child[0],"invalid variable type");
          else
            t->type = t->child[0]->type;
          break;
        case ConstK:
          t->type = Integer;
          break;
        case OpK:
        {
          ExpType lType, rType;
          TokenType op;

          lType = t->child[0]->type;
          rType = t->child[1]->type;
          op = t->attr.op;

          if (lType == Void || rType == Void)
            typeError(t,"void variable cannot be operand");
          else if (lType != rType)
            typeError(t,"operands have different type");
          else
            t->type = Integer;
          break;
        }
        case CallK:
        {
          BucketList l = bucket_lookup(t->attr.name);
          TreeNode * funcNode = NULL;
          TreeNode * arg;
          TreeNode * param;

          if (l == NULL) break;
          funcNode = l->node;
          arg = t->child[0];
          param = funcNode->child[1];

          if (funcNode->kind.decl != FunK)
          { 
            typeError(t, "invalid expression");
            break;
          }

          while (arg != NULL){ 
            if (param == NULL || arg->type == Void)
            { 
              //fprintf(listing,"%d %d\n",arg->kind.type, arg->attr.type);
              typeError(arg, "invalid function call");
              break;
            }

            ExpType pType = param->type;
            ExpType aType = arg->type;   

            if(aType == ArrayInteger && arg->child[0] != NULL)
              aType = Integer;

            if (pType != aType) {   
              typeError(arg, "invalid function call");
              break;
            }
            else { 
              arg = arg->sibling;
              param = param->sibling;
            }
          }
          if (arg == NULL && param != NULL && param->child[0]->attr.type != VOID)
            typeError(t->child[0],"invalid function call");

          t->type = funcNode->type;
          break;
        }
        case IdK:
        case ArrIdK:
        {  
          BucketList l = bucket_lookup(t->attr.name);
          if (l == NULL) break;

          TreeNode * symbolNode = NULL;
          symbolNode = l->node;

          if (t->kind.exp == ArrIdK)
          { 
            if (symbolNode->nodekind == DeclK && symbolNode->kind.decl != ArrVarK)
              typeError(t, "invalid expression");
            else if (symbolNode->nodekind == ParamK && symbolNode->kind.param != ArrParamK)
              typeError(t, "invalid expression");
            else
              t->type = symbolNode->type;
          }
          else
            t->type = symbolNode->type;
          break;
        }
        default:
          break;
       }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case VarK:
        case ArrVarK:
          if (t->child[0]->attr.type == VOID)
          { 
            char * name;            
            if (t->kind.decl == VarK)
              name = t->attr.name;
            else
              name = t->attr.arr.name;
            voidVarError(t, name);
            break;
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

static void preCheckNode(TreeNode * t){ 
  switch (t->nodekind)
  { 
    case StmtK:
      switch (t->kind.stmt)
      { case CompK:
          push_scope(t->scope);
          break;
        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { 
        case FunK:
          functionName = t->attr.name;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree){ 
  push_scope(globalScope);
  traverse(syntaxTree,preCheckNode,checkNode);
  pop_scope();
}