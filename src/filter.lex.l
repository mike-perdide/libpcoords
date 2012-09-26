/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008 Sebastien Tricaud <toady@gscore.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include <string.h>

#include "filter-parser.h"

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

%}
%option noyywrap
%option 8bit prefix="pcvfilter"

 /* [value|plot|plotmin|plotmax|color] [>|<|=|] ["red"|"foobar"|123] [on axis N] */
 /* plot = exact value as plotted by picviz
  * plotmin = number of plots min to display */
 /* ie.:  plot > 100 on axis 3 */
 /* ie.:  plotmin 5 on axes */

FILTERTYPE (value|plot|color)
FREQTYPE    (freq)
SELECTAXIS  (on\ axis)
SELECTAXES  (on\ axes)
NUMFLOAT       [0-9]+\.[0-9]+
NUMBER      [0-9]+
PERCENT     [0-9]+%
SQSTRING    \'([^\\\']|\\.)*\'
DQSTRING    \"([^\\\"]|\\.)*\"

 /* Operators */
OPERATOR_AND (\&\&|\&|and|AND)
OPERATOR_OR  (\|\||\||or|OR)

OPERATOR_AND_WORD (and)
OPERATOR_OR_WORD (or)

 /* Relations */
RELATION      (\=|\!\=|\>|\<|\<\=|\>\=|is|not)
/*
RELATION_EQUAL   \=
RELATION_NOTEQUAL \!\=
RELATION_GREATER \>
RELATION_LESS    \<
RELATION_LESS_OR_EQUAL \<\=
RELATION_GREATER_OR_EQUAL \>\=
*/

VOID            [ \t]+

%%
\(                      { return '('; }
\)                      { return ')'; }

{FILTERTYPE} {
                pcvfilterlval.string = strdup(pcvfiltertext);
                return TOK_FILTERTYPE;
             }

{FREQTYPE}  {
                return TOK_FREQTYPE;
             }

{SELECTAXIS} {
                pcvfilterlval.string = strdup(pcvfiltertext);
                return TOK_SELECTAXIS;
             }

{SELECTAXES} {
                pcvfilterlval.string = strdup(pcvfiltertext);
                return TOK_SELECTAXES;
             }

{VOID}       {
                        /* We don't care */
             }

{DQSTRING}      {
                        pcvfilterlval.string = strdup(escape_str(pcvfiltertext));
                        return TOK_DQSTRING;
                }
{SQSTRING}      {
                        pcvfilterlval.string = strdup(escape_str(pcvfiltertext));
                        return TOK_SQSTRING;
                }

{NUMBER}      {
                        pcvfilterlval.string = strdup(pcvfiltertext);
                        return TOK_NUMBER;
              }

{NUMFLOAT}      {
                        pcvfilterlval.string = strdup(pcvfiltertext);
                        return TOK_NUMFLOAT;
              }


{PERCENT}     {
                        pcvfilterlval.string = strdup(pcvfiltertext);
                        return TOK_PERCENT;
              }

{RELATION} {
                pcvfilterlval.string = strdup(pcvfiltertext);
                return TOK_RELATION;
}

{OPERATOR_AND} { return TOK_OPERATOR_AND; };
{OPERATOR_OR} { return TOK_OPERATOR_OR; };

%%

/* remove unused functions */
typedef void (*dummy_function) ();
dummy_function picvizfilter_unused[] =
{
  (dummy_function) input, (dummy_function) yyunput
};


