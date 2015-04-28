#ifndef PREPROCESSORS_H
#define PREPROCESSORS_H

#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

#ifndef __GNUC__
#define and &&
#define and_eq &=
#define bitand &
#define bitor |
#define compl ~
#define not !
#define not_eq !=
#define or ||
#define or_eq |=
#define xor ^
#define xor_eq ^=
#endif

#define IS ==
#define IS_NOT !=

#define BASIC_INPUT_LAYOUT_SIZE 6

#define SAFE_RELEASE(x) {if(x){(x)->Release();(x)=nullptr;}}
#define SAFE_DELETE(x) {if(x){delete (x);(x)=nullptr;}}
#define SAFE_DELETE_ARRAY(x) {if(x){delete[] (x);(x)=nullptr;}}

#endif
