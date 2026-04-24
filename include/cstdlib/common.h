#pragma once

#include <cstdlib/primitives.h>

#define EXPAND(...) $0$EXPAND($0$EXPAND($0$EXPAND($0$EXPAND(__VA_ARGS__))))
#define $0$EXPAND(...) $1$EXPAND($1$EXPAND($1$EXPAND($1$EXPAND(__VA_ARGS__))))
#define $1$EXPAND(...) $2$EXPAND($2$EXPAND($2$EXPAND($2$EXPAND(__VA_ARGS__))))
#define $2$EXPAND(...) $3$EXPAND($3$EXPAND($3$EXPAND($3$EXPAND(__VA_ARGS__))))
#define $3$EXPAND(...) __VA_ARGS__

#define PARENS ()

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND($0$FOR_EACH(macro, __VA_ARGS__)))
#define $0$FOR_EACH(macro, a, ...) macro(a) __VA_OPT__($1$FOR_EACH PARENS (macro, __VA_ARGS__))
#define $1$FOR_EACH() $0$FOR_EACH

#define BIT(x) (1 << (x))

#define INLINE inline __attribute__((always_inline))
