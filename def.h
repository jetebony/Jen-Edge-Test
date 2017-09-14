#ifndef DEF_H
#define DEF_H

#if LIBRARY_BUILD
#   define LIBRARY_EXPORT __attribute__((__visibility__("default")))
#   define LIBRARY_LOCAL  __attribute__((__visibility__("hidden")))
#else
#   define LIBRARY_EXPORT
#   define LIBRARY_LOCAL
#endif

#endif // DEF_H

