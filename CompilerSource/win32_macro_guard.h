/** Copyright (C) 2026 ENIGMA Development Environment contributors.
 ***
 *** Undefine Windows SDK macros that collide with ENIGMA/JDI identifiers
 *** (e.g. enum class NodeType::ERROR). Include after <windows.h> or before
 *** headers that declare those names.
 **/

#ifndef ENIGMA_WIN32_MACRO_GUARD_H
#define ENIGMA_WIN32_MACRO_GUARD_H

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

#ifdef ERROR
#undef ERROR
#endif
#ifdef DELETE
#undef DELETE
#endif
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif
#ifdef CONST
#undef CONST
#endif

#endif  // Windows family

#endif  // ENIGMA_WIN32_MACRO_GUARD_H
