#ifndef PILLAR_ASSERT_HPP
#define PILLAR_ASSERT_HPP

#include "pillar/compiler.hpp"

// Check if we're on windows and have access to __debugbreak().
#if !defined(PILLAR_DEBUGBREAK)
#if defined(_WIN32) && defined(__has_include)
#if __has_include(<intrin.h>)
#define PILLAR_DEBUGBREAK 1
#endif
#endif
#endif

#if !defined(PILLAR_DEBUGBREAK)
#define PILLAR_DEBUGBREAK 0
#endif

// Check if we're on a compiler that supports __builtin_trap().
#if !defined(PILLAR_BUILTIN_TRAP)
#if defined(__GNUC__) || defined(__clang__)
#define PILLAR_BUILTIN_TRAP 1
#else
#define PILLAR_BUILTIN_TRAP 0
#endif
#endif

#if !defined(PILLAR_BUILTIN_TRAP)
#define PILLAR_BUILTIN_TRAP 0
#endif

#if PILLAR_DEBUGBREAK
#include <intrin.h> // Provides access to the __debugbreak() function on windows.
#define PILLAR_ASSERT_TRAP() ::__debugbreak()
#elif PILLAR_BUILTIN_TRAP
#define PILLAR_ASSERT_TRAP() __builtin_trap()
#else
#define PILLAR_ASSERT_TRAP() ::std::abort()
#endif

namespace pillar
{

void print_assertion(const char* fileName, int line, const char* funcName, const char* message);

inline bool assertion(bool condition, const char* fileName, int line, const char* funcName, const char* message)
{
  if (PILLAR_UNLIKELY(condition))
  {
    print_assertion(fileName, line, funcName, message);
    PILLAR_ASSERT_TRAP();
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace pillar

#if defined(NDEBUG) && NDEBUG
#define PILLAR_ASSERT(...) (!(__VA_ARGS__))
#else
#define PILLAR_ASSERT(...) assertion(!(__VA_ARGS__), __FILE__, __LINE__, __func__, #__VA_ARGS__)
#endif

#endif // PILLAR_ASSERT_HPP
