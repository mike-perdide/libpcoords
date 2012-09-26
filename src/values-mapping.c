/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <sebastien@honeynet.org>
 * Copyright (C) 2008 Philippe Saade <psaade@gmail.com>
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
 *
 * $Id: values-mapping.c 623 2009-05-27 21:13:21Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <math.h>

#if defined(__linux__) && ! defined(__USE_XOPEN)
# define __USE_XOPEN
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <picviz.h>

/* Holds the position for the enum type*/
//static picviz_properties_t *enumhashes[PICVIZ_MAX_AXES];
//static int enumcount[PICVIZ_MAX_AXES];

static int years_to_factor(char *buf, PcvHeight *out)
{
        time_t t;
        struct tm tm;
	char *ptr;

        memset(&tm, 0, sizeof(tm));

        ptr = strptime(buf, "%Y-%m-%d %H:%M:%S", &tm);
	t = mktime(&tm);
	*out = (PcvHeight) t;

        return 0;
}



static int timeline_to_factor(char *buf, PcvHeight *factor)
{
        struct tm tm;
	char *ptr;

        memset(&tm, 0, sizeof(tm));

        ptr = strptime(buf, "%H:%M:%S", &tm);
	*factor = tm.tm_sec + (tm.tm_min * 60) + (tm.tm_hour * 60 * 60);

        return 0;
}



static inline unsigned long long ntoh64(unsigned long long input)
{
        union {
                uint64_t val64;
                uint32_t val32[2];
        } combo_r, combo_w;

        combo_r.val64 = input;

        combo_w.val32[0] = ntohl(combo_r.val32[1]);
        combo_w.val32[1] = ntohl(combo_r.val32[0]);

        return combo_w.val64;
}


static inline unsigned long long factor_ip6(unsigned char *data)
{
        union {
                unsigned char data[16];
                unsigned long long val64[2];
        } combo_r, combo_w;

        memcpy(combo_r.data, data, sizeof(combo_r.data));
        combo_w.val64[0] = ntoh64(combo_r.val64[1]);
        combo_w.val64[1] = ntoh64(combo_r.val64[0]);

        return combo_w.val64[1];
}



static int ip_to_factor(char *buf, PcvHeight *factor)
{
        int ret;
        struct addrinfo hints, *res = NULL;

        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_NUMERICHOST;
        hints.ai_family = PF_UNSPEC;

        ret = getaddrinfo(buf, NULL, &hints, &res);
        if ( ret < 0 ) {
                fprintf(stderr, "error looking up '%s': %s.\n", buf, gai_strerror(ret));
                return -1;
        }

        if ( res->ai_family == AF_INET )
                *factor = (PcvHeight) ntohl(((struct sockaddr_in *)res->ai_addr)->sin_addr.s_addr);

        else if ( res->ai_family == AF_INET6 )
                *factor = (PcvHeight) factor_ip6(((struct sockaddr_in6 *)res->ai_addr)->sin6_addr.s6_addr);

        else {
                fprintf(stderr, "unsupported address family.\n");
                ret = -1;
        }

        freeaddrinfo(res);
        return ret;
}



/*
 * This function map a string given it's prefix (number of character
 * handled in the prefix is dependant on PcvHeight size: 8 on 64 bits,
 * 4 on 32 bits).
 *
 * The prefix is mapped to a PcvHeight value which increase with the
 * alphabetical weight of a given prefix character (left-most character
 * have the most weight).
 */
static int string_to_factor(char *buf, int string_algo, PcvHeight *factor)
{
	unsigned int i = 0, j;
	union {
		PcvHeight data;
		unsigned char dc[sizeof(PcvHeight) / sizeof(unsigned char)];
	} combo;

	*factor = 0;

	if (string_algo > 0) {
		combo.data = 0;
		j = (sizeof(PcvHeight) / sizeof(unsigned char));

		while ( buf[i] && i < sizeof(PcvHeight) / sizeof(unsigned char) )
			combo.dc[--j] = toupper(buf[i++]);

		*factor = combo.data;
	} else {
		while (*buf++) {
			char c = *buf;
			*factor += c;
		}
	}

	return 0;
}

/*
 * Scale an enumeration
 * Function written by Philippe Saade
 */
static double enum_to_factor(int enumber)
{
	double res = 0;
	int N = ilogb(enumber);
	int i;
	int x = enumber;

	if ( ! enumber) return -1;

	for (i = 0; i != N+1; i++) {
		if (x%2 == 0) {
			res = 2 * res;
		} else {
			res = 1+2*res;
		}
		x = x >> 1;
	}

	res = res / pow(2, N+1);

	return res;
}


/*
 * We first calculate values without caring of that axis min and max
 */
PcvHeight picviz_line_value_get_from_string_dummy(PicvizImage *image, PicvizAxis *axis, int string_algo, char *string)
{
	PcvHeight factor = 0;
	PcvString buf[10];
	PcvString pos;
	double enumfactor;

	if (!axis) return 0;

	/* Nothing to calculate, move on */
	if (string[0] == '\0') return 0;

	switch (axis->type) {
		case DATATYPE_EMPTY:
			break;
		case DATATYPE_FLOAT:
		case DATATYPE_INTEGER:
		case DATATYPE_SHORT:
		case DATATYPE_PORT:
  	        case DATATYPE_USEC:
	        case DATATYPE_NUMERIC:
			factor = (PcvHeight)strtoul(string, NULL, 10);
			break;
		case DATATYPE_YEARS:
			years_to_factor(string, &factor);
			break;
		case DATATYPE_STRING:
			string_to_factor(string, string_algo, &factor);
			break;
		case DATATYPE_TIMELINE:
			timeline_to_factor(string, &factor);
			break;
		case DATATYPE_IPV4:
			ip_to_factor(string, &factor);
			break;
		case DATATYPE_GOLD:
		case DATATYPE_CHAR:
   	        case DATATYPE_B12:
			factor = (PcvHeight)atoi(string);
			break;
		case DATATYPE_ENUM:
			pos = picviz_properties_get(axis->enum_hash, string);
			if (pos) {
				//fprintf(stderr,"got it (id=%llu,pos=%s)!\n",axis->id, pos);
				factor = (PcvHeight) (enum_to_factor(atoi(pos)) * image->height);
			} else {
				sprintf((char *)buf, "%d", axis->enum_count);
				//fprintf(stderr,"set it (id=%llu,eenumcount=%d)!\n",axis->id, axis->enum_count);
				picviz_properties_set(axis->enum_hash, string, buf);
				enumfactor = enum_to_factor(axis->enum_count);
				factor = (PcvHeight) (enumfactor * image->height);
				axis->enum_count++;
			}
#if 0			
			if ( ! enumcount[axis->id] ) {
				/* Initialize */
				picviz_properties_new(&enumhashes[axis->id]);
				enumcount[axis->id] = 1;
				/* FIXME: properties must also be able to receive an integer */
				sprintf((char *)buf, "%d", enumcount[axis->id]);
				picviz_properties_set(enumhashes[axis->id], string, buf);
				enumfactor = enum_to_factor(enumcount[axis->id]);
				factor = (PcvHeight) (enumfactor * image->height);
				enumcount[axis->id]++;
			} else {
				pos = picviz_properties_get(enumhashes[axis->id], string);
				if (pos) {
					factor = (PcvHeight) (enum_to_factor(atoi(pos)) * image->height);
				} else {
					sprintf((char *)buf, "%d", enumcount[axis->id]);
					picviz_properties_set(enumhashes[axis->id], string, (char *)buf);
					enumfactor = enum_to_factor(enumcount[axis->id]);
					factor = (PcvHeight) (enumfactor * image->height);
					enumcount[axis->id]++;
				}
			}
#endif
			break;
		case DATATYPE_LN:
			factor = (PcvHeight)strtoul(string, NULL, 10);
			break;
		default:
			fprintf(stderr, "Cannot map value from choosen variable\n");
	}

	return factor;
}

PcvHeight picviz_line_value_get_with_minmax(PicvizImage *image, PicvizAxis *axis, char *string, PcvHeight min _U_, PcvHeight max)
{
	PcvHeight scale;

	switch (axis->type) {
		case DATATYPE_LN:
			scale = (PcvHeight) ((double)log(atoi(string) + 1) / (log(max+1)) * (PcvHeight)image->height);
			break;
		default:
			scale = 0;
			break;
	}

	return scale;

}

PcvHeight picviz_values_mapping_get_from_y(PicvizImage *image, PcvHeight max_val, PcvHeight y)
{
        float value;

	/* XXX: Hack, should be removed! (test with test-var-string2.pcv and with/without pcv -l) */
	if (y > max_val) max_val = y;

        /*
         * Avoid division by 0.
         */
        if ( max_val == 0 )
                return 0;

        value = (float)(image->height - image->header_height) / max_val;
        value *= y;

        return (PcvHeight)value;

}

#ifdef _UNIT_TEST_
#include <stdio.h>

void run_test(char *val)
{
        //printf("Value=%s -> Int=%u\n", val, ipstr2i(val));
}

int main(void)
{
        PcvHeight f;
        int i;
	char *buf;

        f = picviz_line_value_get_from_string_dummy(DATATYPE_STRING, STRING_TYPE_MAX);

        run_test("192.168.0.42");
        run_test("127.0.0.1");
        run_test("0.0.0.0");
        run_test("255.255.255.255");

	buf = "2038-01-18";
	years_to_factor(buf, &f);
	printf("factor=%d\n", f);

        return 0;
}
#endif

