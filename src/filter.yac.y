%name-prefix="pcvfilter"

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pcoords.h>

extern int pcvfilterlex(void);
extern void pcvfilterlex_init(void);
extern void pcvfilterlex_destroy(void);
extern void pcvfiltererror(char *);
extern void *pcvfilter_scan_string(const char *);
extern void pcvfilter_delete_buffer(void *);

static PicvizFilter *processed_filter;

#define YY_ABORT return -1;
#define YYERROR_VERBOSE
#define operator_or 1
#define operator_and 2


PicvizFilterRelation pcoords_filter_relation_get(char *str);
PicvizFilterType pcoords_filter_type_get(char *str);

%}

%union {
        int operator;
        char *string;
        int   number;
        double numfloat;
        struct pcoords_filter_criterion *criterion;
}

%token <string>TOK_FILTERTYPE
%token <string>TOK_SELECTAXIS
%token <string>TOK_SELECTAXES
%token <string>TOK_DQSTRING
%token <string>TOK_SQSTRING
%token <string>TOK_NUMBER
%token <string>TOK_NUMFLOAT
%token <string>TOK_PERCENT
%token <string>TOK_RELATION

%token  TOK_FREQTYPE

%token TOK_RELATION_EQUAL
%token TOK_RELATION_NOTEQUAL
%token TOK_RELATION_GREATER
%token TOK_RELATION_LESS
%token TOK_RELATION_LESS_OR_EQUAL
%token TOK_RELATION_GREATER_OR_EQUAL

%token TOK_OPERATOR_AND
%token TOK_OPERATOR_OR

%type <criterion> criteria
%type <criterion> criteria_base
%type <criterion> criterion
%type <criterion> criterion_numfloat
%type <criterion> criterion_number
%type <criterion> criterion_percent
%type <criterion> criterion_string
%type <operator> operator

%start input

%%


input:
        criteria {
                processed_filter = pcoords_filter_new();
                processed_filter->criterion = $1;
        }
;


criteria:
        criteria_base {
                $$ = $1;
        }

        | criteria operator criteria_base {
                if ( $2 == operator_or )
                        pcoords_filter_or_criterion($1, $3);
                else
                        pcoords_filter_and_criterion($1, $3);

                $$ = $1;
        }
;

criteria_base:
        criterion {
                $$ = $1;
        }

        | '(' criteria ')' {
                $$ = $2;
        }
;


criterion:
	        | criterion criterion_numfloat {
                        $$ = $2;
	        }
	        | criterion criterion_number {
                        $$ = $2;
                }
                | criterion criterion_percent {
                        $$ = $2;
                }
                | criterion criterion_string {
                        $$ = $2;
                }
;

criterion_numfloat:       TOK_FREQTYPE TOK_RELATION TOK_NUMFLOAT {

                        $$ = pcoords_filter_criterion_new();

                        $$->type = PF_FREQ_FILTER;
                        $$->relation = pcoords_filter_relation_get($2);

                        $$->options = PF_OPTIONS_NONE;

                        $$->value.numfloat = atof($3);

                        free($2);
                        free($3);
                }
                ;

criterion_number:       TOK_FILTERTYPE TOK_RELATION TOK_NUMBER TOK_SELECTAXIS TOK_NUMBER {


                        $$ = pcoords_filter_criterion_new();

                        $$->type = pcoords_filter_type_get($1);
                        $$->relation = pcoords_filter_relation_get($2);
                        if ( ! strcmp($5, "0")) {
                                fprintf(stderr, "ERROR: No NULL axis possible. If you think counting starts at 0, you are crazy. You may consider taking a programming career!\n");
                        }

                        $$->options = PF_OPTIONS_NONE;

                        $$->value.plot = atoi($3);
                        $$->axis = atoi($5) - 1;

                        free($1);
                        free($2);
                        free($3);
                        free($4);
                        free($5);
                }
                ;

criterion_percent:         TOK_FILTERTYPE TOK_RELATION TOK_PERCENT TOK_SELECTAXIS TOK_NUMBER {

                        char *percent_ptr;
                        size_t percent_size;

                        $$ = pcoords_filter_criterion_new();

                        $$->type = pcoords_filter_type_get($1);
                        $$->relation = pcoords_filter_relation_get($2);
                        if ( ! strcmp($5, "0")) {
                                fprintf(stderr, "ERROR: No NULL axis possible. If you think counting starts at 0, you are crazy. You may consider taking a programming career!\n");
                        }

                        $$->options = PF_OPTIONS_PLOTPERCENT;
                        percent_ptr = $3;
                        percent_size = strlen(percent_ptr);
                        percent_ptr[percent_size - 1] = '\0';

                        $$->value.plot = atoi(percent_ptr);
                        $$->axis = atoi($5) - 1;

                        free($1);
                        free($2);
                        free($3);
                        free($4);
                        free($5);
                }
                ;

criterion_string:          TOK_FILTERTYPE TOK_RELATION TOK_DQSTRING TOK_SELECTAXIS TOK_NUMBER {

                        if (!strcmp($1,"plot")) {
                                fprintf(stderr,"ERROR: plot does not match a string!\n");
                        }
                        if ( ! strcmp($5, "0")) {
                                fprintf(stderr, "ERROR: No NULL axis possible. If you think counting starts at 0, you are crazy. You may consider taking a programming career!\n");
                        }
                        $$ = pcoords_filter_criterion_new();

                        $$->type = pcoords_filter_type_get($1);
                        $$->relation = pcoords_filter_relation_get($2);
                        $$->options = PF_OPTIONS_NONE;
                        $$->value.data = strdup($3);
                        $$->axis = atoi($5) - 1;

                        free($1);
                        free($2);
                        free($3);
                        free($4);
                        free($5);
                }
                ;

operator:       TOK_OPERATOR_AND        { $$ = operator_and; }
                | TOK_OPERATOR_OR       { $$ = operator_or; }
;
%%

PicvizFilterType pcoords_filter_type_get(char *str)
{
        if (!strcmp(str,"value")) {
                return PF_VALUE_FILTER;
        }
        if (!strcmp(str,"color")) {
                return PF_COLOR_FILTER;
        }
        if (!strcmp(str,"plot")) {
                return PF_PLOT_FILTER;
        }
        if (!strcmp(str,"freq")) {
                return PF_FREQ_FILTER;
        }
        return PF_TYPE_ERROR;
}

PicvizFilterRelation pcoords_filter_relation_get(char *str)
{
        char c = str[0];

        switch(c) {
                case '=':
                        return PF_RELATION_EQUAL;
                case '!':
                        if (str[1] == '=') {
                                return PF_RELATION_NOTEQUAL;
                        }
                        return PF_RELATION_ERROR;
                case '>':
                        if (str[1] == '=') {
                                return PF_RELATION_GREATER_OR_EQUAL;
                        }
                        if (str[1] == '\0') {
                                return PF_RELATION_GREATER;
                        }
                        return PF_RELATION_ERROR;
                case '<':
                        if (str[1] == '=') {
                                return PF_RELATION_LESS_OR_EQUAL;
                        }
                        if (str[1] == '\0') {
                                return PF_RELATION_LESS;
                        }
                        return PF_RELATION_ERROR;
                case 'i':
                        if (str[1] == 's') {
                                return PF_RELATION_EQUAL;
                        }
                        return PF_RELATION_ERROR;
                case 'n':
                        if ((str[1] == 'o') && (str[2] == 't')) {
                                return PF_RELATION_NOTEQUAL;
                        }
                        return PF_RELATION_ERROR;
                default:
                        fprintf(stderr,"Error parsing relation\n");
        }
        return PF_RELATION_ERROR;
}

/* Should latter return a pcoords_filter_t */
PicvizFilter *pcoords_filter_build(char *filterbuf)
{
        void *state;

        if (!filterbuf) {
                fprintf(stderr, "No filter given!\n");
                return NULL;
        }

        processed_filter = NULL;

        state = pcvfilter_scan_string(filterbuf);
        pcvfilterparse();
        pcvfilter_delete_buffer(state);

        return processed_filter;
}


void pcvfiltererror(char *err)
{
        fprintf(stderr, "[W] Cannot apply filter (%s)\n", err);
}

#ifdef _UNIT_TEST_
int main(void)
{
        char *str = "show only plot > 100 on axis 3";
        void *state;

        state = pcvfilter__scan_string(str);
        pcvfilterparse();
        pcvfilter__delete_buffer(state);

        return 0;
}
#endif /* _UNIT_TEST_ */
