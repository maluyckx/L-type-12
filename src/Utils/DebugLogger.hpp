#ifndef LTYPE_UTILS_DEBUGLOGGER_H
#define LTYPE_UTILS_DEBUGLOGGER_H

#ifndef DEBUG
#define DEBUGMSG(msg)
#define DEBUGERR(errmsg)
#else

#include <iostream>

/// Log a message, removed when compiled in release mode
#define DEBUGMSG(msg) do { std::cerr << "LOG: " << msg << std::endl; } while (0)
/// Log an error message, removed when compiled in release mode
#define DEBUGERR(errmsg) do { std::cerr <<"ERROR: "<< errmsg << std::endl; } while (0)
#endif

#endif
