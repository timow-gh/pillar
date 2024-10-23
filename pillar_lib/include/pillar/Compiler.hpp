#ifndef PILLAR_COMPILER_HPP
#define PILLAR_COMPILER_HPP

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_attribute(noreturn)
#define PILLAR_NORETURN __attribute__((noreturn))
#else
#define PILLAR_NORETURN
#endif

#if __has_attribute(packed)
#define PILLAR_PACKED __attribute__((packed))
#else
#define PILLAR_PACKED
#endif

#if __has_builtin(__builtin_expect)
#define PILLAR_LIKELY(exp) (__builtin_expect(!!(exp), true))
#define PILLAR_UNLIKELY(exp) (__builtin_expect(!!(exp), false))
#else
#define PILLAR_LIKELY(exp) (!!(exp))
#define PILLAR_UNLIKELY(exp) (!!(exp))
#endif

#if __has_builtin(__builtin_prefetch)
#define PILLAR_PREFETCH(exp) (__builtin_prefetch(exp))
#else
#define PILLAR_PREFETCH(exp)
#endif

#if __has_builtin(__builtin_assume)
#define PILLAR_ASSUME(exp) (__builtin_assume(exp))
#else
#define UTILS_ASSUME(exp)
#endif

#if __has_attribute(always_inline)
#define PILLAR_ALWAYS_INLINE __attribute__((always_inline))
#else
#define PILLAR_ALWAYS_INLINE
#endif

#if __has_attribute(noinline)
#define PILLAR_NOINLINE __attribute__((noinline))
#else
#define PILLAR_NOINLINE
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1900
#define PILLAR_RESTRICT __restrict
#elif (defined(__clang__) || defined(__GNUC__))
#define PILLAR_RESTRICT __restrict__
#else
#define PILLAR_RESTRICT
#endif

#endif // PILLAR_COMPILER_HPP
