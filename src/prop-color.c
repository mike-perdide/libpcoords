/*
 * props/color.c: Manage [color="red"] property
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <picviz.h>

char *picviz_color_named_to_hexstr(char *color)
{
	int MAX_COLORS = 13;
	char *pcolors[] = {"white","black", "red", "green", "blue", "yellow", "grey", "turquoise", "pink", "orange", "darkblue", "darkgreen", "darkred", "brown"};
	char *chart[] = {"#FFFFFF","#000000", "#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#C0C0C0", "#00FFFF", "#FF0099", "#FF9900", "#3333CC", "#339933", "#990000", "#8B6969"};
	int i=0;

	/* color = #xxxxxx, we return #XXXXXX */
        if (color[0] == '#')
            return picviz_string_up(color);

	/* color = (0.1, 1, 0.5) */
	if (color[0] == '(') {
		char *tok;
		char *buf;
		char strcolor[7];
		double r;
		double g;
		double b;

		*color++; /* to skip (*/

		buf = strtok_r(color, ",", &tok);
		r = atof(buf);
		buf = strtok_r(NULL, ",", &tok);
		g = atof(buf);
		/* 0.3) -> 0.3 anyway */
		buf = strtok_r(NULL, ",", &tok);
		b = atof(buf);

		r = r * 255;
		g = g * 255;
		b = b * 255;
		sprintf(strcolor, "#%.2X%.2X%.2X", (int)r, (int)g, (int)b);
		return strdup(strcolor);
	}

	/* color = "red", "blue", ... */
        for (i=0;i<=MAX_COLORS;i++) {
#ifdef DEBUG_PROPERTIES
                printf("color=[%s];pcolors[i]=[%s], value=[%s]\n", color, pcolors[i], chart[i]);
#endif
                if (!strcmp(color, pcolors[i])) {
                        return strdup(chart[i]);
                }
        }

        /* color = ??
	 * -> we can't find the color, we shout and return */
	picviz_debug(PICVIZ_DEBUG_WARNING, PICVIZ_AREA_CORE,
			"Unknown color: '%s'", color);

	return strdup("#000000"); /* We return black as default */
}


float picviz_color_extract_r(char *s)
{
        int i;
        char p[3];

        p[0] = s[1];
        p[1] = s[2];
        p[2] = '\0';

        i = strtol(p, NULL, 16);

        return (float) i / 255;
}

float picviz_color_extract_g(char *s)
{
        int i;
        char p[3];

        p[0] = s[3];
        p[1] = s[4];
        p[2] = '\0';

        i = strtol(p, NULL, 16);

        return (float) i / 255;
}

float picviz_color_extract_b(char *s)
{
        char p[3];
        int i;

        p[0] = s[5];
        p[1] = s[6];
        p[2] = '\0';

        i = strtol(p, NULL, 16);

        return (float) i / 255;
}

#ifdef _UNIT_TEST_
int main(void)
{
        char *ptr = "FF";
        int val;

        printf("White=%s\n", picviz_color_named_to_hexstr("white"));
        printf("Red=%s\n", picviz_color_named_to_hexstr("red"));
        printf("Red value of #FF0000 = %f\n", picviz_color_extract_r("#FF0000"));
        printf("Red value of #eA0000 = %f\n", picviz_color_extract_r("#eA0000"));
        printf("Green value of #eABF00 = %f\n", picviz_color_extract_g("#eABF00"));
        printf("#0a6106 r=%f,g=%f,b=%f\n", picviz_color_extract_r("#0a6106"), picviz_color_extract_g("#0a6106"), picviz_color_extract_b("#0a6106"));

}
#endif
