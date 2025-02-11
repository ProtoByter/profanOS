#ifndef TYPE_H
#define TYPE_H

typedef struct {
    int seconds;
    int minutes;
    int hours;
    int day_of_week;
    int day_of_month;
    int month;
    int year;
    int full[6];
} time_t;

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define NULL 0

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Math (from stdlib)
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef struct _div_t {
    int quot;
    int rem;
} div_t;

typedef struct _ldiv_t {
    long quot;
    long rem;
} ldiv_t;

typedef struct _lldiv_t {
    long long quot;
    long long rem;
} lldiv_t;

struct drand48_data {
    unsigned short __x[3]; /* Current state.  */
    unsigned short __old_x[3]; /* Old state.  */
    unsigned short __c; /* Additive const. in congruential formula.  */
    unsigned short __init; /* Flag for initializing.  */
    unsigned long long __a; /* Factor in congruential formula.  */
};

// more things for stdlib :
typedef unsigned int size_t;
typedef unsigned short wchar_t;

typedef struct _Mbstatet
{ // state of a multibyte translation
    unsigned long _Wchar;
    unsigned short _Byte, _State;
} _Mbstatet;

typedef _Mbstatet mbstate_t;

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int_least8_t;
typedef short              int_least16_t;
typedef int                int_least32_t;
typedef long long          int_least64_t;
typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char        int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef long long          int_fast64_t;
typedef unsigned char      uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef void (*oefuncp)(int, void *);  /* on_exit function pointer */
typedef int (*__compar_fn_t)(const void *, const void *);
typedef int (*__compar_d_fn_t)(const void *, const void *);

struct random_data {
    int32_t *fptr;      /* Front pointer.  */
    int32_t *rptr;      /* Rear pointer.  */
    int32_t *state;     /* Array of state values.  */
    int rand_type;      /* Type of random number generator.  */
    int rand_deg;       /* Degree of random number generator.  */
    int rand_sep;       /* Distance between front and rear.  */
    int32_t *end_ptr;   /* Pointer behind state table.  */
};

typedef void *locale_t; // PLACEHOLDER, //TODO : implement locale_t

#define Wvoid void
#define Wchar char
typedef unsigned char __string_uchar_t;
#define Wuchar __string_uchar_t
#define Wint int
typedef int            errno_t;
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
typedef long           __time32_t;
typedef size_t rsize_t;

typedef struct FILE {
    char *filename;
    char *path;
    char *name;
    char *mode;
    char *buffer;
    unsigned int buffer_size;
    unsigned int buffer_pos;
    int eof;
    int error;
    int is_temp;
} FILE;

typedef struct fpos_t {
    
} fpos_t;

// typedef of byte and sbyte
typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned long DWORD, *PDWORD, *LPDWORD;

typedef int bool;

#endif
