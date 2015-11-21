/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse pcap_parse
#define yylex   pcap_lex
#define yyerror pcap_error
#define yylval  pcap_lval
#define yychar  pcap_char
#define yydebug pcap_debug
#define yynerrs pcap_nerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DST = 258,
     SRC = 259,
     HOST = 260,
     GATEWAY = 261,
     NET = 262,
     NETMASK = 263,
     PORT = 264,
     PORTRANGE = 265,
     LESS = 266,
     GREATER = 267,
     PROTO = 268,
     PROTOCHAIN = 269,
     CBYTE = 270,
     ARP = 271,
     RARP = 272,
     IP = 273,
     SCTP = 274,
     TCP = 275,
     UDP = 276,
     ICMP = 277,
     IGMP = 278,
     IGRP = 279,
     PIM = 280,
     VRRP = 281,
     CARP = 282,
     ATALK = 283,
     AARP = 284,
     DECNET = 285,
     LAT = 286,
     SCA = 287,
     MOPRC = 288,
     MOPDL = 289,
     TK_BROADCAST = 290,
     TK_MULTICAST = 291,
     NUM = 292,
     INBOUND = 293,
     OUTBOUND = 294,
     PF_IFNAME = 295,
     PF_RSET = 296,
     PF_RNR = 297,
     PF_SRNR = 298,
     PF_REASON = 299,
     PF_ACTION = 300,
     TYPE = 301,
     SUBTYPE = 302,
     DIR = 303,
     ADDR1 = 304,
     ADDR2 = 305,
     ADDR3 = 306,
     ADDR4 = 307,
     RA = 308,
     TA = 309,
     LINK = 310,
     GEQ = 311,
     LEQ = 312,
     NEQ = 313,
     ID = 314,
     EID = 315,
     HID = 316,
     HID6 = 317,
     AID = 318,
     LSH = 319,
     RSH = 320,
     LEN = 321,
     IPV6 = 322,
     ICMPV6 = 323,
     AH = 324,
     ESP = 325,
     VLAN = 326,
     MPLS = 327,
     PPPOED = 328,
     PPPOES = 329,
     GENEVE = 330,
     ISO = 331,
     ESIS = 332,
     CLNP = 333,
     ISIS = 334,
     L1 = 335,
     L2 = 336,
     IIH = 337,
     LSP = 338,
     SNP = 339,
     CSNP = 340,
     PSNP = 341,
     STP = 342,
     IPX = 343,
     NETBEUI = 344,
     LANE = 345,
     LLC = 346,
     METAC = 347,
     BCC = 348,
     SC = 349,
     ILMIC = 350,
     OAMF4EC = 351,
     OAMF4SC = 352,
     OAM = 353,
     OAMF4 = 354,
     CONNECTMSG = 355,
     METACONNECT = 356,
     VPI = 357,
     VCI = 358,
     RADIO = 359,
     FISU = 360,
     LSSU = 361,
     MSU = 362,
     HFISU = 363,
     HLSSU = 364,
     HMSU = 365,
     SIO = 366,
     OPC = 367,
     DPC = 368,
     SLS = 369,
     HSIO = 370,
     HOPC = 371,
     HDPC = 372,
     HSLS = 373,
     AND = 374,
     OR = 375,
     UMINUS = 376
   };
#endif
/* Tokens.  */
#define DST 258
#define SRC 259
#define HOST 260
#define GATEWAY 261
#define NET 262
#define NETMASK 263
#define PORT 264
#define PORTRANGE 265
#define LESS 266
#define GREATER 267
#define PROTO 268
#define PROTOCHAIN 269
#define CBYTE 270
#define ARP 271
#define RARP 272
#define IP 273
#define SCTP 274
#define TCP 275
#define UDP 276
#define ICMP 277
#define IGMP 278
#define IGRP 279
#define PIM 280
#define VRRP 281
#define CARP 282
#define ATALK 283
#define AARP 284
#define DECNET 285
#define LAT 286
#define SCA 287
#define MOPRC 288
#define MOPDL 289
#define TK_BROADCAST 290
#define TK_MULTICAST 291
#define NUM 292
#define INBOUND 293
#define OUTBOUND 294
#define PF_IFNAME 295
#define PF_RSET 296
#define PF_RNR 297
#define PF_SRNR 298
#define PF_REASON 299
#define PF_ACTION 300
#define TYPE 301
#define SUBTYPE 302
#define DIR 303
#define ADDR1 304
#define ADDR2 305
#define ADDR3 306
#define ADDR4 307
#define RA 308
#define TA 309
#define LINK 310
#define GEQ 311
#define LEQ 312
#define NEQ 313
#define ID 314
#define EID 315
#define HID 316
#define HID6 317
#define AID 318
#define LSH 319
#define RSH 320
#define LEN 321
#define IPV6 322
#define ICMPV6 323
#define AH 324
#define ESP 325
#define VLAN 326
#define MPLS 327
#define PPPOED 328
#define PPPOES 329
#define GENEVE 330
#define ISO 331
#define ESIS 332
#define CLNP 333
#define ISIS 334
#define L1 335
#define L2 336
#define IIH 337
#define LSP 338
#define SNP 339
#define CSNP 340
#define PSNP 341
#define STP 342
#define IPX 343
#define NETBEUI 344
#define LANE 345
#define LLC 346
#define METAC 347
#define BCC 348
#define SC 349
#define ILMIC 350
#define OAMF4EC 351
#define OAMF4SC 352
#define OAM 353
#define OAMF4 354
#define CONNECTMSG 355
#define METACONNECT 356
#define VPI 357
#define VCI 358
#define RADIO 359
#define FISU 360
#define LSSU 361
#define MSU 362
#define HFISU 363
#define HLSSU 364
#define HMSU 365
#define SIO 366
#define OPC 367
#define DPC 368
#define SLS 369
#define HSIO 370
#define HOPC 371
#define HDPC 372
#define HSLS 373
#define AND 374
#define OR 375
#define UMINUS 376




/* Copy the first part of user declarations.  */
#line 1 "grammar.y"

/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WIN32
#include <pcap-stdinc.h>
#else /* WIN32 */
#include <sys/types.h>
#include <sys/socket.h>
#endif /* WIN32 */

#include <stdlib.h>

#ifndef WIN32
#if __STDC__
struct mbuf;
struct rtentry;
#endif

#include <netinet/in.h>
#include <arpa/inet.h>
#endif /* WIN32 */

#include <stdio.h>

#include "pcap-int.h"

#include "gencode.h"
#ifdef HAVE_NET_PFVAR_H
#include <net/if.h>
#include <net/pfvar.h>
#include <net/if_pflog.h>
#endif
#include "llc.h"
#include "ieee80211.h"
#include <pcap/namedb.h>

#ifdef HAVE_OS_PROTO_H
#include "os-proto.h"
#endif

#define QSET(q, p, d, a) (q).proto = (p),\
			 (q).dir = (d),\
			 (q).addr = (a)

struct tok {
	int v;			/* value */
	const char *s;		/* string */
};

static const struct tok ieee80211_types[] = {
	{ IEEE80211_FC0_TYPE_DATA, "data" },
	{ IEEE80211_FC0_TYPE_MGT, "mgt" },
	{ IEEE80211_FC0_TYPE_MGT, "management" },
	{ IEEE80211_FC0_TYPE_CTL, "ctl" },
	{ IEEE80211_FC0_TYPE_CTL, "control" },
	{ 0, NULL }
};
static const struct tok ieee80211_mgt_subtypes[] = {
	{ IEEE80211_FC0_SUBTYPE_ASSOC_REQ, "assocreq" },
	{ IEEE80211_FC0_SUBTYPE_ASSOC_REQ, "assoc-req" },
	{ IEEE80211_FC0_SUBTYPE_ASSOC_RESP, "assocresp" },
	{ IEEE80211_FC0_SUBTYPE_ASSOC_RESP, "assoc-resp" },
	{ IEEE80211_FC0_SUBTYPE_REASSOC_REQ, "reassocreq" },
	{ IEEE80211_FC0_SUBTYPE_REASSOC_REQ, "reassoc-req" },
	{ IEEE80211_FC0_SUBTYPE_REASSOC_RESP, "reassocresp" },
	{ IEEE80211_FC0_SUBTYPE_REASSOC_RESP, "reassoc-resp" },
	{ IEEE80211_FC0_SUBTYPE_PROBE_REQ, "probereq" },
	{ IEEE80211_FC0_SUBTYPE_PROBE_REQ, "probe-req" },
	{ IEEE80211_FC0_SUBTYPE_PROBE_RESP, "proberesp" },
	{ IEEE80211_FC0_SUBTYPE_PROBE_RESP, "probe-resp" },
	{ IEEE80211_FC0_SUBTYPE_BEACON, "beacon" },
	{ IEEE80211_FC0_SUBTYPE_ATIM, "atim" },
	{ IEEE80211_FC0_SUBTYPE_DISASSOC, "disassoc" },
	{ IEEE80211_FC0_SUBTYPE_DISASSOC, "disassociation" },
	{ IEEE80211_FC0_SUBTYPE_AUTH, "auth" },
	{ IEEE80211_FC0_SUBTYPE_AUTH, "authentication" },
	{ IEEE80211_FC0_SUBTYPE_DEAUTH, "deauth" },
	{ IEEE80211_FC0_SUBTYPE_DEAUTH, "deauthentication" },
	{ 0, NULL }
};
static const struct tok ieee80211_ctl_subtypes[] = {
	{ IEEE80211_FC0_SUBTYPE_PS_POLL, "ps-poll" },
	{ IEEE80211_FC0_SUBTYPE_RTS, "rts" },
	{ IEEE80211_FC0_SUBTYPE_CTS, "cts" },
	{ IEEE80211_FC0_SUBTYPE_ACK, "ack" },
	{ IEEE80211_FC0_SUBTYPE_CF_END, "cf-end" },
	{ IEEE80211_FC0_SUBTYPE_CF_END_ACK, "cf-end-ack" },
	{ 0, NULL }
};
static const struct tok ieee80211_data_subtypes[] = {
	{ IEEE80211_FC0_SUBTYPE_DATA, "data" },
	{ IEEE80211_FC0_SUBTYPE_CF_ACK, "data-cf-ack" },
	{ IEEE80211_FC0_SUBTYPE_CF_POLL, "data-cf-poll" },
	{ IEEE80211_FC0_SUBTYPE_CF_ACPL, "data-cf-ack-poll" },
	{ IEEE80211_FC0_SUBTYPE_NODATA, "null" },
	{ IEEE80211_FC0_SUBTYPE_NODATA_CF_ACK, "cf-ack" },
	{ IEEE80211_FC0_SUBTYPE_NODATA_CF_POLL, "cf-poll"  },
	{ IEEE80211_FC0_SUBTYPE_NODATA_CF_ACPL, "cf-ack-poll" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_DATA, "qos-data" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_CF_ACK, "qos-data-cf-ack" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_CF_POLL, "qos-data-cf-poll" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_CF_ACPL, "qos-data-cf-ack-poll" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_NODATA, "qos" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_NODATA_CF_POLL, "qos-cf-poll" },
	{ IEEE80211_FC0_SUBTYPE_QOS|IEEE80211_FC0_SUBTYPE_NODATA_CF_ACPL, "qos-cf-ack-poll" },
	{ 0, NULL }
};
static const struct tok llc_s_subtypes[] = {
	{ LLC_RR, "rr" },
	{ LLC_RNR, "rnr" },
	{ LLC_REJ, "rej" },
	{ 0, NULL }
};
static const struct tok llc_u_subtypes[] = {
	{ LLC_UI, "ui" },
	{ LLC_UA, "ua" },
	{ LLC_DISC, "disc" },
	{ LLC_DM, "dm" },
	{ LLC_SABME, "sabme" },
	{ LLC_TEST, "test" },
	{ LLC_XID, "xid" },
	{ LLC_FRMR, "frmr" },
	{ 0, NULL }
};
struct type2tok {
	int type;
	const struct tok *tok;
};
static const struct type2tok ieee80211_type_subtypes[] = {
	{ IEEE80211_FC0_TYPE_MGT, ieee80211_mgt_subtypes },
	{ IEEE80211_FC0_TYPE_CTL, ieee80211_ctl_subtypes },
	{ IEEE80211_FC0_TYPE_DATA, ieee80211_data_subtypes },
	{ 0, NULL }
};

static int
str2tok(const char *str, const struct tok *toks)
{
	int i;

	for (i = 0; toks[i].s != NULL; i++) {
		if (pcap_strcasecmp(toks[i].s, str) == 0)
			return (toks[i].v);
	}
	return (-1);
}

int n_errors = 0;

static struct qual qerr = { Q_UNDEF, Q_UNDEF, Q_UNDEF, Q_UNDEF };

static void
yyerror(const char *msg)
{
	++n_errors;
	bpf_error("%s", msg);
	/* NOTREACHED */
}

#ifdef NEED_YYPARSE_WRAPPER
int yyparse(void);

int
pcap_parse()
{
	return (yyparse());
}
#endif

#ifdef HAVE_NET_PFVAR_H
static int
pfreason_to_num(const char *reason)
{
	const char *reasons[] = PFRES_NAMES;
	int i;

	for (i = 0; reasons[i]; i++) {
		if (pcap_strcasecmp(reason, reasons[i]) == 0)
			return (i);
	}
	bpf_error("unknown PF reason");
	/*NOTREACHED*/
}

static int
pfaction_to_num(const char *action)
{
	if (pcap_strcasecmp(action, "pass") == 0 ||
	    pcap_strcasecmp(action, "accept") == 0)
		return (PF_PASS);
	else if (pcap_strcasecmp(action, "drop") == 0 ||
		pcap_strcasecmp(action, "block") == 0)
		return (PF_DROP);
#if HAVE_PF_NAT_THROUGH_PF_NORDR
	else if (pcap_strcasecmp(action, "rdr") == 0)
		return (PF_RDR);
	else if (pcap_strcasecmp(action, "nat") == 0)
		return (PF_NAT);
	else if (pcap_strcasecmp(action, "binat") == 0)
		return (PF_BINAT);
	else if (pcap_strcasecmp(action, "nordr") == 0)
		return (PF_NORDR);
#endif
	else {
		bpf_error("unknown PF action");
		/*NOTREACHED*/
	}
}
#else /* !HAVE_NET_PFVAR_H */
static int
pfreason_to_num(const char *reason)
{
	bpf_error("libpcap was compiled on a machine without pf support");
	/*NOTREACHED*/

	/* this is to make the VC compiler happy */
	return -1;
}

static int
pfaction_to_num(const char *action)
{
	bpf_error("libpcap was compiled on a machine without pf support");
	/*NOTREACHED*/

	/* this is to make the VC compiler happy */
	return -1;
}
#endif /* HAVE_NET_PFVAR_H */


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 256 "grammar.y"
{
	int i;
	bpf_u_int32 h;
	u_char *e;
	char *s;
	struct stmt *stmt;
	struct arth *a;
	struct {
		struct qual q;
		int atmfieldtype;
		int mtp3fieldtype;
		struct block *b;
	} blk;
	struct block *rblk;
}
/* Line 193 of yacc.c.  */
#line 617 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 630 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   788

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  220
/* YYNRULES -- Number of states.  */
#define YYNSTATES  294

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   376

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,     2,     2,     2,   137,   123,     2,
     130,   129,   126,   124,     2,   125,     2,   127,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   136,     2,
     133,   132,   131,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   134,     2,   135,   138,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   122,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   128
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     8,     9,    11,    15,    19,    23,
      27,    29,    31,    33,    35,    39,    41,    45,    49,    51,
      55,    57,    59,    61,    64,    66,    68,    70,    74,    78,
      80,    82,    84,    87,    91,    94,    97,   100,   103,   106,
     109,   113,   115,   119,   123,   125,   127,   129,   132,   134,
     137,   139,   140,   142,   144,   148,   152,   156,   160,   162,
     164,   166,   168,   170,   172,   174,   176,   178,   180,   182,
     184,   186,   188,   190,   192,   194,   196,   198,   200,   202,
     204,   206,   208,   210,   212,   214,   216,   218,   220,   222,
     224,   226,   228,   230,   232,   234,   236,   238,   240,   242,
     244,   246,   248,   250,   252,   254,   256,   258,   260,   263,
     266,   269,   272,   277,   279,   281,   284,   286,   289,   291,
     293,   296,   298,   301,   303,   305,   308,   310,   313,   316,
     319,   322,   325,   328,   333,   336,   339,   342,   344,   346,
     348,   350,   352,   354,   357,   360,   362,   364,   366,   368,
     370,   372,   374,   376,   378,   380,   382,   384,   386,   391,
     398,   402,   406,   410,   414,   418,   422,   426,   430,   434,
     438,   441,   445,   447,   449,   451,   453,   455,   457,   459,
     463,   465,   467,   469,   471,   473,   475,   477,   479,   481,
     483,   485,   487,   489,   491,   494,   497,   501,   503,   505,
     509,   511,   513,   515,   517,   519,   521,   523,   525,   527,
     529,   531,   533,   535,   537,   539,   542,   545,   549,   551,
     553
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     140,     0,    -1,   141,   142,    -1,   141,    -1,    -1,   151,
      -1,   142,   143,   151,    -1,   142,   143,   145,    -1,   142,
     144,   151,    -1,   142,   144,   145,    -1,   119,    -1,   120,
      -1,   146,    -1,   174,    -1,   148,   149,   129,    -1,    59,
      -1,    61,   127,    37,    -1,    61,     8,    61,    -1,    61,
      -1,    62,   127,    37,    -1,    62,    -1,    60,    -1,    63,
      -1,   147,   145,    -1,   121,    -1,   130,    -1,   146,    -1,
     150,   143,   145,    -1,   150,   144,   145,    -1,   174,    -1,
     149,    -1,   153,    -1,   147,   151,    -1,   154,   155,   156,
      -1,   154,   155,    -1,   154,   156,    -1,   154,    13,    -1,
     154,    14,    -1,   154,   157,    -1,   152,   145,    -1,   148,
     142,   129,    -1,   158,    -1,   171,   169,   171,    -1,   171,
     170,   171,    -1,   159,    -1,   175,    -1,   176,    -1,   177,
     178,    -1,   181,    -1,   182,   183,    -1,   158,    -1,    -1,
       4,    -1,     3,    -1,     4,   120,     3,    -1,     3,   120,
       4,    -1,     4,   119,     3,    -1,     3,   119,     4,    -1,
      49,    -1,    50,    -1,    51,    -1,    52,    -1,    53,    -1,
      54,    -1,     5,    -1,     7,    -1,     9,    -1,    10,    -1,
       6,    -1,    55,    -1,    18,    -1,    16,    -1,    17,    -1,
      19,    -1,    20,    -1,    21,    -1,    22,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    31,    -1,    32,    -1,    34,    -1,
      33,    -1,    67,    -1,    68,    -1,    69,    -1,    70,    -1,
      76,    -1,    77,    -1,    79,    -1,    80,    -1,    81,    -1,
      82,    -1,    83,    -1,    84,    -1,    86,    -1,    85,    -1,
      78,    -1,    87,    -1,    88,    -1,    89,    -1,   104,    -1,
     154,    35,    -1,   154,    36,    -1,    11,    37,    -1,    12,
      37,    -1,    15,    37,   173,    37,    -1,    38,    -1,    39,
      -1,    71,   174,    -1,    71,    -1,    72,   174,    -1,    72,
      -1,    73,    -1,    74,   174,    -1,    74,    -1,    75,   174,
      -1,    75,    -1,   160,    -1,   154,   161,    -1,   165,    -1,
      40,    59,    -1,    41,    59,    -1,    42,    37,    -1,    43,
      37,    -1,    44,   167,    -1,    45,   168,    -1,    46,   162,
      47,   163,    -1,    46,   162,    -1,    47,   164,    -1,    48,
     166,    -1,    37,    -1,    59,    -1,    37,    -1,    59,    -1,
      59,    -1,    91,    -1,    91,    59,    -1,    91,    42,    -1,
      37,    -1,    59,    -1,    37,    -1,    59,    -1,    59,    -1,
     131,    -1,    56,    -1,   132,    -1,    57,    -1,   133,    -1,
      58,    -1,   174,    -1,   172,    -1,   158,   134,   171,   135,
      -1,   158,   134,   171,   136,    37,   135,    -1,   171,   124,
     171,    -1,   171,   125,   171,    -1,   171,   126,   171,    -1,
     171,   127,   171,    -1,   171,   137,   171,    -1,   171,   123,
     171,    -1,   171,   122,   171,    -1,   171,   138,   171,    -1,
     171,    64,   171,    -1,   171,    65,   171,    -1,   125,   171,
      -1,   148,   172,   129,    -1,    66,    -1,   123,    -1,   122,
      -1,   133,    -1,   131,    -1,   132,    -1,    37,    -1,   148,
     174,   129,    -1,    90,    -1,    92,    -1,    93,    -1,    96,
      -1,    97,    -1,    94,    -1,    95,    -1,    98,    -1,    99,
      -1,   100,    -1,   101,    -1,   102,    -1,   103,    -1,   179,
      -1,   169,    37,    -1,   170,    37,    -1,   148,   180,   129,
      -1,    37,    -1,   179,    -1,   180,   144,   179,    -1,   105,
      -1,   106,    -1,   107,    -1,   108,    -1,   109,    -1,   110,
      -1,   111,    -1,   112,    -1,   113,    -1,   114,    -1,   115,
      -1,   116,    -1,   117,    -1,   118,    -1,   184,    -1,   169,
      37,    -1,   170,    37,    -1,   148,   185,   129,    -1,    37,
      -1,   184,    -1,   185,   144,   184,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   330,   330,   334,   336,   338,   339,   340,   341,   342,
     344,   346,   348,   349,   351,   353,   354,   356,   358,   371,
     380,   389,   398,   407,   409,   411,   413,   414,   415,   417,
     419,   421,   422,   424,   425,   426,   427,   428,   429,   431,
     432,   433,   434,   436,   438,   439,   440,   441,   442,   443,
     446,   447,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   464,   465,   466,   467,   470,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   533,   534,   535,
     536,   537,   538,   541,   546,   549,   553,   556,   557,   563,
     564,   584,   600,   601,   622,   625,   626,   639,   640,   643,
     646,   647,   648,   650,   651,   652,   654,   655,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
     669,   670,   671,   673,   674,   675,   676,   677,   679,   680,
     682,   683,   684,   685,   686,   687,   688,   690,   691,   692,
     693,   696,   697,   699,   700,   701,   702,   704,   711,   712,
     715,   716,   717,   718,   719,   720,   723,   724,   725,   726,
     727,   728,   729,   730,   732,   733,   734,   735,   737,   750,
     751
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DST", "SRC", "HOST", "GATEWAY", "NET",
  "NETMASK", "PORT", "PORTRANGE", "LESS", "GREATER", "PROTO", "PROTOCHAIN",
  "CBYTE", "ARP", "RARP", "IP", "SCTP", "TCP", "UDP", "ICMP", "IGMP",
  "IGRP", "PIM", "VRRP", "CARP", "ATALK", "AARP", "DECNET", "LAT", "SCA",
  "MOPRC", "MOPDL", "TK_BROADCAST", "TK_MULTICAST", "NUM", "INBOUND",
  "OUTBOUND", "PF_IFNAME", "PF_RSET", "PF_RNR", "PF_SRNR", "PF_REASON",
  "PF_ACTION", "TYPE", "SUBTYPE", "DIR", "ADDR1", "ADDR2", "ADDR3",
  "ADDR4", "RA", "TA", "LINK", "GEQ", "LEQ", "NEQ", "ID", "EID", "HID",
  "HID6", "AID", "LSH", "RSH", "LEN", "IPV6", "ICMPV6", "AH", "ESP",
  "VLAN", "MPLS", "PPPOED", "PPPOES", "GENEVE", "ISO", "ESIS", "CLNP",
  "ISIS", "L1", "L2", "IIH", "LSP", "SNP", "CSNP", "PSNP", "STP", "IPX",
  "NETBEUI", "LANE", "LLC", "METAC", "BCC", "SC", "ILMIC", "OAMF4EC",
  "OAMF4SC", "OAM", "OAMF4", "CONNECTMSG", "METACONNECT", "VPI", "VCI",
  "RADIO", "FISU", "LSSU", "MSU", "HFISU", "HLSSU", "HMSU", "SIO", "OPC",
  "DPC", "SLS", "HSIO", "HOPC", "HDPC", "HSLS", "AND", "OR", "'!'", "'|'",
  "'&'", "'+'", "'-'", "'*'", "'/'", "UMINUS", "')'", "'('", "'>'", "'='",
  "'<'", "'['", "']'", "':'", "'%'", "'^'", "$accept", "prog", "null",
  "expr", "and", "or", "id", "nid", "not", "paren", "pid", "qid", "term",
  "head", "rterm", "pqual", "dqual", "aqual", "ndaqual", "pname", "other",
  "pfvar", "p80211", "type", "subtype", "type_subtype", "pllc", "dir",
  "reason", "action", "relop", "irelop", "arth", "narth", "byteop", "pnum",
  "atmtype", "atmmultitype", "atmfield", "atmvalue", "atmfieldvalue",
  "atmlistvalue", "mtp2type", "mtp3field", "mtp3value", "mtp3fieldvalue",
  "mtp3listvalue", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    33,   124,    38,    43,    45,    42,    47,   376,    41,
      40,    62,    61,    60,    91,    93,    58,    37,    94
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   139,   140,   140,   141,   142,   142,   142,   142,   142,
     143,   144,   145,   145,   145,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   147,   148,   149,   149,   149,   150,
     150,   151,   151,   152,   152,   152,   152,   152,   152,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     154,   154,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   156,   156,   156,   156,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   160,
     160,   160,   160,   161,   161,   161,   161,   162,   162,   163,
     163,   164,   165,   165,   165,   166,   166,   167,   167,   168,
     169,   169,   169,   170,   170,   170,   171,   171,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   173,   173,   173,   173,   173,   174,   174,
     175,   175,   175,   175,   175,   175,   175,   176,   176,   176,
     176,   177,   177,   178,   178,   178,   178,   179,   180,   180,
     181,   181,   181,   181,   181,   181,   182,   182,   182,   182,
     182,   182,   182,   182,   183,   183,   183,   183,   184,   185,
     185
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     0,     1,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     1,     3,     3,     1,     3,
       1,     1,     1,     2,     1,     1,     1,     3,     3,     1,
       1,     1,     2,     3,     2,     2,     2,     2,     2,     2,
       3,     1,     3,     3,     1,     1,     1,     2,     1,     2,
       1,     0,     1,     1,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     4,     1,     1,     2,     1,     2,     1,     1,
       2,     1,     2,     1,     1,     2,     1,     2,     2,     2,
       2,     2,     2,     4,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     1,     1,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    51,     1,     0,     0,     0,    71,    72,    70,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    88,    87,   178,   113,   114,     0,
       0,     0,     0,     0,     0,    69,   172,    89,    90,    91,
      92,   116,   118,   119,   121,   123,    93,    94,   103,    95,
      96,    97,    98,    99,   100,   102,   101,   104,   105,   106,
     180,   142,   181,   182,   185,   186,   183,   184,   187,   188,
     189,   190,   191,   192,   107,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,    24,
       0,    25,     2,    51,    51,     5,     0,    31,     0,    50,
      44,   124,   126,     0,   157,   156,    45,    46,     0,    48,
       0,   110,   111,     0,   127,   128,   129,   130,   147,   148,
     131,   149,   132,     0,   115,   117,   120,   122,   144,   143,
       0,     0,   170,    10,    11,    51,    51,    32,     0,   157,
     156,    15,    21,    18,    20,    22,    39,    12,     0,     0,
      13,    53,    52,    64,    68,    65,    66,    67,    36,    37,
     108,   109,     0,     0,     0,    58,    59,    60,    61,    62,
      63,    34,    35,    38,   125,     0,   151,   153,   155,     0,
       0,     0,     0,     0,     0,     0,     0,   150,   152,   154,
       0,     0,     0,     0,   197,     0,     0,     0,    47,   193,
     218,     0,     0,     0,    49,   214,   174,   173,   176,   177,
     175,     0,     0,     0,     7,    51,    51,     6,   156,     9,
       8,    40,   171,   179,     0,     0,     0,    23,    26,    30,
       0,    29,     0,     0,     0,     0,   137,   138,   134,   141,
     135,   145,   146,   136,    33,     0,   168,   169,   166,   165,
     160,   161,   162,   163,   164,   167,    42,    43,   198,     0,
     194,   195,   219,     0,   215,   216,   112,   156,    17,    16,
      19,    14,     0,     0,    57,    55,    56,    54,     0,   158,
       0,   196,     0,   217,     0,    27,    28,   139,   140,   133,
       0,   199,   220,   159
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   138,   135,   136,   227,   147,   148,   130,
     229,   230,    95,    96,    97,    98,   171,   172,   173,   131,
     100,   101,   174,   238,   289,   240,   102,   243,   120,   122,
     192,   193,   103,   104,   211,   105,   106,   107,   108,   198,
     199,   259,   109,   110,   204,   205,   263
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -216
static const yytype_int16 yypact[] =
{
    -216,    24,   257,  -216,     0,    12,    17,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,    13,
      23,    29,    72,   -18,    54,  -216,  -216,  -216,  -216,  -216,
    -216,   -25,   -25,  -216,   -25,   -25,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,   -24,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
     604,  -216,   -30,   489,   489,  -216,   125,  -216,   734,     3,
    -216,  -216,  -216,   183,  -216,  -216,  -216,  -216,    -5,  -216,
      39,  -216,  -216,   -54,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,   -25,  -216,  -216,  -216,  -216,  -216,  -216,
     604,   -16,   -23,  -216,  -216,   373,   373,  -216,  -103,   -10,
       2,  -216,  -216,    -7,    11,  -216,  -216,  -216,   125,   125,
    -216,    -3,    21,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,   -12,    77,    -9,  -216,  -216,  -216,  -216,  -216,
    -216,    78,  -216,  -216,  -216,   604,  -216,  -216,  -216,   604,
     604,   604,   604,   604,   604,   604,   604,  -216,  -216,  -216,
     604,   604,   604,   604,  -216,   111,   113,   114,  -216,  -216,
    -216,   115,   124,   126,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,   131,     2,   575,  -216,   373,   373,  -216,    10,  -216,
    -216,  -216,  -216,  -216,   112,   137,   138,  -216,  -216,    47,
     -30,     2,   173,   176,   178,   186,  -216,  -216,   143,  -216,
    -216,  -216,  -216,  -216,  -216,   127,   -64,   -64,   580,   598,
    -104,  -104,   -23,   -23,   575,   575,   575,   575,  -216,   -99,
    -216,  -216,  -216,   -45,  -216,  -216,  -216,   -49,  -216,  -216,
    -216,  -216,   125,   125,  -216,  -216,  -216,  -216,    -1,  -216,
     156,  -216,   111,  -216,   115,  -216,  -216,  -216,  -216,  -216,
      59,  -216,  -216,  -216
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -216,  -216,  -216,   193,   -34,  -215,   -90,  -135,     7,    -2,
    -216,  -216,   -80,  -216,  -216,  -216,  -216,    26,  -216,     9,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
     -79,   -43,   -26,   -92,  -216,   -37,  -216,  -216,  -216,  -216,
    -175,  -216,  -216,  -216,  -216,  -174,  -216
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -42
static const yytype_int16 yytable[] =
{
      94,   224,   139,   -41,   124,   125,   146,   126,   127,    93,
     -13,    99,    26,   137,   228,   273,   133,   134,   128,   118,
     258,   134,   185,   186,     3,   236,   221,   262,   241,   196,
     281,   202,   194,   190,   191,   129,   287,   111,   139,   123,
     123,   119,   123,   123,   282,   214,   219,   237,   284,   112,
     242,   176,   177,   178,   113,   217,   220,   140,   288,   150,
     183,   184,   185,   186,   132,   197,   116,   203,   206,   207,
     -29,   -29,   114,   190,   191,   134,   200,   208,   209,   210,
     223,   228,   115,   153,   283,   155,   212,   156,   157,   133,
     134,    94,    94,   140,   149,   176,   177,   178,   218,   218,
      93,    93,    99,    99,   213,    91,   195,   291,   201,   117,
     292,   150,   231,   121,   190,   191,   232,   233,   175,   222,
     225,   123,   -41,   -41,   139,    91,   187,   188,   189,   -13,
     -13,   223,   -41,   216,   216,   137,   239,   175,   226,   -13,
     234,   235,   215,   215,    99,    99,   149,   123,   194,   245,
     260,   261,   200,   246,   247,   248,   249,   250,   251,   252,
     253,   264,    26,   265,   254,   255,   256,   257,   266,    91,
     187,   188,   189,   268,   269,   270,   271,   274,   218,   267,
     275,   276,   285,   286,   141,   142,   143,   144,   145,   277,
     278,   179,   180,   290,   293,    92,   272,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   216,    94,     0,     0,     0,     0,     0,
       0,     0,   215,   215,    99,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   150,   150,     0,     0,   176,
     177,   178,     0,     0,     0,     0,    89,   179,   180,   181,
     182,   183,   184,   185,   186,    91,     0,    -3,     0,     0,
       0,     0,   279,   280,   190,   191,     0,     0,     4,     5,
     149,   149,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     0,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,     0,     0,   181,   182,   183,   184,   185,
     186,     0,    35,     0,   187,   188,   189,     0,     0,     0,
     190,   191,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,     0,     0,    89,     0,
       0,     0,    90,     0,     4,     5,     0,    91,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,     0,     0,
      26,    27,    28,    29,    30,    31,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,   141,   142,   143,   144,   145,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,    89,     0,     0,     0,    90,     0,
       4,     5,     0,    91,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,     0,    26,    27,    28,    29,
      30,    31,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,     0,
      89,     0,     0,     0,    90,     0,     0,     0,     0,    91,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,   179,
     180,    26,     0,     0,   179,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
       0,     0,   179,   180,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,     0,     0,     0,     0,     0,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,     0,     0,     0,   181,   182,   183,
     184,   185,   186,   182,   183,   184,   185,   186,    74,     0,
       0,     0,   190,   191,     0,     0,     0,   190,   191,     0,
       0,     0,   183,   184,   185,   186,     0,     0,     0,    90,
       0,     0,     0,     0,    91,   190,   191,   151,   152,   153,
     154,   155,     0,   156,   157,     0,     0,   158,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,   163,   164,   165,   166,   167,   168,   169,   170
};

static const yytype_int16 yycheck[] =
{
       2,     8,    94,     0,    41,    42,    96,    44,    45,     2,
       0,     2,    37,    93,   149,   230,   119,   120,    42,    37,
     195,   120,   126,   127,     0,    37,   129,   201,    37,   108,
     129,   110,    37,   137,   138,    59,    37,    37,   130,    41,
      42,    59,    44,    45,   259,   135,   136,    59,   263,    37,
      59,    56,    57,    58,    37,   135,   136,    94,    59,    96,
     124,   125,   126,   127,    90,   108,    37,   110,   122,   123,
     119,   120,    59,   137,   138,   120,    37,   131,   132,   133,
     129,   216,    59,     5,   129,     7,   123,     9,    10,   119,
     120,    93,    94,   130,    96,    56,    57,    58,   135,   136,
      93,    94,    93,    94,   130,   130,   108,   282,   110,    37,
     284,   148,   149,    59,   137,   138,   119,   120,   134,   129,
     127,   123,   119,   120,   216,   130,   131,   132,   133,   119,
     120,   129,   129,   135,   136,   215,    59,   134,   127,   129,
     119,   120,   135,   136,   135,   136,   148,   149,    37,   175,
      37,    37,    37,   179,   180,   181,   182,   183,   184,   185,
     186,    37,    37,    37,   190,   191,   192,   193,    37,   130,
     131,   132,   133,    61,    37,    37,   129,     4,   215,   216,
       4,     3,   272,   273,    59,    60,    61,    62,    63,     3,
      47,    64,    65,    37,   135,     2,   230,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   215,   216,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   272,   273,    -1,    -1,    56,
      57,    58,    -1,    -1,    -1,    -1,   121,    64,    65,   122,
     123,   124,   125,   126,   127,   130,    -1,     0,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,    -1,    -1,    11,    12,
     272,   273,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    55,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,   138,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    11,    12,    -1,   130,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    59,    60,    61,    62,    63,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      11,    12,    -1,   130,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,   130,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    64,
      65,    37,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   123,   124,   125,   126,   127,   104,    -1,
      -1,    -1,   137,   138,    -1,    -1,    -1,   137,   138,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,   130,   137,   138,     3,     4,     5,
       6,     7,    -1,     9,    10,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   140,   141,     0,    11,    12,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    55,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     125,   130,   142,   147,   148,   151,   152,   153,   154,   158,
     159,   160,   165,   171,   172,   174,   175,   176,   177,   181,
     182,    37,    37,    37,    59,    59,    37,    37,    37,    59,
     167,    59,   168,   148,   174,   174,   174,   174,    42,    59,
     148,   158,   171,   119,   120,   143,   144,   151,   142,   172,
     174,    59,    60,    61,    62,    63,   145,   146,   147,   148,
     174,     3,     4,     5,     6,     7,     9,    10,    13,    14,
      35,    36,    46,    47,    48,    49,    50,    51,    52,    53,
      54,   155,   156,   157,   161,   134,    56,    57,    58,    64,
      65,   122,   123,   124,   125,   126,   127,   131,   132,   133,
     137,   138,   169,   170,    37,   148,   169,   170,   178,   179,
      37,   148,   169,   170,   183,   184,   122,   123,   131,   132,
     133,   173,   174,   171,   145,   147,   148,   151,   174,   145,
     151,   129,   129,   129,     8,   127,   127,   145,   146,   149,
     150,   174,   119,   120,   119,   120,    37,    59,   162,    59,
     164,    37,    59,   166,   156,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   179,   180,
      37,    37,   184,   185,    37,    37,    37,   174,    61,    37,
      37,   129,   143,   144,     4,     4,     3,     3,    47,   135,
     136,   129,   144,   129,   144,   145,   145,    37,    59,   163,
      37,   179,   184,   135
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 331 "grammar.y"
    {
	finish_parse((yyvsp[(2) - (2)].blk).b);
}
    break;

  case 4:
#line 336 "grammar.y"
    { (yyval.blk).q = qerr; }
    break;

  case 6:
#line 339 "grammar.y"
    { gen_and((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 7:
#line 340 "grammar.y"
    { gen_and((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 8:
#line 341 "grammar.y"
    { gen_or((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 9:
#line 342 "grammar.y"
    { gen_or((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 10:
#line 344 "grammar.y"
    { (yyval.blk) = (yyvsp[(0) - (1)].blk); }
    break;

  case 11:
#line 346 "grammar.y"
    { (yyval.blk) = (yyvsp[(0) - (1)].blk); }
    break;

  case 13:
#line 349 "grammar.y"
    { (yyval.blk).b = gen_ncode(NULL, (bpf_u_int32)(yyvsp[(1) - (1)].i),
						   (yyval.blk).q = (yyvsp[(0) - (1)].blk).q); }
    break;

  case 14:
#line 351 "grammar.y"
    { (yyval.blk) = (yyvsp[(2) - (3)].blk); }
    break;

  case 15:
#line 353 "grammar.y"
    { (yyval.blk).b = gen_scode((yyvsp[(1) - (1)].s), (yyval.blk).q = (yyvsp[(0) - (1)].blk).q); }
    break;

  case 16:
#line 354 "grammar.y"
    { (yyval.blk).b = gen_mcode((yyvsp[(1) - (3)].s), NULL, (yyvsp[(3) - (3)].i),
				    (yyval.blk).q = (yyvsp[(0) - (3)].blk).q); }
    break;

  case 17:
#line 356 "grammar.y"
    { (yyval.blk).b = gen_mcode((yyvsp[(1) - (3)].s), (yyvsp[(3) - (3)].s), 0,
				    (yyval.blk).q = (yyvsp[(0) - (3)].blk).q); }
    break;

  case 18:
#line 358 "grammar.y"
    {
				  /* Decide how to parse HID based on proto */
				  (yyval.blk).q = (yyvsp[(0) - (1)].blk).q;
				  if ((yyval.blk).q.addr == Q_PORT)
				  	bpf_error("'port' modifier applied to ip host");
				  else if ((yyval.blk).q.addr == Q_PORTRANGE)
				  	bpf_error("'portrange' modifier applied to ip host");
				  else if ((yyval.blk).q.addr == Q_PROTO)
				  	bpf_error("'proto' modifier applied to ip host");
				  else if ((yyval.blk).q.addr == Q_PROTOCHAIN)
				  	bpf_error("'protochain' modifier applied to ip host");
				  (yyval.blk).b = gen_ncode((yyvsp[(1) - (1)].s), 0, (yyval.blk).q);
				}
    break;

  case 19:
#line 371 "grammar.y"
    {
#ifdef INET6
				  (yyval.blk).b = gen_mcode6((yyvsp[(1) - (3)].s), NULL, (yyvsp[(3) - (3)].i),
				    (yyval.blk).q = (yyvsp[(0) - (3)].blk).q);
#else
				  bpf_error("'ip6addr/prefixlen' not supported "
					"in this configuration");
#endif /*INET6*/
				}
    break;

  case 20:
#line 380 "grammar.y"
    {
#ifdef INET6
				  (yyval.blk).b = gen_mcode6((yyvsp[(1) - (1)].s), 0, 128,
				    (yyval.blk).q = (yyvsp[(0) - (1)].blk).q);
#else
				  bpf_error("'ip6addr' not supported "
					"in this configuration");
#endif /*INET6*/
				}
    break;

  case 21:
#line 389 "grammar.y"
    {
				  (yyval.blk).b = gen_ecode((yyvsp[(1) - (1)].e), (yyval.blk).q = (yyvsp[(0) - (1)].blk).q);
				  /*
				   * $1 was allocated by "pcap_ether_aton()",
				   * so we must free it now that we're done
				   * with it.
				   */
				  free((yyvsp[(1) - (1)].e));
				}
    break;

  case 22:
#line 398 "grammar.y"
    {
				  (yyval.blk).b = gen_acode((yyvsp[(1) - (1)].e), (yyval.blk).q = (yyvsp[(0) - (1)].blk).q);
				  /*
				   * $1 was allocated by "pcap_ether_aton()",
				   * so we must free it now that we're done
				   * with it.
				   */
				  free((yyvsp[(1) - (1)].e));
				}
    break;

  case 23:
#line 407 "grammar.y"
    { gen_not((yyvsp[(2) - (2)].blk).b); (yyval.blk) = (yyvsp[(2) - (2)].blk); }
    break;

  case 24:
#line 409 "grammar.y"
    { (yyval.blk) = (yyvsp[(0) - (1)].blk); }
    break;

  case 25:
#line 411 "grammar.y"
    { (yyval.blk) = (yyvsp[(0) - (1)].blk); }
    break;

  case 27:
#line 414 "grammar.y"
    { gen_and((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 28:
#line 415 "grammar.y"
    { gen_or((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 29:
#line 417 "grammar.y"
    { (yyval.blk).b = gen_ncode(NULL, (bpf_u_int32)(yyvsp[(1) - (1)].i),
						   (yyval.blk).q = (yyvsp[(0) - (1)].blk).q); }
    break;

  case 32:
#line 422 "grammar.y"
    { gen_not((yyvsp[(2) - (2)].blk).b); (yyval.blk) = (yyvsp[(2) - (2)].blk); }
    break;

  case 33:
#line 424 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (3)].i), (yyvsp[(2) - (3)].i), (yyvsp[(3) - (3)].i)); }
    break;

  case 34:
#line 425 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (2)].i), (yyvsp[(2) - (2)].i), Q_DEFAULT); }
    break;

  case 35:
#line 426 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (2)].i), Q_DEFAULT, (yyvsp[(2) - (2)].i)); }
    break;

  case 36:
#line 427 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (2)].i), Q_DEFAULT, Q_PROTO); }
    break;

  case 37:
#line 428 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (2)].i), Q_DEFAULT, Q_PROTOCHAIN); }
    break;

  case 38:
#line 429 "grammar.y"
    { QSET((yyval.blk).q, (yyvsp[(1) - (2)].i), Q_DEFAULT, (yyvsp[(2) - (2)].i)); }
    break;

  case 39:
#line 431 "grammar.y"
    { (yyval.blk) = (yyvsp[(2) - (2)].blk); }
    break;

  case 40:
#line 432 "grammar.y"
    { (yyval.blk).b = (yyvsp[(2) - (3)].blk).b; (yyval.blk).q = (yyvsp[(1) - (3)].blk).q; }
    break;

  case 41:
#line 433 "grammar.y"
    { (yyval.blk).b = gen_proto_abbrev((yyvsp[(1) - (1)].i)); (yyval.blk).q = qerr; }
    break;

  case 42:
#line 434 "grammar.y"
    { (yyval.blk).b = gen_relation((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a), 0);
				  (yyval.blk).q = qerr; }
    break;

  case 43:
#line 436 "grammar.y"
    { (yyval.blk).b = gen_relation((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a), 1);
				  (yyval.blk).q = qerr; }
    break;

  case 44:
#line 438 "grammar.y"
    { (yyval.blk).b = (yyvsp[(1) - (1)].rblk); (yyval.blk).q = qerr; }
    break;

  case 45:
#line 439 "grammar.y"
    { (yyval.blk).b = gen_atmtype_abbrev((yyvsp[(1) - (1)].i)); (yyval.blk).q = qerr; }
    break;

  case 46:
#line 440 "grammar.y"
    { (yyval.blk).b = gen_atmmulti_abbrev((yyvsp[(1) - (1)].i)); (yyval.blk).q = qerr; }
    break;

  case 47:
#line 441 "grammar.y"
    { (yyval.blk).b = (yyvsp[(2) - (2)].blk).b; (yyval.blk).q = qerr; }
    break;

  case 48:
#line 442 "grammar.y"
    { (yyval.blk).b = gen_mtp2type_abbrev((yyvsp[(1) - (1)].i)); (yyval.blk).q = qerr; }
    break;

  case 49:
#line 443 "grammar.y"
    { (yyval.blk).b = (yyvsp[(2) - (2)].blk).b; (yyval.blk).q = qerr; }
    break;

  case 51:
#line 447 "grammar.y"
    { (yyval.i) = Q_DEFAULT; }
    break;

  case 52:
#line 450 "grammar.y"
    { (yyval.i) = Q_SRC; }
    break;

  case 53:
#line 451 "grammar.y"
    { (yyval.i) = Q_DST; }
    break;

  case 54:
#line 452 "grammar.y"
    { (yyval.i) = Q_OR; }
    break;

  case 55:
#line 453 "grammar.y"
    { (yyval.i) = Q_OR; }
    break;

  case 56:
#line 454 "grammar.y"
    { (yyval.i) = Q_AND; }
    break;

  case 57:
#line 455 "grammar.y"
    { (yyval.i) = Q_AND; }
    break;

  case 58:
#line 456 "grammar.y"
    { (yyval.i) = Q_ADDR1; }
    break;

  case 59:
#line 457 "grammar.y"
    { (yyval.i) = Q_ADDR2; }
    break;

  case 60:
#line 458 "grammar.y"
    { (yyval.i) = Q_ADDR3; }
    break;

  case 61:
#line 459 "grammar.y"
    { (yyval.i) = Q_ADDR4; }
    break;

  case 62:
#line 460 "grammar.y"
    { (yyval.i) = Q_RA; }
    break;

  case 63:
#line 461 "grammar.y"
    { (yyval.i) = Q_TA; }
    break;

  case 64:
#line 464 "grammar.y"
    { (yyval.i) = Q_HOST; }
    break;

  case 65:
#line 465 "grammar.y"
    { (yyval.i) = Q_NET; }
    break;

  case 66:
#line 466 "grammar.y"
    { (yyval.i) = Q_PORT; }
    break;

  case 67:
#line 467 "grammar.y"
    { (yyval.i) = Q_PORTRANGE; }
    break;

  case 68:
#line 470 "grammar.y"
    { (yyval.i) = Q_GATEWAY; }
    break;

  case 69:
#line 472 "grammar.y"
    { (yyval.i) = Q_LINK; }
    break;

  case 70:
#line 473 "grammar.y"
    { (yyval.i) = Q_IP; }
    break;

  case 71:
#line 474 "grammar.y"
    { (yyval.i) = Q_ARP; }
    break;

  case 72:
#line 475 "grammar.y"
    { (yyval.i) = Q_RARP; }
    break;

  case 73:
#line 476 "grammar.y"
    { (yyval.i) = Q_SCTP; }
    break;

  case 74:
#line 477 "grammar.y"
    { (yyval.i) = Q_TCP; }
    break;

  case 75:
#line 478 "grammar.y"
    { (yyval.i) = Q_UDP; }
    break;

  case 76:
#line 479 "grammar.y"
    { (yyval.i) = Q_ICMP; }
    break;

  case 77:
#line 480 "grammar.y"
    { (yyval.i) = Q_IGMP; }
    break;

  case 78:
#line 481 "grammar.y"
    { (yyval.i) = Q_IGRP; }
    break;

  case 79:
#line 482 "grammar.y"
    { (yyval.i) = Q_PIM; }
    break;

  case 80:
#line 483 "grammar.y"
    { (yyval.i) = Q_VRRP; }
    break;

  case 81:
#line 484 "grammar.y"
    { (yyval.i) = Q_CARP; }
    break;

  case 82:
#line 485 "grammar.y"
    { (yyval.i) = Q_ATALK; }
    break;

  case 83:
#line 486 "grammar.y"
    { (yyval.i) = Q_AARP; }
    break;

  case 84:
#line 487 "grammar.y"
    { (yyval.i) = Q_DECNET; }
    break;

  case 85:
#line 488 "grammar.y"
    { (yyval.i) = Q_LAT; }
    break;

  case 86:
#line 489 "grammar.y"
    { (yyval.i) = Q_SCA; }
    break;

  case 87:
#line 490 "grammar.y"
    { (yyval.i) = Q_MOPDL; }
    break;

  case 88:
#line 491 "grammar.y"
    { (yyval.i) = Q_MOPRC; }
    break;

  case 89:
#line 492 "grammar.y"
    { (yyval.i) = Q_IPV6; }
    break;

  case 90:
#line 493 "grammar.y"
    { (yyval.i) = Q_ICMPV6; }
    break;

  case 91:
#line 494 "grammar.y"
    { (yyval.i) = Q_AH; }
    break;

  case 92:
#line 495 "grammar.y"
    { (yyval.i) = Q_ESP; }
    break;

  case 93:
#line 496 "grammar.y"
    { (yyval.i) = Q_ISO; }
    break;

  case 94:
#line 497 "grammar.y"
    { (yyval.i) = Q_ESIS; }
    break;

  case 95:
#line 498 "grammar.y"
    { (yyval.i) = Q_ISIS; }
    break;

  case 96:
#line 499 "grammar.y"
    { (yyval.i) = Q_ISIS_L1; }
    break;

  case 97:
#line 500 "grammar.y"
    { (yyval.i) = Q_ISIS_L2; }
    break;

  case 98:
#line 501 "grammar.y"
    { (yyval.i) = Q_ISIS_IIH; }
    break;

  case 99:
#line 502 "grammar.y"
    { (yyval.i) = Q_ISIS_LSP; }
    break;

  case 100:
#line 503 "grammar.y"
    { (yyval.i) = Q_ISIS_SNP; }
    break;

  case 101:
#line 504 "grammar.y"
    { (yyval.i) = Q_ISIS_PSNP; }
    break;

  case 102:
#line 505 "grammar.y"
    { (yyval.i) = Q_ISIS_CSNP; }
    break;

  case 103:
#line 506 "grammar.y"
    { (yyval.i) = Q_CLNP; }
    break;

  case 104:
#line 507 "grammar.y"
    { (yyval.i) = Q_STP; }
    break;

  case 105:
#line 508 "grammar.y"
    { (yyval.i) = Q_IPX; }
    break;

  case 106:
#line 509 "grammar.y"
    { (yyval.i) = Q_NETBEUI; }
    break;

  case 107:
#line 510 "grammar.y"
    { (yyval.i) = Q_RADIO; }
    break;

  case 108:
#line 512 "grammar.y"
    { (yyval.rblk) = gen_broadcast((yyvsp[(1) - (2)].i)); }
    break;

  case 109:
#line 513 "grammar.y"
    { (yyval.rblk) = gen_multicast((yyvsp[(1) - (2)].i)); }
    break;

  case 110:
#line 514 "grammar.y"
    { (yyval.rblk) = gen_less((yyvsp[(2) - (2)].i)); }
    break;

  case 111:
#line 515 "grammar.y"
    { (yyval.rblk) = gen_greater((yyvsp[(2) - (2)].i)); }
    break;

  case 112:
#line 516 "grammar.y"
    { (yyval.rblk) = gen_byteop((yyvsp[(3) - (4)].i), (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)); }
    break;

  case 113:
#line 517 "grammar.y"
    { (yyval.rblk) = gen_inbound(0); }
    break;

  case 114:
#line 518 "grammar.y"
    { (yyval.rblk) = gen_inbound(1); }
    break;

  case 115:
#line 519 "grammar.y"
    { (yyval.rblk) = gen_vlan((yyvsp[(2) - (2)].i)); }
    break;

  case 116:
#line 520 "grammar.y"
    { (yyval.rblk) = gen_vlan(-1); }
    break;

  case 117:
#line 521 "grammar.y"
    { (yyval.rblk) = gen_mpls((yyvsp[(2) - (2)].i)); }
    break;

  case 118:
#line 522 "grammar.y"
    { (yyval.rblk) = gen_mpls(-1); }
    break;

  case 119:
#line 523 "grammar.y"
    { (yyval.rblk) = gen_pppoed(); }
    break;

  case 120:
#line 524 "grammar.y"
    { (yyval.rblk) = gen_pppoes((yyvsp[(2) - (2)].i)); }
    break;

  case 121:
#line 525 "grammar.y"
    { (yyval.rblk) = gen_pppoes(-1); }
    break;

  case 122:
#line 526 "grammar.y"
    { (yyval.rblk) = gen_geneve((yyvsp[(2) - (2)].i)); }
    break;

  case 123:
#line 527 "grammar.y"
    { (yyval.rblk) = gen_geneve(-1); }
    break;

  case 124:
#line 528 "grammar.y"
    { (yyval.rblk) = (yyvsp[(1) - (1)].rblk); }
    break;

  case 125:
#line 529 "grammar.y"
    { (yyval.rblk) = (yyvsp[(2) - (2)].rblk); }
    break;

  case 126:
#line 530 "grammar.y"
    { (yyval.rblk) = (yyvsp[(1) - (1)].rblk); }
    break;

  case 127:
#line 533 "grammar.y"
    { (yyval.rblk) = gen_pf_ifname((yyvsp[(2) - (2)].s)); }
    break;

  case 128:
#line 534 "grammar.y"
    { (yyval.rblk) = gen_pf_ruleset((yyvsp[(2) - (2)].s)); }
    break;

  case 129:
#line 535 "grammar.y"
    { (yyval.rblk) = gen_pf_rnr((yyvsp[(2) - (2)].i)); }
    break;

  case 130:
#line 536 "grammar.y"
    { (yyval.rblk) = gen_pf_srnr((yyvsp[(2) - (2)].i)); }
    break;

  case 131:
#line 537 "grammar.y"
    { (yyval.rblk) = gen_pf_reason((yyvsp[(2) - (2)].i)); }
    break;

  case 132:
#line 538 "grammar.y"
    { (yyval.rblk) = gen_pf_action((yyvsp[(2) - (2)].i)); }
    break;

  case 133:
#line 542 "grammar.y"
    { (yyval.rblk) = gen_p80211_type((yyvsp[(2) - (4)].i) | (yyvsp[(4) - (4)].i),
					IEEE80211_FC0_TYPE_MASK |
					IEEE80211_FC0_SUBTYPE_MASK);
				}
    break;

  case 134:
#line 546 "grammar.y"
    { (yyval.rblk) = gen_p80211_type((yyvsp[(2) - (2)].i),
					IEEE80211_FC0_TYPE_MASK);
				}
    break;

  case 135:
#line 549 "grammar.y"
    { (yyval.rblk) = gen_p80211_type((yyvsp[(2) - (2)].i),
					IEEE80211_FC0_TYPE_MASK |
					IEEE80211_FC0_SUBTYPE_MASK);
				}
    break;

  case 136:
#line 553 "grammar.y"
    { (yyval.rblk) = gen_p80211_fcdir((yyvsp[(2) - (2)].i)); }
    break;

  case 138:
#line 557 "grammar.y"
    { (yyval.i) = str2tok((yyvsp[(1) - (1)].s), ieee80211_types);
				  if ((yyval.i) == -1)
				  	bpf_error("unknown 802.11 type name");
				}
    break;

  case 140:
#line 564 "grammar.y"
    { const struct tok *types = NULL;
				  int i;
				  for (i = 0;; i++) {
				  	if (ieee80211_type_subtypes[i].tok == NULL) {
				  		/* Ran out of types */
						bpf_error("unknown 802.11 type");
						break;
					}
					if ((yyvsp[(-1) - (1)].i) == ieee80211_type_subtypes[i].type) {
						types = ieee80211_type_subtypes[i].tok;
						break;
					}
				  }

				  (yyval.i) = str2tok((yyvsp[(1) - (1)].s), types);
				  if ((yyval.i) == -1)
					bpf_error("unknown 802.11 subtype name");
				}
    break;

  case 141:
#line 584 "grammar.y"
    { int i;
				  for (i = 0;; i++) {
				  	if (ieee80211_type_subtypes[i].tok == NULL) {
				  		/* Ran out of types */
						bpf_error("unknown 802.11 type name");
						break;
					}
					(yyval.i) = str2tok((yyvsp[(1) - (1)].s), ieee80211_type_subtypes[i].tok);
					if ((yyval.i) != -1) {
						(yyval.i) |= ieee80211_type_subtypes[i].type;
						break;
					}
				  }
				}
    break;

  case 142:
#line 600 "grammar.y"
    { (yyval.rblk) = gen_llc(); }
    break;

  case 143:
#line 601 "grammar.y"
    { if (pcap_strcasecmp((yyvsp[(2) - (2)].s), "i") == 0)
					(yyval.rblk) = gen_llc_i();
				  else if (pcap_strcasecmp((yyvsp[(2) - (2)].s), "s") == 0)
					(yyval.rblk) = gen_llc_s();
				  else if (pcap_strcasecmp((yyvsp[(2) - (2)].s), "u") == 0)
					(yyval.rblk) = gen_llc_u();
				  else {
				  	u_int subtype;

					subtype = str2tok((yyvsp[(2) - (2)].s), llc_s_subtypes);
					if (subtype != -1)
						(yyval.rblk) = gen_llc_s_subtype(subtype);
					else {
						subtype = str2tok((yyvsp[(2) - (2)].s), llc_u_subtypes);
						if (subtype == -1)
					  		bpf_error("unknown LLC type name \"%s\"", (yyvsp[(2) - (2)].s));
						(yyval.rblk) = gen_llc_u_subtype(subtype);
					}
				  }
				}
    break;

  case 144:
#line 622 "grammar.y"
    { (yyval.rblk) = gen_llc_s_subtype(LLC_RNR); }
    break;

  case 146:
#line 626 "grammar.y"
    { if (pcap_strcasecmp((yyvsp[(1) - (1)].s), "nods") == 0)
					(yyval.i) = IEEE80211_FC1_DIR_NODS;
				  else if (pcap_strcasecmp((yyvsp[(1) - (1)].s), "tods") == 0)
					(yyval.i) = IEEE80211_FC1_DIR_TODS;
				  else if (pcap_strcasecmp((yyvsp[(1) - (1)].s), "fromds") == 0)
					(yyval.i) = IEEE80211_FC1_DIR_FROMDS;
				  else if (pcap_strcasecmp((yyvsp[(1) - (1)].s), "dstods") == 0)
					(yyval.i) = IEEE80211_FC1_DIR_DSTODS;
				  else
					bpf_error("unknown 802.11 direction");
				}
    break;

  case 147:
#line 639 "grammar.y"
    { (yyval.i) = (yyvsp[(1) - (1)].i); }
    break;

  case 148:
#line 640 "grammar.y"
    { (yyval.i) = pfreason_to_num((yyvsp[(1) - (1)].s)); }
    break;

  case 149:
#line 643 "grammar.y"
    { (yyval.i) = pfaction_to_num((yyvsp[(1) - (1)].s)); }
    break;

  case 150:
#line 646 "grammar.y"
    { (yyval.i) = BPF_JGT; }
    break;

  case 151:
#line 647 "grammar.y"
    { (yyval.i) = BPF_JGE; }
    break;

  case 152:
#line 648 "grammar.y"
    { (yyval.i) = BPF_JEQ; }
    break;

  case 153:
#line 650 "grammar.y"
    { (yyval.i) = BPF_JGT; }
    break;

  case 154:
#line 651 "grammar.y"
    { (yyval.i) = BPF_JGE; }
    break;

  case 155:
#line 652 "grammar.y"
    { (yyval.i) = BPF_JEQ; }
    break;

  case 156:
#line 654 "grammar.y"
    { (yyval.a) = gen_loadi((yyvsp[(1) - (1)].i)); }
    break;

  case 158:
#line 657 "grammar.y"
    { (yyval.a) = gen_load((yyvsp[(1) - (4)].i), (yyvsp[(3) - (4)].a), 1); }
    break;

  case 159:
#line 658 "grammar.y"
    { (yyval.a) = gen_load((yyvsp[(1) - (6)].i), (yyvsp[(3) - (6)].a), (yyvsp[(5) - (6)].i)); }
    break;

  case 160:
#line 659 "grammar.y"
    { (yyval.a) = gen_arth(BPF_ADD, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 161:
#line 660 "grammar.y"
    { (yyval.a) = gen_arth(BPF_SUB, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 162:
#line 661 "grammar.y"
    { (yyval.a) = gen_arth(BPF_MUL, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 163:
#line 662 "grammar.y"
    { (yyval.a) = gen_arth(BPF_DIV, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 164:
#line 663 "grammar.y"
    { (yyval.a) = gen_arth(BPF_MOD, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 165:
#line 664 "grammar.y"
    { (yyval.a) = gen_arth(BPF_AND, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 166:
#line 665 "grammar.y"
    { (yyval.a) = gen_arth(BPF_OR, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 167:
#line 666 "grammar.y"
    { (yyval.a) = gen_arth(BPF_XOR, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 168:
#line 667 "grammar.y"
    { (yyval.a) = gen_arth(BPF_LSH, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 169:
#line 668 "grammar.y"
    { (yyval.a) = gen_arth(BPF_RSH, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); }
    break;

  case 170:
#line 669 "grammar.y"
    { (yyval.a) = gen_neg((yyvsp[(2) - (2)].a)); }
    break;

  case 171:
#line 670 "grammar.y"
    { (yyval.a) = (yyvsp[(2) - (3)].a); }
    break;

  case 172:
#line 671 "grammar.y"
    { (yyval.a) = gen_loadlen(); }
    break;

  case 173:
#line 673 "grammar.y"
    { (yyval.i) = '&'; }
    break;

  case 174:
#line 674 "grammar.y"
    { (yyval.i) = '|'; }
    break;

  case 175:
#line 675 "grammar.y"
    { (yyval.i) = '<'; }
    break;

  case 176:
#line 676 "grammar.y"
    { (yyval.i) = '>'; }
    break;

  case 177:
#line 677 "grammar.y"
    { (yyval.i) = '='; }
    break;

  case 179:
#line 680 "grammar.y"
    { (yyval.i) = (yyvsp[(2) - (3)].i); }
    break;

  case 180:
#line 682 "grammar.y"
    { (yyval.i) = A_LANE; }
    break;

  case 181:
#line 683 "grammar.y"
    { (yyval.i) = A_METAC;	}
    break;

  case 182:
#line 684 "grammar.y"
    { (yyval.i) = A_BCC; }
    break;

  case 183:
#line 685 "grammar.y"
    { (yyval.i) = A_OAMF4EC; }
    break;

  case 184:
#line 686 "grammar.y"
    { (yyval.i) = A_OAMF4SC; }
    break;

  case 185:
#line 687 "grammar.y"
    { (yyval.i) = A_SC; }
    break;

  case 186:
#line 688 "grammar.y"
    { (yyval.i) = A_ILMIC; }
    break;

  case 187:
#line 690 "grammar.y"
    { (yyval.i) = A_OAM; }
    break;

  case 188:
#line 691 "grammar.y"
    { (yyval.i) = A_OAMF4; }
    break;

  case 189:
#line 692 "grammar.y"
    { (yyval.i) = A_CONNECTMSG; }
    break;

  case 190:
#line 693 "grammar.y"
    { (yyval.i) = A_METACONNECT; }
    break;

  case 191:
#line 696 "grammar.y"
    { (yyval.blk).atmfieldtype = A_VPI; }
    break;

  case 192:
#line 697 "grammar.y"
    { (yyval.blk).atmfieldtype = A_VCI; }
    break;

  case 194:
#line 700 "grammar.y"
    { (yyval.blk).b = gen_atmfield_code((yyvsp[(0) - (2)].blk).atmfieldtype, (bpf_int32)(yyvsp[(2) - (2)].i), (bpf_u_int32)(yyvsp[(1) - (2)].i), 0); }
    break;

  case 195:
#line 701 "grammar.y"
    { (yyval.blk).b = gen_atmfield_code((yyvsp[(0) - (2)].blk).atmfieldtype, (bpf_int32)(yyvsp[(2) - (2)].i), (bpf_u_int32)(yyvsp[(1) - (2)].i), 1); }
    break;

  case 196:
#line 702 "grammar.y"
    { (yyval.blk).b = (yyvsp[(2) - (3)].blk).b; (yyval.blk).q = qerr; }
    break;

  case 197:
#line 704 "grammar.y"
    {
	(yyval.blk).atmfieldtype = (yyvsp[(0) - (1)].blk).atmfieldtype;
	if ((yyval.blk).atmfieldtype == A_VPI ||
	    (yyval.blk).atmfieldtype == A_VCI)
		(yyval.blk).b = gen_atmfield_code((yyval.blk).atmfieldtype, (bpf_int32) (yyvsp[(1) - (1)].i), BPF_JEQ, 0);
	}
    break;

  case 199:
#line 712 "grammar.y"
    { gen_or((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;

  case 200:
#line 715 "grammar.y"
    { (yyval.i) = M_FISU; }
    break;

  case 201:
#line 716 "grammar.y"
    { (yyval.i) = M_LSSU; }
    break;

  case 202:
#line 717 "grammar.y"
    { (yyval.i) = M_MSU; }
    break;

  case 203:
#line 718 "grammar.y"
    { (yyval.i) = MH_FISU; }
    break;

  case 204:
#line 719 "grammar.y"
    { (yyval.i) = MH_LSSU; }
    break;

  case 205:
#line 720 "grammar.y"
    { (yyval.i) = MH_MSU; }
    break;

  case 206:
#line 723 "grammar.y"
    { (yyval.blk).mtp3fieldtype = M_SIO; }
    break;

  case 207:
#line 724 "grammar.y"
    { (yyval.blk).mtp3fieldtype = M_OPC; }
    break;

  case 208:
#line 725 "grammar.y"
    { (yyval.blk).mtp3fieldtype = M_DPC; }
    break;

  case 209:
#line 726 "grammar.y"
    { (yyval.blk).mtp3fieldtype = M_SLS; }
    break;

  case 210:
#line 727 "grammar.y"
    { (yyval.blk).mtp3fieldtype = MH_SIO; }
    break;

  case 211:
#line 728 "grammar.y"
    { (yyval.blk).mtp3fieldtype = MH_OPC; }
    break;

  case 212:
#line 729 "grammar.y"
    { (yyval.blk).mtp3fieldtype = MH_DPC; }
    break;

  case 213:
#line 730 "grammar.y"
    { (yyval.blk).mtp3fieldtype = MH_SLS; }
    break;

  case 215:
#line 733 "grammar.y"
    { (yyval.blk).b = gen_mtp3field_code((yyvsp[(0) - (2)].blk).mtp3fieldtype, (u_int)(yyvsp[(2) - (2)].i), (u_int)(yyvsp[(1) - (2)].i), 0); }
    break;

  case 216:
#line 734 "grammar.y"
    { (yyval.blk).b = gen_mtp3field_code((yyvsp[(0) - (2)].blk).mtp3fieldtype, (u_int)(yyvsp[(2) - (2)].i), (u_int)(yyvsp[(1) - (2)].i), 1); }
    break;

  case 217:
#line 735 "grammar.y"
    { (yyval.blk).b = (yyvsp[(2) - (3)].blk).b; (yyval.blk).q = qerr; }
    break;

  case 218:
#line 737 "grammar.y"
    {
	(yyval.blk).mtp3fieldtype = (yyvsp[(0) - (1)].blk).mtp3fieldtype;
	if ((yyval.blk).mtp3fieldtype == M_SIO ||
	    (yyval.blk).mtp3fieldtype == M_OPC ||
	    (yyval.blk).mtp3fieldtype == M_DPC ||
	    (yyval.blk).mtp3fieldtype == M_SLS ||
	    (yyval.blk).mtp3fieldtype == MH_SIO ||
	    (yyval.blk).mtp3fieldtype == MH_OPC ||
	    (yyval.blk).mtp3fieldtype == MH_DPC ||
	    (yyval.blk).mtp3fieldtype == MH_SLS)
		(yyval.blk).b = gen_mtp3field_code((yyval.blk).mtp3fieldtype, (u_int) (yyvsp[(1) - (1)].i), BPF_JEQ, 0);
	}
    break;

  case 220:
#line 751 "grammar.y"
    { gen_or((yyvsp[(1) - (3)].blk).b, (yyvsp[(3) - (3)].blk).b); (yyval.blk) = (yyvsp[(3) - (3)].blk); }
    break;


/* Line 1267 of yacc.c.  */
#line 3403 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 753 "grammar.y"


