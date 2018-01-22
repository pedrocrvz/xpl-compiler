%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: xpl_scanner.l,v 1.6 2017/05/17 15:44:33 ist180990 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

HEX         						0[xX][0-9a-fA-F]+
EXP									[eE][+\-]?{INTEGER}+
INTEGER								0|([1-9][0-9]*)
SPACE	    						[ \t\n\r]
DIGIT   							[0-9]

%x X_STRING X_IGNORE X_COMMENT

%%



"//".*$                				; /* ignore comments */

"/*"								yy_push_state(X_COMMENT);
<X_COMMENT>"/*"       				yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        				yy_pop_state();
<X_COMMENT>.|\n        				;  


">="                   				return tGE; 
"<="                   				return tLE; 
"=="                   				return tEQ; 
"!="                   				return tNE;
"!!" 								return tPRINTLN;

"while"                				return tWHILE; 
"if"                   				return tIF; 
"else"                 				return tELSE; 

"elseif"			   				return tELSEIF;
"int"				   				return tINTEGERDECL;
"real"                 				return tREALDECL;
"string"			   				return tSTRINGDECL;
"procedure"			   				return tPROCEDURE;
"public"			   				return tPUBLIC;
"use"     			   				return tUSE;
"sweep"				   				return tSWEEP;
"next"				   				return tNEXT;
"null"				   				return tNULL;
"stop"				   				return tSTOP;
"return"			   				return tRETURN;

[A-Za-z][A-Za-z0-9_]*  				yylval.s = new std::string(yytext); return tIDENTIFIER;


\"                     				yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\" 						yy_pop_state(); return tSTRING;
<X_STRING>\\\"						*yylval.s += yytext + 1;
<X_STRING>\\\\						*yylval.s += yytext + 1;
<X_STRING>\\t         				*yylval.s += std::string(1,9);
<X_STRING>\\n         				*yylval.s += std::string(1,10);
<X_STRING>\\r         				*yylval.s += std::string(1,13);

<X_STRING>\\0|\\00         			yy_push_state(X_IGNORE);
<X_STRING>\\([a-fA-F0-9]{1,2})		*yylval.s += std::string(1, strtol(yytext+1, nullptr, 16)); 
<X_STRING>.            				*yylval.s += yytext;
<X_STRING>\n           				yyerror("newline in string");

<X_IGNORE>\"		   				yy_pop_state(); return tSTRING;
<X_IGNORE>.|\n         				;


{INTEGER}			   				{ yylval.i = strtol(yytext, nullptr, 10); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tINTEGER; }
{HEX}				   				{ yylval.i = strtol(yytext, nullptr, 16); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tINTEGER; }

{DIGIT}+"."{DIGIT}*{EXP}?	   		{ yylval.d = strtod(yytext, nullptr); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tREAL; }
{DIGIT}*?"."{DIGIT}{EXP}?	   		{ yylval.d = strtod(yytext, nullptr); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tREAL; }
{DIGIT}{EXP}	   					{ yylval.d = strtod(yytext, nullptr); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tREAL; }

[-=<>()/#$@!&|~{}+*?,:;.%\[\]]      return *yytext;

{SPACE}+               				; /* ignore whitespace */

.                      				yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }
