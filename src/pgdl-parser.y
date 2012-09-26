%{
#include <stdio.h>
#include <string.h>
#include <picviz.h>
#include <stdlib.h>    /* atoi() */
#include <unistd.h> /* exit() */

int axis_position;

extern int yylex(void);
extern void yylex_init(void);
extern void yylex_destroy(void);
extern void yyerror(char *);
extern void *yy_scan_string(const char *);
extern void yy_delete_buffer(void *);

void _pgdl_init_lexer(void);

static void picviz_key_value(char *key, char *value);

typedef enum section_t {
        EMPTY,
        HEADER,
        ENGINE,
        AXES,
        DATA
} section_t;

static section_t section_state = EMPTY;
int close_section_check = 0;
struct axis_t *axis;
float lines_values[PICVIZ_MAX_AXES]; /* store positions */
int max_axes = 0; /* to know if we should clean lines_values */
struct pcimage_t *image;
struct line_t *line;
int i = 0;
char *layer = NULL;
char *line_color;
char *line_penwidth;
char *line_layer = NULL;
char *axis_label = NULL;
char *axis_relative = NULL;
char *axis_print = NULL;
/* char *axis_current_type=""; */
/* If the line must be removed by filters,
 * we lock further axes reading and unlock on next line
 */
char lock = 0;

/* #define DEBUGSR 1 */

/* I know this is ugly,
 * I'll get back to it later
 */
#define FILE_MODE 0
#define LINE_MODE 1
char FILE_OR_LINE = FILE_MODE;

#define YY_ABORT return -1;
#define YYERROR_VERBOSE

%}
%token  TOK_SEMICOLON
%token  TOK_COMMA

%token  TOK_EQUAL
%token  <string> TOK_STRING
%token  <string> TOK_DQSTRING
%token  <string> TOK_WORD
%token  <string> TOK_DATATYPE
%token  <string> TOK_SECTION
%token  TOK_OPEN_SECTION
%token  TOK_CLOSE_SECTION

%token  <string> TOK_PROPERTY
%token  TOK_OPEN_PROP
%token  TOK_CLOSE_PROP

%union {
        char *string;
        unsigned long long int number;
}

%type <string> property
%type <string> properties
%type <string> value
%type <string> dataval

%start pcv

%%

pcv:    /* empty */
        | pcv section_new
        | pcv key_value_data
        | pcv type_var TOK_SEMICOLON
        | pcv section_end
        ;

section_new:    TOK_SECTION TOK_OPEN_SECTION
                {
#ifdef DEBUGSR /* Debug shifts-reduces*/
        printf("==> section_new:    TOK_SECTION(%s) TOK_OPEN_SECTION\n", $1);
#endif /* DEBUGSR */

                        if (close_section_check) {
                                yyerror("Opening new section, but the latest was not closed");
                                YY_ABORT;
                        }
                        if (!strncmp($1, "header", 6)) {
                                section_state = HEADER;
                        } else if (!strncmp($1, "engine", 6)) {
                                section_state = ENGINE;
                        } else if (!strncmp($1, "axes", 4)) {
                                section_state = AXES;
                        } else if (!strncmp($1, "data", 4)) {
                                section_state = DATA;
                        }

                        close_section_check++;
#ifdef DEBUG
                        printf("New section: %s\n", $1);
#endif
                        free($1);

#ifdef DEBUGSR
        printf("<== section_new:    TOK_SECTION TOK_OPEN_SECTION\n");
#endif /* DEBUGSR */
                }
		| /* Layers */
		TOK_SECTION TOK_WORD TOK_OPEN_SECTION
		{
#ifdef DEBUGSR
        printf("==> section_new:  TOK_SECTION TOK_WORD TOK_OPEN_SECTION\n");
#endif /* DEBUGSR */

			close_section_check++;
			if (!strncmp($1, "layer", 5)) {
				layer = $2;
			}
#ifdef DEBUGSR
        printf("<== section_new:  TOK_SECTION TOK_WORD TOK_OPEN_SECTION\n");
#endif /* DEBUGSR */
		}
                ;

property:
                TOK_PROPERTY TOK_EQUAL TOK_DQSTRING
                {
#ifdef DEBUGSR
        printf("==> property:       TOK_PROPERTY(%s) TOK_EQUAL TOK_DQSTRING(%s)\n", $1, $3);
#endif /* DEBUGSR */
                        switch (section_state) {
                                case DATA:
                                       if (!strcmp("color",$1)) {
                                                line_color = strdup($3);
                                        }
                                       if (!strcmp("penwidth",$1)) {
                                                line_penwidth = strdup($3);

                                        }
				       if (!strcmp("inlayer",$1)) {
					       line_layer = strdup($3);
				       }
                                        // Add line property
                                        break;
                                case AXES:
                                        if (!strcmp("relative",$1)) {
                                                axis_relative = strdup($3);
                                        }
                                        if (!strcmp("print",$1)) {
                                                axis_print = strdup($3);
                                        }
                                        if (!strcmp("label",$1))
                                        {
                                                axis_label = strdup($3);
                                                engine.__axis_label_exists = 1;
                                        }
                                        break;
                                default:
                                        break;
                        }

                        free($1);
                        free($3);
#ifdef DEBUGSR
        printf("<== property:       TOK_PROPERTY TOK_EQUAL TOK_DQSTRING\n");
#endif /* DEBUGSR */
                }
                ;

properties:
        property
        {
                $$ = $1;
        }
        |
        property TOK_COMMA properties
        {
                $$ = $1;
        }

type_var:       /* Data without any property */
		TOK_WORD TOK_WORD
                {
#ifdef DEBUGSR
        printf("==> type_var:   TOK_DATATYPE(%s) TOK_WORD(%s)\n", $1, $2);
#endif /* DEBUGSR */
		unsigned int counter = 0;
		char axis_exists = 0;
		char *datatype;
		char *axisname;
		char *endptr;
		int errno = 0;

		datatype = $1;
		axisname = $2;
		/* Check if we do not repeat an axis we already have */
		while (image->axesorder[counter]) {	
			if (!strcmp(axisname, image->axesorder[counter])) {
			//	fprintf(stderr, "We already know this axis(%s)\n", $2);
				if (max_axes >= PICVIZ_MAX_AXES) {
					yyerror("Axes overflow: it is not possible to have so much axes.");
				}
				image->axesorder[max_axes] = strdup(axisname);
				max_axes++;
				axis_exists = 1;
				break;
			}
			counter++;
		}
		if (!axis_exists) {
			/* axis_current_type = $1; */
			axis = picviz_axis_new();
			/* Check if we have a numeric value ( axes { 4096 axis1; })*/
			axis->numeric = strtoull(datatype, &endptr, 10);
			if (errno) {
				free(datatype);
				free($2);
				yyerror("Problem with numeric conversion.");
			}
			if (datatype != endptr) {
				/* We have a numeric */
/* 				fprintf(stderr, "The numeric value is %llu\n", axis->numeric); */
				picviz_axis_set_type(axis, DATATYPE_NUMERIC);
			} else {
				/* We have something like axes { (enum|...) axis1; } */
				picviz_axis_set_type_from_string(axis, datatype);
			}
			axis->name = strdup($2);
			picviz_image_axis_append(image, axis);
			if (max_axes >= PICVIZ_MAX_AXES) {
				free(datatype);
				free($2);
				yyerror("Axes overflow: it is not possible to have so much axes.");
			}

			image->axesorder[max_axes] = strdup($2);
			max_axes++;
		} /* if (!axis_exists) { */
		free(datatype);
		free(axisname);
#ifdef DEBUGSR
        printf("<== type_var:   TOK_DATATYPE TOK_WORD\n");
#endif /* DEBUGSR */
                } /* Data with properties */
                | TOK_WORD TOK_WORD TOK_OPEN_PROP properties TOK_CLOSE_PROP
                {
#ifdef DEBUGSR
        printf("==> type_var:    TOK_DATATYPE(%s) TOK_WORD(%s) TOK_OPEN_PROP property TOK_CLOSE_PROP\n", $1, $2);
#endif /* DEBUGSR */
		unsigned int counter = 0;
		char axis_exists = 0;
		char *datatype;
		char *axisname;
		char *endptr;
		int errno = 0;

		datatype = $1;
		axisname = $2;

		/* Check if we do not repeat an axis we already have */
		while (image->axesorder[counter]) {	
			if (!strcmp($2, image->axesorder[counter])) {
				//fprintf(stderr, "We already know this axis(%s)\n", $2);
			if (max_axes >= PICVIZ_MAX_AXES) {
				yyerror("Axes overflow: it is not possible to have so much axes.");
			}

				image->axesorder[max_axes] = strdup($2);
				max_axes++;
				free($1);
				free($2);
				axis_exists = 1;
			}
			counter++;
		}
		if (!axis_exists) {
			/* axis_current_type = $1; */
			axis = picviz_axis_new();
			axis->numeric = strtoull(datatype, &endptr, 10);
			if (errno) {
				free(datatype);
				free($2);
				yyerror("Problem with numeric conversion.");
			}
			if (datatype != endptr) {
				/* We have a numeric */
/* 				fprintf(stderr, "The numeric value is %llu\n", axis->numeric); */
				picviz_axis_set_type(axis, DATATYPE_NUMERIC);
			} else {
				/* We have something like axes { (enum|...) axis1; } */
				picviz_axis_set_type_from_string(axis, datatype);
			}
			axis->name = strdup($2);
/* 			picviz_axis_set_type_from_string(axis, $1); */
			if (axis_label) {
			  picviz_properties_set(axis->props, "label", axis_label);
			  free(axis_label);
			}
			if (axis_relative) {
				picviz_properties_set(axis->props,"relative", axis_relative);
			}
			if (axis_print) {
				picviz_properties_set(axis->props,"print", axis_print);
			}
			picviz_image_axis_append(image, axis);
			axis_relative = NULL;
			axis_print = NULL;
			if (max_axes >= PICVIZ_MAX_AXES) {
				yyerror("Axes overflow: it is not possible to have so much axes.");
			}

/* 			fprintf(stderr, "max=%d, value=%s\n", $2); */
/* 			max=168765424, value=(null) */

			image->axesorder[max_axes] = strdup($2);
			max_axes++;

			free($1);
			free($2);
		}
#ifdef DEBUG
                        printf("data with props\n");
#endif
#ifdef DEBUGSR
        printf("<== type_var:    TOK_DATATYPE TOK_WORD TOK_OPEN_PROP property TOK_CLOSE_PROP\n");
#endif /* DEBUGSR */
                }
                ;

/* l="foo",p="bar" */
value:  TOK_PROPERTY TOK_EQUAL TOK_DQSTRING /* I dont care of the chosen name matches a property name */
        {
#ifdef DEBUGSR
        printf("==> value:  TOK_PROPERTY TOK_EQUAL TOK_DQSTRING\n");
#endif /* DEBUGSR */
		picviz_key_value($1,$3);
#ifdef DEBUGSR
        printf("<== value:  TOK_PROPERTY TOK_EQUAL TOK_DQSTRING\n");
#endif /* DEBUGSR */
        }
        |
        TOK_WORD TOK_EQUAL TOK_DQSTRING
        {
		//printf("key='%s' value='%s'\n", $1, $3);
		picviz_key_value($1,$3);
        }

        ;

dataval: value
        {
                $$ = $1;
        }
        |
        value TOK_COMMA dataval
        {
                $$ = $1;
        }
        ;

key_value_data: dataval TOK_OPEN_PROP properties TOK_CLOSE_PROP TOK_SEMICOLON
                {
#ifdef DEBUGSR
        printf("==> key_value_data: dataval TOK_OPEN_PROP properties TOK_CLOSE_PROP TOK_SEMICOLON\n");
#endif /* DEBUGSR */
        if ( section_state == DATA ) {
		if (line) {
			char *color = picviz_color_named_to_hexstr(line_color);
			picviz_properties_set(line->props, "color", color);
			free(color);
			line_color = "black";
			picviz_properties_set(line->props, "penwidth", line_penwidth);
			line_penwidth = DEFAULT_PENWIDTH;
			if (line_layer) {
				line->layer = line_layer;
			} else {
				line->layer = layer;
			}
			line_layer = NULL;
			if (FILE_OR_LINE == FILE_MODE) {
				if (image->filter) {
					if (!lock) {
						picviz_image_line_append(image, line);
					} else {
						line->hidden = 1;
					}
				} else {
					picviz_image_line_append(image, line);
				}
			}
		}
        }
#ifdef DEBUGSR
        printf("<== key_value_data: dataval TOK_OPEN_PROP property TOK_CLOSE_PROP TOK_SEMICOLON\n");
#endif /* DEBUGSR */
                }
                |
                dataval TOK_SEMICOLON
                {
#ifdef DEBUGSR
        printf("==> dataval TOK_SEMICOLON\n");
#endif /* DEBUGSR */


                        if ( section_state == DATA ) {
				line->layer = layer;
                                if (FILE_OR_LINE == FILE_MODE) {
                                        if (!lock) {
                                                picviz_image_line_append(image, line);
                                        } else {
                                                line->hidden = 1;
                                        }
                                }
                        }
#ifdef DEBUGSR
        printf("<== dataval TOK_SEMICOLON\n");
#endif /* DEBUGSR */
                }
                ;

section_end:    TOK_CLOSE_SECTION
                {
                        close_section_check--;
			layer = NULL;
                }
                ;

%%

char *yyget_text(void);
int yyget_lineno (void);

void yyerror(char *str)
{
	picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "PCV file error: invalid token '%s' at line '%d': %s", yyget_text(), yyget_lineno(), str);
	exit(1);
}

PicvizImage *pcv_parse(char *filename, char *filterbuf)
{
        extern FILE *yyin;
	int ret;

        axis_position = 0;

	picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PARSER, "Parsing");

        image = picviz_image_new();
	if (!image) return NULL;
        if (filterbuf) {
                image->filter = picviz_filter_build(filterbuf);
        }
        yyin = fopen(filename,"r");	
        if ( ! yyin ) {
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Cannot open file '%s'", filename);
                return NULL;
        }
	/* _pgdl_reset_lexer(); */

        line_color = strdup("#000000");
        line_penwidth = strdup(DEFAULT_PENWIDTH);

	/* fseek(yyin, 0, SEEK_SET);  */
	yyrestart(yyin);
	ret  = yyparse ();
	switch (ret) {
	case 1:
		picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Invalid input!\n");
		fclose(yyin);
		return NULL;
		break;
	case 2:
		picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Not enough memory!\n");
		fclose(yyin);
		return NULL;
		break;		
	}

	fflush(yyin);
	fclose(yyin);
	yyin = NULL;

        picviz_render_image(image);

        return image;
}

PicvizLine *picviz_parse_line(char *string)
{
        void *state;
        int ret;

        FILE_OR_LINE = LINE_MODE;

        section_state = DATA;
        state = yy_scan_string(string);
        ret = yyparse();
        yy_delete_buffer(state);

        return line;
}

void picviz_key_value(char *key, char *value)
{
#ifdef DEBUGSR
        printf("==> picviz_key_value: TOK_WORD(%s) TOK_EQUAL TOK_DQSTRING(%s)\n", key, value);
#endif /* DEBUGSR */
        struct axisplot_t *axisplot;

	if ( section_state == HEADER ) {
		if ( ! strcmp(key, "height") ) {
			image->height = strtoull(value, NULL, 10);
		}
		if ( ! strcmp(key, "bgcolor") ) {
			image->bgcolor = picviz_color_named_to_hexstr(value);
		}
		if ( ! strcmp(key, "bgalpha") ) {
  		        char *endptr;
		        image->bgalpha = strtod(value, &endptr);
			if ( (endptr == value) || (image->bgalpha < 0) || (image->bgalpha > 1)) {
			  yyerror("Incorrect bgalpha value. It should be between 0.0 and 1.0");
			}
		}
		if ( ! strcmp(key, "hide-layers") ) {
			char *saveptr;
			char *token;

			token = strtok_r(value, ",", &saveptr);
			if (token) {
				picviz_hash_set(image->hidden_layers, token, (char *)"1", 1);
			}
			while ( token = strtok_r(NULL, ",", &saveptr) ) {
				picviz_hash_set(image->hidden_layers, token, (char *)"1", 1);
			}
		}
		if ( ! strcmp(key, "width") ) {
			image->width = atoi(value);
		}
		if ( ! strcmp(key, "header-height") ) {
			image->header_height = atoi(value);
		}
		if ( ! strcmp(key, "filter") ) {
			image->filter = picviz_filter_build(value);
		}
		if ( ! strcmp(key, "logo") ) {
			image->logo = strdup(value);
		}
		if ( ! strcmp(key, "logo.x") ) {
			image->logo_x = strtoull(value, NULL, 10);
		}
		if ( ! strcmp(key, "logo.y") ) {
			image->logo_y = strtoull(value, NULL, 10);
		}
		if ( ! strcmp(key, "font.color") ) {
		        image->font_color = strdup(value);
		}
		if ( ! strcmp(key, "font.size") ) {
 		        image->font_size = atoi(value);
		}
         } else if ( section_state == DATA ) {
                if ( ! axis_position ) {
                        line = picviz_line_new();
                        lock = 0;
                }

                if ( ! lock ) {
                        axisplot = picviz_axisplot_new();
                        axisplot->strval = strdup(value);
                        /* We first dump data into the structure, we render latter */
                        //axisplot->y = picviz_line_value_get_from_string(value); <- done in the rendering, since we need all values to be first set
#if 0
			picviz_debug(PICVIZ_DEBUG_WARNING, PICVIZ_AREA_PARSER, "For the value '%s' I have axis type '%d'", value, axis->type);
#endif
			picviz_axisplot_set_y(axis, axisplot, 
					      picviz_render_value(image, axis, value)
					      );
                        axisplot->axis_id = axis_position;
                        picviz_line_axis_append(line, key, axisplot);

#ifdef DEBUGDATA
                        printf("[%d]variable=%s,value=%s\n", axis_position, key, value);
#endif
                }
                axis_position++;

        } else if ( section_state == ENGINE ) {
                if ( ! strcmp(key, "axis_default_space") ) {
                        engine.axis_default_space = atoi(value);
                }
                if ( ! strcmp(key, "relative") ) {
                        engine.relative = atoi(value);
                }
                if ( ! strcmp(key, "string_algo") ) {
                        if ( ! strcmp(value, "basic")) {
                                engine.string_algo = 0;
                        } else {
                                engine.string_algo = atoi(value);
                        }
                }
        }

        free(value);
        free(key);
#ifdef DEBUGSR
        printf("<== picviz_key_value: TOK_WORD TOK_EQUAL TOK_DQSTRING\n");
#endif /* DEBUGSR */

}

#ifdef _UNIT_TEST_
/* gcc config-parser.lex.c config-parser.yacc.c -o config-parser -D_UNIT_TEST_ -ly -lfl */
int main(void)
{

        axis_position = 0;

        pcv_parse("test.pcv", NULL);

        picviz_image_debug_printall(image);

        return 0;
}
#endif

