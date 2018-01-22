%{
// $Id: xpl_parser.y,v 1.13 2017/05/18 01:04:56 ist180990 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                  i;	/* integer value */
  double               d; 
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *basictype;
  xpl::block_node      *blocknode;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING
%token tWHILE tIF tPRINTLN tREAD
%token tELSEIF tPROCEDURE tPUBLIC tUSE tSWEEP tNEXT tNULL tSTOP tRETURN tINTEGERDECL tREALDECL tSTRINGDECL

%nonassoc tIFX
%nonassoc tELSE
%nonassoc tELSEIF

%right '='
%right '|' ']' ')'
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-' '('
%left '*' '/' '%' '&' '['
%nonassoc tUNARY
%nonassoc '?'

%type <node> stmt file decl func vardecl pvardecl elseif
%type <sequence> list decls exprs block_var funcarg funcargs
%type <expression> expr literal
%type <lvalue> lval
%type <basictype> type
%type <blocknode> block 
%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file       : decls                                { compiler->ast($1); }
           |                                      { compiler->ast(new cdk::nil_node(LINE)); }
           ;

decls      : decl                                 { $$ = new cdk::sequence_node(LINE, $1); }
           | decls decl                           { $$ = new cdk::sequence_node(LINE, $2, $1); }
           ;

decl       : vardecl ';'                          { $$ = $1; }
           | func                                 { $$ = $1; }
           ;


pvardecl   : type tIDENTIFIER                     { $$ = new xpl::variable_declaration_node(LINE, $1, $2, false, false, new xpl::null_node(LINE));}
           | type tIDENTIFIER '=' expr            { $$ = new xpl::variable_declaration_node(LINE, $1, $2, false, false, $4);}
           ;


vardecl    : tPUBLIC type tIDENTIFIER             { $$ = new xpl::variable_declaration_node(LINE, $2, $3, true, false, new xpl::null_node(LINE)); }
           | tPUBLIC type tIDENTIFIER '=' expr    { $$ = new xpl::variable_declaration_node(LINE, $2, $3, true, false, $5); }
           | tUSE type tIDENTIFIER                { $$ = new xpl::variable_declaration_node(LINE, $2, $3, false, true, new xpl::null_node(LINE)); }
           | tUSE type tIDENTIFIER  '=' expr      { $$ = new xpl::variable_declaration_node(LINE, $2, $3, false, true, $5); }
           | pvardecl                             { $$ = $1; } //VAR PRIVADA AO MODULO
           ;


funcargs   : funcarg                              { $$ = $1; }
           |                                      { $$ = new cdk::sequence_node(LINE); }
           ;

funcarg    : type tIDENTIFIER                     {$$ = new cdk::sequence_node(LINE,new xpl::variable_declaration_node(LINE,$1,$2,false,false, new xpl::null_node(LINE))); }
           | type tIDENTIFIER ',' funcargs            { $$ = new cdk::sequence_node(LINE,new xpl::variable_declaration_node(LINE,$1,$2,false,false, new xpl::null_node(LINE)),$4);}
           ;


func       : type tIDENTIFIER '(' funcargs ')' '=' literal      { $$ = new xpl::function_declaration_node(LINE, $1, $2, false, false, $4, $7); }
           | type tIDENTIFIER '(' funcargs ')'                  { $$ = new xpl::function_declaration_node(LINE, $1, $2, false, false, $4, new xpl::null_node(LINE)); }
           | tPROCEDURE tIDENTIFIER '(' funcargs ')'            { $$ = new xpl::function_declaration_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $2, false, false, $4, new xpl::null_node(LINE)); }

           | tPUBLIC type tIDENTIFIER '(' funcargs ')'              { $$ = new xpl::function_declaration_node(LINE, $2, $3, true, false, $5, new xpl::null_node(LINE)); }
           | tPUBLIC type tIDENTIFIER '(' funcargs ')' '=' literal  { $$ = new xpl::function_declaration_node(LINE, $2, $3, true, false, $5, $8); }
           | tPUBLIC tPROCEDURE tIDENTIFIER '(' funcargs ')'        { $$ = new xpl::function_declaration_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $3, true, false, $5, new xpl::null_node(LINE)); }

           | tUSE type tIDENTIFIER '(' funcargs ')'              { $$ = new xpl::function_declaration_node(LINE, $2, $3, true, false, $5, new xpl::null_node(LINE)); }
           | tUSE type tIDENTIFIER '(' funcargs ')' '=' literal  { $$ = new xpl::function_declaration_node(LINE, $2, $3, true, false, $5, $8); }
           | tUSE tPROCEDURE tIDENTIFIER '(' funcargs ')'         { $$ = new xpl::function_declaration_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $3, false, true, $5, new xpl::null_node(LINE)); }
           
           | type tIDENTIFIER '(' funcargs ')' block                        { $$ = new xpl::function_definition_node(LINE, $1, $2, false, false, $4, new xpl::null_node(LINE), $6); }
           | type tIDENTIFIER '(' funcargs ')' '=' literal block            { $$ = new xpl::function_definition_node(LINE, $1, $2, false, false, $4,  $7, $8); }
           | tPROCEDURE tIDENTIFIER '(' funcargs ')' block                  { $$ = new xpl::function_definition_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $2, false, false, $4, new xpl::null_node(LINE), $6); }

           | tPUBLIC type tIDENTIFIER '(' funcargs ')' block                { $$ = new xpl::function_definition_node(LINE, $2, $3, true, false, $5, new xpl::null_node(LINE), $7);}
           | tPUBLIC type tIDENTIFIER '(' funcargs ')' '=' literal block    { $$ = new xpl::function_definition_node(LINE, $2, $3, true, false, $5,  $8, $9); }
           | tPUBLIC tPROCEDURE tIDENTIFIER '(' funcargs ')' block          { $$ = new xpl::function_definition_node(LINE, new basic_type(0, basic_type::TYPE_VOID), $3, true, false, $5, new xpl::null_node(LINE), $7); }
           ;


type       : tINTEGERDECL                             { $$ = new basic_type(4, basic_type::TYPE_INT); }
           | tREALDECL                                { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
           | tSTRINGDECL                              { $$ = new basic_type(4, basic_type::TYPE_STRING); }
           | '['type']'                           { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
           ;

literal    : tINTEGER                             { $$ = new cdk::integer_node(LINE, $1); }
           | tREAL                                { $$ = new cdk::double_node(LINE, $1); }
           | string                              { $$ = new cdk::string_node(LINE, $1); }
           | tNULL                                { $$ = new xpl::null_node(LINE);}
           ;

block_var  : pvardecl ';'                         { $$ = new cdk::sequence_node(LINE, $1); }
           | block_var pvardecl ';'               { $$ = new cdk::sequence_node(LINE, $2, $1); }  
           ;

block      : '{' '}'                              { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
           | '{' block_var '}'                    { $$ = new xpl::block_node(LINE, $2, new cdk::sequence_node(LINE)); }
           | '{' list '}'                         { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), $2); }
           | '{' block_var list '}'               { $$ = new xpl::block_node(LINE, $2, $3); }
           ;  

list       : stmt	                                { $$ = new cdk::sequence_node(LINE, $1); }
	         | list stmt                            { $$ = new cdk::sequence_node(LINE, $2, $1); }
	         ;

stmt       : expr ';'                             { $$ = new xpl::evaluation_node(LINE, $1); }
       	   | expr '!'                             { $$ = new xpl::print_node(LINE, $1, false); }
           | expr tPRINTLN                        { $$ = new xpl::print_node(LINE, $1, true); }
           | tREAD lval ';'                       { $$ = new xpl::read_node(LINE); }
           | tWHILE '(' expr ')' stmt             { $$ = new xpl::while_node(LINE, $3, $5); }


           | tSWEEP '+' '(' lval ':' expr ':' expr ':' expr ')' stmt    { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, $10, $12); }
           | tSWEEP '+' '(' lval ':' expr ':' expr ')' stmt             { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, new cdk::integer_node(LINE,1), $10); }
           | tSWEEP '-' '(' lval ':' expr ':' expr ':' expr ')' stmt    { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, $10, $12); }
           | tSWEEP '-' '(' lval ':' expr ':' expr ')' stmt             { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, new cdk::integer_node(LINE,-1), $10); }


           | tIF '(' expr ')' stmt %prec tIFX               { $$ = new xpl::if_node(LINE, $3, $5); }
           | tIF '(' expr ')' stmt tELSE stmt               { $$ = new xpl::if_else_node(LINE, $3, $5, $7); }
           | tIF '(' expr ')' stmt elseif                   { $$ = new xpl::if_else_node(LINE, $3, $5, $6); }
           | '{' list '}'                                   { $$ = $2; }

           | tNEXT                                { $$ = new xpl::next_node(LINE);}
           | tSTOP                                { $$ = new xpl::stop_node(LINE);}
           | tRETURN                              { $$ = new xpl::return_node(LINE);}
           ;


elseif     : tELSEIF '(' expr ')' stmt                       { $$ = new xpl::if_node(LINE, $3, $5); }
           | tELSEIF '(' expr ')' stmt tELSE stmt            { $$ = new xpl::if_else_node(LINE, $3, $5, $7); }
           | tELSEIF '(' expr ')' stmt elseif                { $$ = new xpl::if_else_node(LINE, $3, $5, $6); }
           ;


expr       : literal                              { $$ = $1; }
           | '+' expr %prec tUNARY                { $$ = new xpl::identity_node(LINE, $2); }
           | '-' expr %prec tUNARY                { $$ = new cdk::neg_node(LINE, $2); }
           | '~' expr %prec tUNARY                { $$ = new cdk::not_node(LINE, $2); }
           | expr '+' expr	                      { $$ = new cdk::add_node(LINE, $1, $3); }
           | expr '-' expr	                      { $$ = new cdk::sub_node(LINE, $1, $3); }
           | expr '*' expr	                      { $$ = new cdk::mul_node(LINE, $1, $3); }
           | expr '/' expr	                      { $$ = new cdk::div_node(LINE, $1, $3); }
           | expr '%' expr	                      { $$ = new cdk::mod_node(LINE, $1, $3); }
           | expr '<' expr	                      { $$ = new cdk::lt_node(LINE, $1, $3); }
           | expr '>' expr	                      { $$ = new cdk::gt_node(LINE, $1, $3); }
           | expr '|' expr                        { $$ = new cdk::or_node(LINE, $1, $3); }
           | expr '&' expr                        { $$ = new cdk::and_node(LINE, $1, $3); }
           | '@'                                  { $$ = new xpl::read_node(LINE); }
           | expr tGE expr	                      { $$ = new cdk::or_node(LINE, $1, $3); }
           | expr tLE expr                        { $$ = new cdk::le_node(LINE, $1, $3); }
           | expr tNE expr	                      { $$ = new cdk::ne_node(LINE, $1, $3); }
           | expr tEQ expr	                      { $$ = new cdk::eq_node(LINE, $1, $3); }
           | tIDENTIFIER '(' exprs ')'             { $$ = new xpl::function_call_node(LINE, $1, new cdk::sequence_node(LINE,$3)); }
           | tIDENTIFIER '(' ')'                  { $$ = new xpl::function_call_node(LINE, $1, new cdk::sequence_node(LINE)); }
           | '(' expr ')'                         { $$ = $2; }
           | '[' expr']'                          { $$ = new xpl::allocation_node(LINE,$2);}
           | lval '?'                             { $$ = new xpl::adress_of_node(LINE, $1); }
           | lval                                 { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
           | lval '=' expr                        { $$ = new cdk::assignment_node(LINE, $1, $3); }
           ;

exprs      : expr                                 { $$ = new cdk::sequence_node(LINE, $1); }                             
           | exprs ',' expr                       { $$ = new cdk::sequence_node(LINE, $3, $1); }
           ;


string     : tSTRING                              { $$ = $1; }
           | string tSTRING                       { $$ = new std::string(*$1+*$2); delete $1; delete $2;  }
           ;

lval       : tIDENTIFIER                                { $$ = new cdk::identifier_node(LINE, $1);  }
           | '(' expr ')' '[' expr ']'                  { $$ = new xpl::index_node(LINE, $2, $5); } 
           | lval '[' expr ']'                          { $$ = new xpl::index_node(LINE, $1, $3); }
           | tIDENTIFIER '(' exprs ')' '[' expr ']'     { $$ = new xpl::index_node(LINE, new xpl::function_call_node(LINE, $1, $3), $6); }
           | '[' expr ']' '[' expr ']'                  { $$ = new xpl::index_node(LINE, new xpl::allocation_node(LINE, $2), $5); }
           ;

%%
