#ifndef PREPROCESSORS_H
#define PREPROCESSORS_H

#ifdef _MSC_VER
#define noexcept
#endif

#ifndef __GNUC__
#define and	&&
#define and_eq &=
#define bitand &
#define bitor |
#define compl ~
#define not	!
#define not_eq !=
#define or ||
#define or_eq |=
#define xor	^
#define xor_eq ^=
#endif

#define is ==
#define is_not !=

#define SAFE_DELETE(x)  \
  if (x is_not nullptr) \
  {                     \
    delete x;           \
    x = nullptr;        \
  }

#define SAFE_RELEASE(x)  \
  if (x is_not nullptr) \
  {                     \
    x->Release();           \
    x = nullptr;        \
  }

#endif