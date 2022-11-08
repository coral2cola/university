/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */

static int savedNumber;

static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */

static int yylex(void);
int yyerror(char * message);


%}

/* 11.25 shift reduce comflict 해결 */
%nonassoc NO_ELSE
%nonassoc ELSE

/* 11.25  modify code for token */
%token IF ELSE WHILE RETURN INT VOID
%token ID NUM
%token ASSIGN EQ NE LT LE GT GE PLUS MINUS TIMES OVER LPAREN RPAREN LBRACE RBRACE LCURLY RCURLY SEMI COMMA
%token ERROR

%% /* Grammar for TINY */
/* 11.25  modify code like BNF Grammar */

program             : declaration_list { savedTree = $1;} 
                    ;

declaration_list    : declaration_list declaration { 
                        YYSTYPE t = $1;
                        if (t != NULL) { 
                          while (t->sibling != NULL)t = t->sibling;
                          t->sibling = $2;
                          $$ = $1; 
                        }
                        else $$ = $2;
                      }
                    | declaration  { $$ = $1; }
                    ;

declaration         : var_declaration { $$ = $1; }
                    | fun_declaration { $$ = $1; }
                    ;

identifier          : ID { 
                        savedName = copyString(tokenString); 
                        savedLineNo = lineno;
                      }
                    ;

num                 : NUM { 
                        savedNumber = atoi(tokenString); 
                        savedLineNo = lineno;
                      }
                    ;

var_declaration	    : type_specifier identifier SEMI {
                        $$ = newDeclNode(VarK);
                        $$->child[0] = $1;
                        $$->attr.name = savedName;
                        $$->lineno = lineno;
                      }
			              | type_specifier identifier LBRACE num RBRACE SEMI {
                        $$ = newDeclNode(ArrVarK);
                        $$->child[0] = $1;
                        $$->attr.arr.name = savedName;
                        $$->attr.arr.size = savedNumber;
                        $$->lineno = lineno;
                      }
			              ;          

type_specifier		  : INT { 
                        $$ = newTypeNode(TypeNameK);
                        $$->attr.type = INT;
                      }
                    | VOID { 
                        $$ = newTypeNode(TypeNameK);
                        $$->attr.type = VOID;
                      }
                    ;

fun_declaration     : type_specifier identifier {
                        $$ = newDeclNode(FunK);
                        $$->attr.name = savedName;
                        $$->lineno = lineno;
                      }
                      LPAREN params RPAREN compound_stmt {
                        $$ = $3;
                        $$->child[0] = $1;
                        $$->child[1] = $5;
                        $$->child[2] = $7;
                      }
                    ;

params              : param_list { $$ = $1; }
                    | type_specifier {
                       $$ = newParamNode(SingleParamK);
                        $$->child[0] = $1;
                        $$->attr.name = copyString("(null)");
                      }
                    ;

param_list		      : param_list COMMA param {
                        YYSTYPE t = $1;
                        if(t != NULL) { 
                          while (t->sibling != NULL)  t = t->sibling;
                          t->sibling = $3;
                          $$ = $1; 
                        }
                        else $$ = $3;
                      }
                    | param { $$ = $1; }
                    ;

param               : type_specifier identifier {
                        $$ = newParamNode(SingleParamK);
                        $$->attr.name = savedName;
                        $$->child[0] = $1;
                      }
			              | type_specifier identifier LBRACE RBRACE {
                        $$ = newParamNode(ArrParamK);
                        $$->attr.name = savedName;
                        $$->child[0] = $1;
                      }
                    ;

compound_stmt		    : LCURLY local_declarations statement_list RCURLY {
                        $$ = newStmtNode(CompK);
                        $$->child[0] = $2;
                        $$->child[1] = $3;
                      }
			              ;
           
local_declarations	: local_declarations var_declaration {
                        YYSTYPE t = $1;
                        if(t != NULL) { 
                          while (t->sibling != NULL)  t = t->sibling;
                          t->sibling = $2;
                          $$ = $1; 
                        }
                        else $$ = $2;
                      }
                    | {$$ = NULL;}
                    ;

statement_list		  : statement_list statement {
                        YYSTYPE t = $1;
                        if(t != NULL) { 
                          while (t->sibling != NULL)  t = t->sibling;
                          t->sibling = $2;
                          $$ = $1; 
                        }
                        else $$ = $2;
                      }
                    | { $$ = NULL; }
                    ;

statement		        : expression_stmt { $$ = $1; }
                    | compound_stmt { $$ = $1; }
                    | selection_stmt { $$ = $1; }
                    | iteration_stmt { $$ = $1; }
                    | return_stmt { $$ = $1; }
                    ;

expression_stmt		  : expression SEMI { $$ = $1; }
                    | SEMI { $$ = NULL; }
                    ;

selection_stmt		  : IF LPAREN expression RPAREN statement %prec NO_ELSE {
                        $$ = newStmtNode(IfK);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                        $$->child[2] = NULL;
                      }
                    | IF LPAREN expression RPAREN statement ELSE statement {
                        $$ = newStmtNode(IfEK);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                        $$->child[2] = $7;
                      }
                    ;


iteration_stmt		  : WHILE LPAREN expression RPAREN statement {
                        $$ = newStmtNode(WhileK);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                      }
			              ;

return_stmt		      : RETURN SEMI {
                      $$ = newStmtNode(ReturnK);
                      $$->child[0] = NULL;
                    }
                    | RETURN expression SEMI {
                      $$ = newStmtNode(ReturnK);
                      $$->child[0] = $2;
                    }
                    ;


expression		      : var ASSIGN expression {
                        $$ = newExpNode(AssignK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                      }
                    | simple_expression { $$ = $1; }
                    ;

var			            : identifier {
                        $$ = newExpNode(IdK);
                        $$->attr.name = savedName;
                      }
                    | identifier {
                        $$ = newExpNode(ArrIdK);
                        $$->attr.name = savedName;
                      }
                    LBRACE expression RBRACE {
                        $$ = $2;
                        $$->child[0] = $4;
                      }
                    ;


simple_expression   : additive_expression LE additive_expression {
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = LE;
                      }
                    | additive_expression LT additive_expression { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = LT;
                      }
                    | additive_expression GT additive_expression {
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = GT;
                      }
                    | additive_expression GE additive_expression { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = GE;
                      }
                    | additive_expression EQ additive_expression {
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = EQ;
                      }
                    | additive_expression NE additive_expression { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = NE;
                      }
                    | additive_expression { $$ = $1; }
                    ;

additive_expression : additive_expression PLUS term { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = PLUS;
                      }
                    | additive_expression MINUS term {
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = MINUS;
                      }
                    | term { $$ = $1; }
                    ;

term                : term TIMES factor { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = TIMES;
                      }
                    | term OVER factor { 
                        $$ = newExpNode(OpK);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                        $$->attr.op = OVER;
                      }
                    | factor { $$ = $1; }
                    ;

factor              : LPAREN expression RPAREN { $$ = $2; }
                    | var { $$ = $1; }
                    | call { $$ = $1; }
                    | num { 
                        $$ = newExpNode(ConstK);
                        $$->attr.val = savedNumber;
                      }
                    ;

call                : identifier { 
                        $$ = newExpNode(CallK);
                        $$->attr.name = savedName;
                      }
                    LPAREN args RPAREN { 
                        $$ = $2;
                        $$->child[0] = $4;
                      }
                    ;

args                : arg_list { $$ = $1; }
                    | { $$ = NULL; }
                    ;
                    
arg_list            : arg_list COMMA expression { 
                        YYSTYPE t = $1;
                        if(t != NULL) { 
                          while (t->sibling != NULL) t = t->sibling;
                          t->sibling = $3;
                          $$ = $1; 
                        }
                        else $$ = $3;
                      }
                    | expression { $$ = $1; }
                    ;

%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

