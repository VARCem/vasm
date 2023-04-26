/*
 * VASM		VARCem Multi-Target Assembler.
 *		A simple table-driven assembler for several 8-bit target
 *		devices, like the 6502, 6800, 80x, Z80 et al series. The
 *		code originated from Bernd B�ckmann's "asm6502" project.
 *
 *		This file is part of the VARCem Project.
 *
 *		Definitions for the entire application.
 *
 * Version:	@(#)global.h	1.0.7	2023/04/25
 *
 * Author:	Fred N. van Kempen, <waltje@varcem.com>
 *
 *		Copyright 2023 Fred N. van Kempen.
 *
 *		Redistribution and  use  in source  and binary forms, with
 *		or  without modification, are permitted  provided that the
 *		following conditions are met:
 *
 *		1. Redistributions of  source  code must retain the entire
 *		   above notice, this list of conditions and the following
 *		   disclaimer.
 *
 *		2. Redistributions in binary form must reproduce the above
 *		   copyright  notice,  this list  of  conditions  and  the
 *		   following disclaimer in  the documentation and/or other
 *		   materials provided with the distribution.
 *
 *		3. Neither the  name of the copyright holder nor the names
 *		   of  its  contributors may be used to endorse or promote
 *		   products  derived from  this  software without specific
 *		   prior written permission.
 *
 * THIS SOFTWARE  IS  PROVIDED BY THE  COPYRIGHT  HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY EXPRESS  OR  IMPLIED  WARRANTIES,  INCLUDING, BUT  NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE  ARE  DISCLAIMED. IN  NO  EVENT  SHALL THE COPYRIGHT
 * HOLDER OR  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES;  LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON  ANY
 * THEORY OF  LIABILITY, WHETHER IN  CONTRACT, STRICT  LIABILITY, OR  TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY  WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef GLOBAL_H
# define GLOBAL_H


#define COMMENT_CHAR	';'		// starts a comment (until EOL)
#define DOT_CHAR	'.'		// starts a directive or local label
#define EOF_CHAR	0x1a
#define EQUAL_CHAR	'='		// indicates variable assignment
#define COLON_CHAR	':'		// marks a label
#define ALPHA_CHAR	'@'		// marks a local label
#define FMT_B_CHAR	'['		// starts a format specifier
#define FMT_E_CHAR	']'		// ends a format specifier
#define FMT_BIN_CHAR	'%'		// specifies binary format
#define FMT_DEC_CHAR	'#'		// specifies decimal format
#define FMT_HEX_CHAR	'$'		// specifies hex format ($ style)
#define FMT_HEX1_CHAR	'x'		// specifies hex format (C style)
#define FMT_HEX2_CHAR	'X'		// specifies hex format (C style)

#define IS_EOL(c)	(((c) == EOF_CHAR) || ((c) == '\n') || ((c) == '\r'))
#define IS_END(c)	((!(c)) || IS_EOL((c)))
#define IS_SPACE(c)	(((c) == '\t') || ((c) == ' '))
#define IS_IDENT(c)	(((c) == DOT_CHAR) || ((c) == '_'))

#define MAX_FILENAMES	256 + 1		// maximum include files
#define MAX_IFLEVEL	16		// maximum depth of IF levels
#define RADIX_DEFAULT	10		// default radix is decimal


typedef enum errors_e {
    ERR_USER = 1,		// user-specified error
    ERR_ZERO,			// "division by zero"
    ERR_NOCPU,			// "processor type not set"
    ERR_CPU,			// "unknown processor type"
    ERR_MEM,			// "out of memory"
    ERR_NODIRECTIVE,		// "unknown directive"
    ERR_INSTR,			// "unknown instruction"
    ERR_NUM,			// "value expected"
    ERR_FMT,			// "invalid format specifier"
    ERR_EXPR,			// "error in expression"
    ERR_OPER,			// "incomplete operator"
    ERR_UNBALANCED,		// "unbalanced parentheses"
    ERR_ID,			// "identifier expected"
    ERR_IDLEN,			// "identifier length exceeded"
    ERR_STMT,			// "illegal statement"
    ERR_EOL,			// "end of line expected"
    ERR_REDEF,			// "illegal redefinition"
    ERR_IF,			// "IF nesting too deep"
    ERR_ELSE,			// "ELSE without IF"
    ERR_ENDIF,			// "ENDIF without IF"
    ERR_LBLREDEF,		// "symbol already defined as label"
    ERR_CLBR,			// "missing closing brace"
    ERR_UNDEF,			// "undefined value"
    ERR_ILLTYPE,		// "illegal type"
    ERR_STREND,			// "string not terminated"
    ERR_CHREND,			// "character constant not terminated"
    ERR_RNG,			// "value out of range"
    ERR_RNG_BYTE,		// "byte value out of range
    ERR_RNG_WORD,		// "word value out of range
    ERR_LOCAL_REDEF,		// "illegal redefinition of local label"
    ERR_NO_GLOBAL,		// "local label definition .. global label"
    ERR_CHR,			// "malformed character constant"
    ERR_STRLEN,			// "string too long"
    ERR_STR,			// "string expected"
    ERR_OPEN,			// "can not open file"
    ERR_MAXINC,			// "maximum number of include files reached"
    ERR_NO_FMT,			// "file format not enabled"

    ERR_MAXERR			// last generic error
} errors_t;


#define ID_LEN		32		// max #characters in identifiers
#define STR_LEN		128		// max #characters in string literals

#define MAXINT(a,b) (((b) >= (a)) ? (b) : (a))


/* The generic "value" type for variables and expressions. */
typedef struct value {
    uint32_t	v;
    uint8_t	t;
#define TYPE_NONE	0x00
#define TYPE_BYTE	0x01
#define TYPE_WORD	0x02
#define TYPE_DWORD	0x04
#define TYPE_MASK	0x0f
} value_t;

/* For the value-specific directives. */
#define VALUE_DEFINED 0x80
#define DEFINED(x) (((x).t & VALUE_DEFINED) != 0)
#define UNDEFINED(x) (((x).t & VALUE_DEFINED) == 0)
#define SET_DEFINED(v) ((v).t = ((v).t | VALUE_DEFINED))
#define SET_UNDEFINED(v) ((v).t = (v).t & TYPE_MASK);
#define INFER_DEFINED(a,b) \
		if (UNDEFINED(a) || UNDEFINED(b)) { SET_UNDEFINED(a); } \
		else { SET_DEFINED(a); }

/* For the type-specific directives. */
#define TYPE(v) ((v).t & TYPE_MASK)
#define SET_TYPE(v, u) ((v).t = ((v).t & VALUE_DEFINED) | (u))
#define NUM_TYPE(x) (((x) < 0x100) ? TYPE_BYTE : ((x) < 0x10000) ? TYPE_WORD : TYPE_DWORD)
#define INFER_TYPE(a,b) (((a).v >= 0x100) || ((b).v >= 0x100)) \
		? SET_TYPE((a), TYPE_WORD) \
		: SET_TYPE((a), MAXINT(TYPE(a),(TYPE(b))))


/* Data type for storing symbols (labels and variables.) */
typedef struct sym_ {
    char	name[ID_LEN];
    value_t	value;
    uint8_t	kind;			// is it a label or a variable?
#define KIND_LBL 1
#define KIND_VAR 2
    uint8_t	pass;			// defined in which pass?
    short	filenr;			// in which file was it defined?
    int		linenr;			// on what line in that file?
    struct sym_	*next;
    struct sym_	*locals;		// local subdefinitions
} symbol_t;

/* Symbol-specific directives. */
#define IS_LBL(x) (((x)->kind & KIND_LBL) != 0)
#define IS_VAR(x) (((x)->kind & KIND_VAR) != 0)

struct pseudo;


/* Global variables. */
extern int	opt_d,
		opt_C,
		opt_F,
		opt_P,
		opt_q,
		opt_v;
extern char	myname[],
		version[];

extern uint16_t	pc;
extern int	line,
		newline;
extern symbol_t	*current_label;
extern int8_t	org_done;
extern int8_t	radix;
extern int	iflevel,
		ifstate,
		newifstate,
		ifstack[];
extern char	*filenames[];
extern int	filelines[];
extern int	filenames_idx,
		filenames_len;

extern char	*text;
extern int	text_len;

extern uint8_t	*code;
extern uint32_t	code_base;
extern uint16_t	oc,
		sa;

extern int	list_plength,
		list_pwidth;


#if defined(_WIN32) && defined(_MSC_VER)
# define strcasecmp _stricmp
#endif


/* Functions. */
#ifdef _DEBUG
extern char	*dumpline(const char *p);
#endif
extern int	pass(char **, int);
extern void	error(int, const char *);
extern int	is_end(char);
extern void	skip_eol(char **);
extern void	skip_white(char **);
extern void	skip_white_and_comment(char **);
extern void	skip_curr_and_white(char **);
extern void	ident(char **, char *);
extern void	upcase(char **, char *);
extern void	ident_upcase(char **, char *);
extern void	nident(char **, char *);
extern void	nident_upcase(char **, char *);

extern symbol_t	*sym_table(void);
extern char	sym_type(const symbol_t *);
extern symbol_t	*sym_lookup(const char *, symbol_t **);
extern void	sym_free(symbol_t **);
extern symbol_t	*sym_aquire(const char *, symbol_t **);
extern symbol_t	*define_label(const char *, uint16_t, symbol_t *);
extern void	define_variable(const char *, value_t);

extern value_t	expr(char **);
extern value_t	to_byte(value_t, int);
extern value_t	to_word(value_t, int);
extern char	value_type(uint8_t);
extern int	value_format(char **);
extern char	*value_print(value_t);
extern char	*value_print_format(value_t, int);

extern value_t	function(const char *, char **);

extern size_t	file_size(const char *);
extern int	file_read_buf(const char *, char *);
extern int	file_read(const char *, char **, size_t *);
extern void	file_add(const char *, int, const char *, size_t);

extern void	emit_addr(uint32_t);
extern int	emit_str(const char *, int, int);
extern int	emit_byte(uint8_t, int);
extern int	emit_word(uint16_t, int);
extern int	emit_dword(uint32_t, int);
extern int	save_code(const char *, const uint8_t *, int);

extern void	list_set_head(const char *);
extern void	list_set_syms(int);
extern int	list_init(const char *);
extern void	list_close(void);
extern void	list_line(const char *);
extern void	list_page(const char *);
extern void	list_save(uint16_t, uint16_t);
extern void	list_symbols(void);

extern const struct pseudo	*is_pseudo(const char *);
extern char	*pseudo(const struct pseudo *, char **, int);

extern int	set_cpu(const char *, int);


#endif	/*GLOBAL_H*/
