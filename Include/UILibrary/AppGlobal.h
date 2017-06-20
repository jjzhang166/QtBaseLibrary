#pragma once

#ifndef BUILD_USE_STATIC_UI
# if defined(UILIBRARY_LIB)
#  define UILIBRARY_EXPORT __declspec(dllexport)
# else
#  define UILIBRARY_EXPORT __declspec(dllimport)
# endif
#else
# define UILIBRARY_EXPORT
#endif