/* Definitions for data structures and routines for the regular
   expression library, version 0.12.

   Copyright (C) 1985, 1989, 1990, 1991, 1992, 1993 Free Software Foundation, Inc.

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#ifndef __REGEXP_LIBRARY_H__
#define __REGEXP_LIBRARY_H__

/* POSIX says that <sys/types.h> must be included (by the caller) before
   <regex.h>.  */

#ifdef VMS
/* VMS doesn't have `size_t' in <sys/types.h>, even though POSIX says it
   should be there.  */
#include <stddef.h>
#endif


/* The following bits are used to determine the regexp syntax we
   recognize.  The set/not-set meanings are chosen so that Emacs syntax
   remains the value 0.  The bits are given in alphabetical order, and
   the definitions shifted by one from the previous bit; thus, when we
   add or remove a bit, only one other definition need change.  */
typedef unsigned reg_syntax_t;

/* If this bit is not set, then \ inside a bracket expression is literal.
   If set, then such a \ quotes the following character.  */
#define RE_BACKSLASH_ESCAPE_IN_LISTS (1)

/* If this bit is not set, then + and ? are operators, and \+ and \? are
     literals.
   If set, then \+ and \? are operators and + and ? are literals.  */
#define RE_BK_PLUS_QM (RE_BACKSLASH_ESCAPE_IN_LISTS << 1)

/* If this bit is set, then character classes are supported.  They are:
     [:alpha:], [:upper:], [:lower:],  [:digit:], [:alnum:], [:xdigit:],
     [:space:], [:print:], [:punct:], [:graph:], and [:cntrl:].
   If not set, then character classes are not supported.  */
#define RE_CHAR_CLASSES (RE_BK_PLUS_QM << 1)

/* If this bit is set, then ^ and $ are always anchors (outside bracket
     expressions, of course).
   If this bit is not set, then it depends:
        ^  is an anchor if it is at the beginning of a regular
           expression or after an open-group or an alternation operator;
        $  is an anchor if it is at the end of a regular expression, or
           before a close-group or an alternation operator.

   This bit could be (re)combined with RE_CONTEXT_INDEP_OPS, because
   POSIX draft 11.2 says that * etc. in leading positions is undefined.
   We already implemented a previous draft which made those constructs
   invalid, though, so we haven't changed the code back.  */
#define RE_CONTEXT_INDEP_ANCHORS (RE_CHAR_CLASSES << 1)

/* If this bit is set, then special characters are always special
     regardless of where they are in the pattern.
   If this bit is not set, then special characters are special only in
     some contexts; otherwise they are ordinary.  Specifically,
     * + ? and intervals are only special when not after the beginning,
     open-group, or alternation operator.  */
#define RE_CONTEXT_INDEP_OPS (RE_CONTEXT_INDEP_ANCHORS << 1)

/* If this bit is set, then *, +, ?, and { cannot be first in an re or
     immediately after an alternation or begin-group operator.  */
#define RE_CONTEXT_INVALID_OPS (RE_CONTEXT_INDEP_OPS << 1)

/* If this bit is set, then . matches newline.
   If not set, then it doesn't.  */
#define RE_DOT_NEWLINE (RE_CONTEXT_INVALID_OPS << 1)

/* If this bit is set, then . doesn't match NUL.
   If not set, then it does.  */
#define RE_DOT_NOT_NULL (RE_DOT_NEWLINE << 1)

/* If this bit is set, nonmatching lists [^...] do not match newline.
   If not set, they do.  */
#define RE_HAT_LISTS_NOT_NEWLINE (RE_DOT_NOT_NULL << 1)

/* If this bit is set, either \{...\} or {...} defines an
     interval, depending on RE_NO_BK_BRACES.
   If not set, \{, \}, {, and } are literals.  */
#define RE_INTERVALS (RE_HAT_LISTS_NOT_NEWLINE << 1)

/* If this bit is set, +, ? and | aren't recognized as operators.
   If not set, they are.  */
#define RE_LIMITED_OPS (RE_INTERVALS << 1)

/* If this bit is set, newline is an alternation operator.
   If not set, newline is literal.  */
#define RE_NEWLINE_ALT (RE_LIMITED_OPS << 1)

/* If this bit is set, then `{...}' defines an interval, and \{ and \}
     are literals.
  If not set, then `\{...\}' defines an interval.  */
#define RE_NO_BK_BRACES (RE_NEWLINE_ALT << 1)

/* If this bit is set, (...) defines a group, and \( and \) are literals.
   If not set, \(...\) defines a group, and ( and ) are literals.  */
#define RE_NO_BK_PARENS (RE_NO_BK_BRACES << 1)

/* If this bit is set, then \<digit> matches <digit>.
   If not set, then \<digit> is a back-reference.  */
#define RE_NO_BK_REFS (RE_NO_BK_PARENS << 1)

/* If this bit is set, then | is an alternation operator, and \| is literal.
   If not set, then \| is an alternation operator, and | is literal.  */
#define RE_NO_BK_VBAR (RE_NO_BK_REFS << 1)

/* If this bit is set, then an ending range point collating higher
     than the starting range point, as in [z-a], is invalid.
   If not set, then when ending range point collates higher than the
     starting range point, the range is ignored.  */
#define RE_NO_EMPTY_RANGES (RE_NO_BK_VBAR << 1)

/* If this bit is set, then an unmatched ) is ordinary.
   If not set, then an unmatched ) is invalid.  */
#define RE_UNMATCHED_RIGHT_PAREN_ORD (RE_NO_EMPTY_RANGES << 1)

/* This global variable defines the particular regexp syntax to use (for
   some interfaces).  When a regexp is compiled, the syntax used is
   stored in the pattern buffer, so changing this does not affect
   already-compiled regexps.  */
extern reg_syntax_t re_syntax_options;

/* Define combinations of the above bits for the standard possibilities.
   (The [[[ comments delimit what gets put into the Texinfo file, so
   don't delete them!)  */
/* [[[begin syntaxes]]] */
#define RE_SYNTAX_EMACS 0

#define RE_SYNTAX_AWK                           \
  (RE_BACKSLASH_ESCAPE_IN_LISTS | RE_DOT_NOT_NULL           \
   | RE_NO_BK_PARENS            | RE_NO_BK_REFS             \
   | RE_NO_BK_VBAR               | RE_NO_EMPTY_RANGES           \
   | RE_UNMATCHED_RIGHT_PAREN_ORD)

#define RE_SYNTAX_POSIX_AWK                         \
  (RE_SYNTAX_POSIX_EXTENDED | RE_BACKSLASH_ESCAPE_IN_LISTS)

#define RE_SYNTAX_GREP                          \
  (RE_BK_PLUS_QM              | RE_CHAR_CLASSES             \
   | RE_HAT_LISTS_NOT_NEWLINE | RE_INTERVALS                \
   | RE_NEWLINE_ALT)

#define RE_SYNTAX_EGREP                         \
  (RE_CHAR_CLASSES        | RE_CONTEXT_INDEP_ANCHORS            \
   | RE_CONTEXT_INDEP_OPS | RE_HAT_LISTS_NOT_NEWLINE            \
   | RE_NEWLINE_ALT       | RE_NO_BK_PARENS             \
   | RE_NO_BK_VBAR)

#define RE_SYNTAX_POSIX_EGREP                       \
  (RE_SYNTAX_EGREP | RE_INTERVALS | RE_NO_BK_BRACES)

/* P1003.2/D11.2, section 4.20.7.1, lines 5078ff.  */
#define RE_SYNTAX_ED RE_SYNTAX_POSIX_BASIC

#define RE_SYNTAX_SED RE_SYNTAX_POSIX_BASIC

/* Syntax bits common to both basic and extended POSIX regex syntax.  */
#define _RE_SYNTAX_POSIX_COMMON                     \
  (RE_CHAR_CLASSES | RE_DOT_NEWLINE      | RE_DOT_NOT_NULL      \
   | RE_INTERVALS  | RE_NO_EMPTY_RANGES)

#define RE_SYNTAX_POSIX_BASIC                       \
  (_RE_SYNTAX_POSIX_COMMON | RE_BK_PLUS_QM)

/* Differs from ..._POSIX_BASIC only in that RE_BK_PLUS_QM becomes
   RE_LIMITED_OPS, i.e., \? \+ \| are not recognized.  Actually, this
   isn't minimal, since other operators, such as \`, aren't disabled.  */
#define RE_SYNTAX_POSIX_MINIMAL_BASIC                   \
  (_RE_SYNTAX_POSIX_COMMON | RE_LIMITED_OPS)

#define RE_SYNTAX_POSIX_EXTENDED                    \
  (_RE_SYNTAX_POSIX_COMMON | RE_CONTEXT_INDEP_ANCHORS           \
   | RE_CONTEXT_INDEP_OPS  | RE_NO_BK_BRACES                \
   | RE_NO_BK_PARENS       | RE_NO_BK_VBAR              \
   | RE_UNMATCHED_RIGHT_PAREN_ORD)

/* Differs from ..._POSIX_EXTENDED in that RE_CONTEXT_INVALID_OPS
   replaces RE_CONTEXT_INDEP_OPS and RE_NO_BK_REFS is added.  */
#define RE_SYNTAX_POSIX_MINIMAL_EXTENDED                \
  (_RE_SYNTAX_POSIX_COMMON  | RE_CONTEXT_INDEP_ANCHORS          \
   | RE_CONTEXT_INVALID_OPS | RE_NO_BK_BRACES               \
   | RE_NO_BK_PARENS        | RE_NO_BK_REFS             \
   | RE_NO_BK_VBAR      | RE_UNMATCHED_RIGHT_PAREN_ORD)
/* [[[end syntaxes]]] */

/* Maximum number of duplicates an interval can allow.  Some systems
   (erroneously) define this in other header files, but we want our
   value, so remove any previous define.  */
#ifdef RE_DUP_MAX
#undef RE_DUP_MAX
#endif
#define RE_DUP_MAX ((1 << 15) - 1)


/* POSIX `cflags' bits (i.e., information for `regcomp').  */

/* If this bit is set, then use extended regular expression syntax.
   If not set, then use basic regular expression syntax.  */
#define REG_EXTENDED 1

/* If this bit is set, then ignore case when matching.
   If not set, then case is significant.  */
#define REG_ICASE (REG_EXTENDED << 1)

/* If this bit is set, then anchors do not match at newline
     characters in the string.
   If not set, then anchors do match at newlines.  */
#define REG_NEWLINE (REG_ICASE << 1)

/* If this bit is set, then report only success or fail in regexec.
   If not set, then returns differ between not matching and errors.  */
#define REG_NOSUB (REG_NEWLINE << 1)


/* POSIX `eflags' bits (i.e., information for regexec).  */

/* If this bit is set, then the beginning-of-line operator doesn't match
     the beginning of the string (presumably because it's not the
     beginning of a line).
   If not set, then the beginning-of-line operator does match the
     beginning of the string.  */
#define REG_NOTBOL 1

/* Like REG_NOTBOL, except for the end-of-line.  */
#define REG_NOTEOL (1 << 1)


/* If any error codes are removed, changed, or added, update the
   `re_error_msg' table in regex.c.  */
typedef enum
{
  REG_NOERROR = 0,  /* Success.  */
  REG_NOMATCH,      /* Didn't find a match (for regexec).  */

  /* POSIX regcomp return error codes.  (In the order listed in the
     standard.)  */
  REG_BADPAT,       /* Invalid pattern.  */
  REG_ECOLLATE,     /* Not implemented.  */
  REG_ECTYPE,       /* Invalid character class name.  */
  REG_EESCAPE,      /* Trailing backslash.  */
  REG_ESUBREG,      /* Invalid back reference.  */
  REG_EBRACK,       /* Unmatched left bracket.  */
  REG_EPAREN,       /* Parenthesis imbalance.  */
  REG_EBRACE,       /* Unmatched \{.  */
  REG_BADBR,        /* Invalid contents of \{\}.  */
  REG_ERANGE,       /* Invalid range end.  */
  REG_ESPACE,       /* Ran out of memory.  */
  REG_BADRPT,       /* No preceding re for repetition op.  */

  /* Error codes we've added.  */
  REG_EEND,     /* Premature end.  */
  REG_ESIZE,        /* Compiled pattern bigger than 2^16 bytes.  */
  REG_ERPAREN       /* Unmatched ) or \); not returned from regcomp.  */
} reg_errcode_t;

/* This data structure represents a compiled pattern.  Before calling
   the pattern compiler, the fields `buffer', `allocated', `fastmap',
   `translate', and `no_sub' can be set.  After the pattern has been
   compiled, the `re_nsub' field is available.  All other fields are
   private to the regex routines.  */

struct re_pattern_buffer
{
/* [[[begin pattern_buffer]]] */
    /* Space that holds the compiled pattern.  It is declared as
          `unsigned char *' because its elements are
           sometimes used as array indexes.  */
  unsigned char *buffer;

    /* Number of bytes to which `buffer' points.  */
  unsigned long allocated;

    /* Number of bytes actually used in `buffer'.  */
  unsigned long used;

        /* Syntax setting with which the pattern was compiled.  */
  reg_syntax_t syntax;

        /* Pointer to a fastmap, if any, otherwise zero.  re_search uses
           the fastmap, if there is one, to skip over impossible
           starting points for matches.  */
  char *fastmap;

        /* Either a translate table to apply to all characters before
           comparing them, or zero for no translation.  The translation
           is applied to a pattern when it is compiled and to a string
           when it is matched.  */
  char *translate;

    /* Number of subexpressions found by the compiler.  */
  size_t re_nsub;

        /* Zero if this pattern cannot match the empty string, one else.
           Well, in truth it's used only in `re_search_2', to see
           whether or not we should use the fastmap, so we don't set
           this absolutely perfectly; see `re_compile_fastmap' (the
           `duplicate' case).  */
  unsigned can_be_null : 1;

        /* If REGS_UNALLOCATED, allocate space in the `regs' structure
             for `max (RE_NREGS, re_nsub + 1)' groups.
           If REGS_REALLOCATE, reallocate space if necessary.
           If REGS_FIXED, use what's there.  */
#define REGS_UNALLOCATED 0
#define REGS_REALLOCATE 1
#define REGS_FIXED 2
  unsigned regs_allocated : 2;

        /* Set to zero when `regex_compile' compiles a pattern; set to one
           by `re_compile_fastmap' if it updates the fastmap.  */
  unsigned fastmap_accurate : 1;

        /* If set, `re_match_2' does not return information about
           subexpressions.  */
  unsigned no_sub : 1;

        /* If set, a beginning-of-line anchor doesn't match at the
           beginning of the string.  */
  unsigned not_bol : 1;

        /* Similarly for an end-of-line anchor.  */
  unsigned not_eol : 1;

        /* If true, an anchor at a newline matches.  */
  unsigned newline_anchor : 1;

/* [[[end pattern_buffer]]] */
};

typedef struct re_pattern_buffer regex_t;


/* search.c (search_buffer) in Emacs needs this one opcode value.  It is
   defined both in `regex.c' and here.  */
#define RE_EXACTN_VALUE 1

/* Type for byte offsets within the string.  POSIX mandates this.  */
typedef int regoff_t;


/* This is the structure we store register match data in.  See
   regex.texinfo for a full description of what registers match.  */
struct re_registers
{
  unsigned num_regs;
  regoff_t *start;
  regoff_t *end;
};


/* If `regs_allocated' is REGS_UNALLOCATED in the pattern buffer,
   `re_match_2' returns information about at least this many registers
   the first time a `regs' structure is passed.  */
#ifndef RE_NREGS
#define RE_NREGS 30
#endif


/* POSIX specification for registers.  Aside from the different names than
   `re_registers', POSIX uses an array of structures, instead of a
   structure of arrays.  */
typedef struct
{
  regoff_t rm_so;  /* Byte offset from string's start to substring's start.  */
  regoff_t rm_eo;  /* Byte offset from string's start to substring's end.  */
} regmatch_t;

/* Declarations for routines.  */

/* To avoid duplicating every routine declaration -- once with a
   prototype (if we are ANSI), and once without (if we aren't) -- we
   use the following macro to declare argument types.  This
   unfortunately clutters up the declarations a bit, but I think it's
   worth it.  */

#if __STDC__

#define _RE_ARGS(args) args

#else /* not __STDC__ */

#define _RE_ARGS(args) ()

#endif /* not __STDC__ */

/* Sets the current default syntax to SYNTAX, and return the old syntax.
   You can also simply assign to the `re_syntax_options' variable.  */
extern reg_syntax_t re_set_syntax _RE_ARGS ((reg_syntax_t syntax));

/* Compile the regular expression PATTERN, with length LENGTH
   and syntax given by the global `re_syntax_options', into the buffer
   BUFFER.  Return NULL if successful, and an error string if not.  */
extern const char *re_compile_pattern
  _RE_ARGS ((const char *pattern, int length,
             struct re_pattern_buffer *buffer));


/* Compile a fastmap for the compiled pattern in BUFFER; used to
   accelerate searches.  Return 0 if successful and -2 if was an
   internal error.  */
extern int re_compile_fastmap _RE_ARGS ((struct re_pattern_buffer *buffer));


/* Search in the string STRING (with length LENGTH) for the pattern
   compiled into BUFFER.  Start searching at position START, for RANGE
   characters.  Return the starting position of the match, -1 for no
   match, or -2 for an internal error.  Also return register
   information in REGS (if REGS and BUFFER->no_sub are nonzero).  */
extern int re_search
  _RE_ARGS ((struct re_pattern_buffer *buffer, const char *string,
            int length, int start, int range, struct re_registers *regs));


/* Like `re_search', but search in the concatenation of STRING1 and
   STRING2.  Also, stop searching at index START + STOP.  */
extern int re_search_2
  _RE_ARGS ((struct re_pattern_buffer *buffer, const char *string1,
             int length1, const char *string2, int length2,
             int start, int range, struct re_registers *regs, int stop));


/* Like `re_search', but return how many characters in STRING the regexp
   in BUFFER matched, starting at position START.  */
extern int re_match
  _RE_ARGS ((struct re_pattern_buffer *buffer, const char *string,
             int length, int start, struct re_registers *regs));


/* Relates to `re_match' as `re_search_2' relates to `re_search'.  */
extern int re_match_2
  _RE_ARGS ((struct re_pattern_buffer *buffer, const char *string1,
             int length1, const char *string2, int length2,
             int start, struct re_registers *regs, int stop));


/* Set REGS to hold NUM_REGS registers, storing them in STARTS and
   ENDS.  Subsequent matches using BUFFER and REGS will use this memory
   for recording register information.  STARTS and ENDS must be
   allocated with malloc, and must each be at least `NUM_REGS * sizeof
   (regoff_t)' bytes long.

   If NUM_REGS == 0, then subsequent matches should allocate their own
   register data.

   Unless this function is called, the first search or match using
   PATTERN_BUFFER will allocate its own register data, without
   freeing the old data.  */
extern void re_set_registers
  _RE_ARGS ((struct re_pattern_buffer *buffer, struct re_registers *regs,
             unsigned num_regs, regoff_t *starts, regoff_t *ends));

/* 4.2 bsd compatibility.  */
extern char *re_comp _RE_ARGS ((const char *));
extern int re_exec _RE_ARGS ((const char *));

/* POSIX compatibility.  */
extern int regcomp _RE_ARGS ((regex_t *preg, const char *pattern, int cflags));
extern int regexec
  _RE_ARGS ((const regex_t *preg, const char *string, size_t nmatch,
             regmatch_t pmatch[], int eflags));
extern size_t regerror
  _RE_ARGS ((int errcode, const regex_t *preg, char *errbuf,
             size_t errbuf_size));
extern void regfree _RE_ARGS ((regex_t *preg));


/* isalpha etc. are used for the character classes.  */
#include <ctype.h>

#ifndef isascii
#define isascii(c) 1
#endif

#ifdef isblank
#define ISBLANK(c) (isascii (c) && isblank (c))
#else
#define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#endif
#ifdef isgraph
#define ISGRAPH(c) (isascii (c) && isgraph (c))
#else
#define ISGRAPH(c) (isascii (c) && isprint (c) && !isspace (c))
#endif

#define ISPRINT(c) (isascii (c) && isprint (c))
#define ISDIGIT(c) (isascii (c) && isdigit (c))
#define ISALNUM(c) (isascii (c) && isalnum (c))
#define ISALPHA(c) (isascii (c) && isalpha (c))
#define ISCNTRL(c) (isascii (c) && iscntrl (c))
#define ISLOWER(c) (isascii (c) && islower (c))
#define ISPUNCT(c) (isascii (c) && ispunct (c))
#define ISSPACE(c) (isascii (c) && isspace (c))
#define ISUPPER(c) (isascii (c) && isupper (c))
#define ISXDIGIT(c) (isascii (c) && isxdigit (c))

#ifndef NULL
#define NULL 0
#endif

/* We remove any previous definition of `SIGN_EXTEND_CHAR',
   since ours (we hope) works properly with all combinations of
   machines, compilers, `char' and `unsigned char' argument types.
   (Per Bothner suggested the basic approach.)  */
#undef SIGN_EXTEND_CHAR
#if __STDC__
#define SIGN_EXTEND_CHAR(c) ((signed char) (c))
#else  /* not __STDC__ */
/* As in Harbison and Steele.  */
#define SIGN_EXTEND_CHAR(c) ((((unsigned char) (c)) ^ 128) - 128)
#endif

/* Should we use malloc or alloca?  If REGEX_MALLOC is not defined, we
   use `alloca' instead of `malloc'.  This is because using malloc in
   re_search* or re_match* could cause memory leaks when C-g is used in
   Emacs; also, malloc is slower and causes storage fragmentation.  On
   the other hand, malloc is more portable, and easier to debug.

   Because we sometimes use alloca, some routines have to be macros,
   not functions -- `alloca'-allocated space disappears at the end of the
   function it is called in.  */

#ifdef REGEX_MALLOC

#define REGEX_ALLOCATE malloc
#define REGEX_REALLOCATE(source, osize, nsize) realloc (source, nsize)

#else /* not REGEX_MALLOC  */

/* Emacs already defines alloca, sometimes.  */
#ifndef alloca

/* Make alloca work the best possible way.  */
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not __GNUC__ */
#if HAVE_ALLOCA_H
#include <alloca.h>
#else /* not __GNUC__ or HAVE_ALLOCA_H */
#ifndef _AIX /* Already did AIX, up at the top.  */
char *alloca ();
#endif /* not _AIX */
#endif /* not HAVE_ALLOCA_H */
#endif /* not __GNUC__ */

#endif /* not alloca */

#define REGEX_ALLOCATE alloca

/* Assumes a `char *destination' variable.  */
#define REGEX_REALLOCATE(source, osize, nsize)              \
  (destination = (char *) alloca (nsize),               \
   bcopy (source, destination, osize),                  \
   destination)

#endif /* not REGEX_MALLOC */


/* True if `size1' is non-NULL and PTR is pointing anywhere inside
   `string1' or just past its end.  This works if PTR is NULL, which is
   a good thing.  */
#define FIRST_STRING_P(ptr)                     \
  (size1 && string1 <= (ptr) && (ptr) <= string1 + size1)

/* (Re)Allocate N items of type T using malloc, or fail.  */
#define TALLOC(n, t) ((t *) malloc ((n) * sizeof (t)))
#define RETALLOC(addr, n, t) ((addr) = (t *) realloc (addr, (n) * sizeof (t)))
#define REGEX_TALLOC(n, t) ((t *) REGEX_ALLOCATE ((n) * sizeof (t)))

#define BYTEWIDTH 8 /* In bits.  */

#define STREQ(s1, s2) ((strcmp (s1, s2) == 0))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef unsigned char boolean;
#define false 0
#define true 1


/* These are the command codes that appear in compiled regular
   expressions.  Some opcodes are followed by argument bytes.  A
   command code can specify any interpretation whatsoever for its
   arguments.  Zero bytes may appear in the compiled regular expression.

   The value of `exactn' is needed in search.c (search_buffer) in Emacs.
   So regex.h defines a symbol `RE_EXACTN_VALUE' to be 1; the value of
   `exactn' we use here must also be 1.  */

typedef enum
{
  no_op = 0,

        /* Followed by one byte giving n, then by n literal bytes.  */
  exactn = 1,

        /* Matches any (more or less) character.  */
  anychar,

        /* Matches any one char belonging to specified set.  First
           following byte is number of bitmap bytes.  Then come bytes
           for a bitmap saying which chars are in.  Bits in each byte
           are ordered low-bit-first.  A character is in the set if its
           bit is 1.  A character too large to have a bit in the map is
           automatically not in the set.  */
  charset,

        /* Same parameters as charset, but match any character that is
           not one of those specified.  */
  charset_not,

        /* Start remembering the text that is matched, for storing in a
           register.  Followed by one byte with the register number, in
           the range 0 to one less than the pattern buffer's re_nsub
           field.  Then followed by one byte with the number of groups
           inner to this one.  (This last has to be part of the
           start_memory only because we need it in the on_failure_jump
           of re_match_2.)  */
  start_memory,

        /* Stop remembering the text that is matched and store it in a
           memory register.  Followed by one byte with the register
           number, in the range 0 to one less than `re_nsub' in the
           pattern buffer, and one byte with the number of inner groups,
           just like `start_memory'.  (We need the number of inner
           groups here because we don't have any easy way of finding the
           corresponding start_memory when we're at a stop_memory.)  */
  stop_memory,

        /* Match a duplicate of something remembered. Followed by one
           byte containing the register number.  */
  duplicate,

        /* Fail unless at beginning of line.  */
  begline,

        /* Fail unless at end of line.  */
  endline,

        /* Succeeds if at beginning of buffer (if emacs) or at beginning
           of string to be matched (if not).  */
  begbuf,

        /* Analogously, for end of buffer/string.  */
  endbuf,

        /* Followed by two byte relative address to which to jump.  */
  jump,

    /* Same as jump, but marks the end of an alternative.  */
  jump_past_alt,

        /* Followed by two-byte relative address of place to resume at
           in case of failure.  */
  on_failure_jump,

        /* Like on_failure_jump, but pushes a placeholder instead of the
           current string position when executed.  */
  on_failure_keep_string_jump,

        /* Throw away latest failure point and then jump to following
           two-byte relative address.  */
  pop_failure_jump,

        /* Change to pop_failure_jump if know won't have to backtrack to
           match; otherwise change to jump.  This is used to jump
           back to the beginning of a repeat.  If what follows this jump
           clearly won't match what the repeat does, such that we can be
           sure that there is no use backtracking out of repetitions
           already matched, then we change it to a pop_failure_jump.
           Followed by two-byte address.  */
  maybe_pop_jump,

        /* Jump to following two-byte address, and push a dummy failure
           point. This failure point will be thrown away if an attempt
           is made to use it for a failure.  A `+' construct makes this
           before the first repeat.  Also used as an intermediary kind
           of jump when compiling an alternative.  */
  dummy_failure_jump,

    /* Push a dummy failure point and continue.  Used at the end of
       alternatives.  */
  push_dummy_failure,

        /* Followed by two-byte relative address and two-byte number n.
           After matching N times, jump to the address upon failure.  */
  succeed_n,

        /* Followed by two-byte relative address, and two-byte number n.
           Jump to the address N times, then fail.  */
  jump_n,

        /* Set the following two-byte relative address to the
           subsequent two-byte number.  The address *includes* the two
           bytes of number.  */
  set_number_at,

  wordchar, /* Matches any word-constituent character.  */
  notwordchar,  /* Matches any char that is not a word-constituent.  */

  wordbeg,  /* Succeeds if at word beginning.  */
  wordend,  /* Succeeds if at word end.  */

  wordbound,    /* Succeeds if at a word boundary.  */
  notwordbound  /* Succeeds if not at a word boundary.  */

#ifdef emacs
  ,before_dot,  /* Succeeds if before point.  */
  at_dot,   /* Succeeds if at point.  */
  after_dot,    /* Succeeds if after point.  */

    /* Matches any character whose syntax is specified.  Followed by
           a byte which contains a syntax code, e.g., Sword.  */
  syntaxspec,

    /* Matches any character whose syntax is not that specified.  */
  notsyntaxspec
#endif /* emacs */
} re_opcode_t;

/* But patterns can have more than `MAX_REGNUM' registers.  We just
   ignore the excess.  */
typedef unsigned regnum_t;


/* Macros for the compile stack.  */

/* Since offsets can go either forwards or backwards, this type needs to
   be able to hold values from -(MAX_BUF_SIZE - 1) to MAX_BUF_SIZE - 1.  */
typedef int pattern_offset_t;

typedef struct
{
  pattern_offset_t begalt_offset;
  pattern_offset_t fixup_alt_jump;
  pattern_offset_t inner_group_offset;
  pattern_offset_t laststart_offset;
  regnum_t regnum;
} compile_stack_elt_t;


typedef const unsigned char *fail_stack_elt_t;

typedef struct
{
  fail_stack_elt_t *stack;
  unsigned size;
  unsigned long long avail;           /* Offset of next open position.  */
} fail_stack_type;


typedef struct
{
  compile_stack_elt_t *stack;
  unsigned size;
  unsigned avail;           /* Offset of next open position.  */
} compile_stack_type;

/* Structure for per-register (a.k.a. per-group) information.
   This must not be longer than one word, because we push this value
   onto the failure stack.  Other register information, such as the
   starting and ending positions (which are addresses), and the list of
   inner groups (which is a bits list) are maintained in separate
   variables.

   We are making a (strictly speaking) nonportable assumption here: that
   the compiler will pack our bit fields into something that fits into
   the type of `word', i.e., is something that fits into one item on the
   failure stack.  */
typedef union
{
  fail_stack_elt_t word;
  struct
  {
      /* This field is one if this group can match the empty string,
         zero if not.  If not yet determined,  `MATCH_NULL_UNSET_VALUE'.  */
#define MATCH_NULL_UNSET_VALUE 3
    unsigned match_null_string_p : 2;
    unsigned is_active : 1;
    unsigned matched_something : 1;
    unsigned ever_matched_something : 1;
  } bits;
} register_info_type;


/* Subroutine declarations and macros for regex_compile.  */
static void
store_op1 (
    re_opcode_t op,
    unsigned char *loc,
    int arg);

static void
store_op2 (
    re_opcode_t op,
    unsigned char *loc,
    int arg1, int arg2);


static void
insert_op1 (
    re_opcode_t op,
    unsigned char *loc,
    int arg,
    unsigned char *end);


static void
insert_op2 (
    re_opcode_t op,
    unsigned char *loc,
    int arg1, int arg2,
    unsigned char *end);

static boolean
at_begline_loc_p (
    const char *pattern, const char *p,
    reg_syntax_t syntax);

static boolean
at_endline_loc_p (
    const char *p, const char *pend,
    int syntax);


static boolean
group_in_compile_stack (
    compile_stack_type compile_stack,
    regnum_t regnum);


static reg_errcode_t
compile_range (
    const char **p_ptr, const char *pend,
    char *translate,
    reg_syntax_t syntax,
    unsigned char *b);

static int
bcmp_translate (
    const unsigned char *s1, const char *s2,
    register int len,
    char *translate);

static boolean
alt_match_null_string_p (
    unsigned char *p,unsigned char *end,
    register_info_type *reg_info);

static boolean
common_op_match_null_string_p (
    unsigned char **p, unsigned char *end,
    register_info_type *reg_info);

static boolean
group_match_null_string_p (
    unsigned char **p, unsigned char *end,
    register_info_type *reg_info);


#endif /* not __REGEXP_LIBRARY_H__ */
