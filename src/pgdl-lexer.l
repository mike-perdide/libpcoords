/*
 * Written by Sebastien Tricaud (C) 2008-2009 
 * Part of the PicViz project
 */

%x incl

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <picviz.h>
#include "pgdl-parser.h"

static char *filename;
static char *realfile;

void yyerror (char *s);
#if defined(__FreeBSD__) || defined(__APPLE__)
char* strndup(const char* string, size_t n)
{
	char* copy_string = 0;

	if(0 == string || 0 == n)
		return 0;

	copy_string = (char*) malloc(n + 1);
	if(0 == copy_string)
		return 0;

	memcpy(copy_string, string, n);
	*(copy_string + n) = '\0';

	return copy_string;
}
#else
char * strndup (const char *s, size_t n);
#endif

#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE includes[MAX_INCLUDE_DEPTH];
char *filenames[MAX_INCLUDE_DEPTH];
int includes_index = 0;

extern int axis_position;

static char *escape_str(char *str)
{
        size_t w = 0, i = 0;
        int escape_next = 0;

        for ( i = 1; str[i]; i++ ) {

                if ( ! escape_next && str[i] == '\\' ) {
                        escape_next = 1;
                        continue;
                }

                str[w++] = str[i];
                escape_next = 0;
        }

        str[w - 1] = '\0';
        return str;
}

#ifdef __FreeBSD__
void yyset_lineno(int line_number) {
	yylineno = line_number;
}

int yyget_lineno() {
	return yylineno;
}

char * yyget_text() {
	return yytext;
}
#endif

void _pgdl_reset_lexer(void)
{
        ;; BEGIN(0);
        ;; YY_FLUSH_BUFFER;
	;; yyset_lineno(1);
}

%}

%option noyywrap

SECTION		(header|engine|axes|data|layer)
OPEN_SECTION    \{
CLOSE_SECTION   \}
OPEN_PROP	\[
CLOSE_PROP	\]
PROPERTY        (label|color|penwidth|relative|print|inlayer)
SQSTRING	\'([^\\\']|\\.)*\'
DQSTRING	\"([^\\\"]|\\.)*\"
STRING		(\"|\')([^\\(\"|\')]|\\.)*(\"|\')
COMMENT		^#.*\n
WORD            ([a-zA-Z0-9_\-]+(\(\-?[0-9\*]+\))?\.?)+ 
SEMICOLON	\;
COMMA		\,
EQUAL		\=
VOID		[ \t]+
NEWLINE         \n
INCLUDE		@include

%%

{INCLUDE}	{ BEGIN(incl); }

{PROPERTY}      { yylval.string = strdup(yytext);
                  return TOK_PROPERTY; }
{OPEN_PROP}     { return TOK_OPEN_PROP; }
{CLOSE_PROP}     { return TOK_CLOSE_PROP; }

{SEMICOLON}	{
			axis_position = 0;
			return TOK_SEMICOLON;
		}
{COMMA}		{ return TOK_COMMA; }

{SECTION}	{	yylval.string = strdup(yytext);
			return TOK_SECTION;
		}
{OPEN_SECTION}  {	return TOK_OPEN_SECTION;  }
{CLOSE_SECTION} {	return TOK_CLOSE_SECTION; }

{DQSTRING}      {
                        yylval.string = strdup(escape_str(yytext));
                        return TOK_DQSTRING;
                }

{WORD}		{	yylval.string = strdup(yytext);
			return TOK_WORD;
		}

{EQUAL}		{	return TOK_EQUAL; }

{STRING}	{
			yylval.string = strdup(escape_str(yytext));
			return TOK_WORD;
		}
{COMMENT}	{
			/* We don't care */
		}

{VOID}		{
			/* We don't care */
		}

{NEWLINE}       {
			/* We don't care (; is the separator)
			 * so we just inscrease the line number
			 */
                       yyset_lineno(yyget_lineno()+1);
                }

<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+	{ /* got the include file name */
		  if (includes_index >= MAX_INCLUDE_DEPTH) {
			  yyerror("Includes nested too deeply");
			  exit(1);
		  }

		filename = escape_str(strndup(yytext, yyleng));


		filenames[includes_index] = filename;
		includes[includes_index++] = YY_CURRENT_BUFFER;

		realfile = strdup(filename);

		yyin = fopen(realfile, "r");
		if (!yyin) {
			printf("Can not open %s\n", realfile);
			exit(1);
		}
		free(realfile);
		free(filename);

		yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

                  BEGIN(INITIAL);
		}
<<EOF>>		{
		  if (--includes_index < 0) {
                          YY_NEW_FILE;
			  yyterminate();
		  } else {
			  free(filenames[includes_index + 1]);
			  filename = filenames[includes_index];
			  yy_delete_buffer(YY_CURRENT_BUFFER);
			  yy_switch_to_buffer(includes[includes_index]);
		  }
		}

%%

/* remove unused functions */
typedef void (*dummy_function) ();
dummy_function picvizparser_unused[] =
{
  (dummy_function) input, (dummy_function) yyunput
};
