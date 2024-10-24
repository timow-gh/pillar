#include "pillar/Assert.hpp"
#include <cstdio>

namespace pillar
{

void assertion(const char* fileName, int line, const char* funcName, const char* message) {
  std::fprintf(stderr, "%s:%d: internal check failed in '%s': '%s'\n", fileName, line, funcName, message);
}

} // namespace core
