# Cross-Platform: Executable Path Name from Self or PID
```c
// Pseudocode for current executable's path:
#include <__getexecname/internal.h>
const char *internal = __getexecname(void);
```

```c
// Pseudocode for executable path from PID:
#include <__getexecname/external.h>
const char *external = __getexecname(int64_t pid = -1);
```

```c
// Pseudocode for current executable's name:
#include <__getprogname/internal.h>
const char *internal = __getprogname(void);
```

```c
// Pseudocode for executable name from PID:
#include <__getprogname/external.h>
const char *external = __getprogname(int64_t pid = -1);
```

`__getexecname()` is a reimplementation of the Solaris and illumos `getexecname()` function for a wide variety of platforms. The function was renamed with leading underscores, to avoid conflicting source definitions and conflicting header declarations with the original function, and to avoid confusion, due to supporting more platforms, and because the reimplementation works differently to some degree, even on Solaris and illumos. 

It supports Windows, macOS, Linux, FreeBSD, DragonFly BSD, NetBSD, OpenBSD, Solaris, illumos, Haiku, QNX Neutrino, and Android. iOS, iPadOS, tvOS, watchOS, and visionOS are supported platforms as well, though to a limited extent, because only getting the executable path name from the current process is supported on those platforms, and not the executable path name of an external PID; iOS, iPadOS, tvOS, watchOS, and visionOS are the only targets with this issue.

If `__getexecname/internal.h` is included to provide the function, the function returns the executable path name of the current process. If `__getexecname/external.h` is included to provide the function, the function is given an optional PID argument for returning an executable path name based on the given PID. If the PID argument is equal to negative one, or omitted completely, the executable path name to the current process is returned.

If on Windows, the function's C-string return value is limited to a byte length of `MAX_PATH`, which is `260` bytes. On Unix-likes, the function's C-string return value is limited to a byte length of `PATH_MAX`, which is a number of bytes that varies and is platform-specific. If the expected executable path name requires more bytes than what is allowable for the current platform, the function will fail and return a null pointer.

The return value, whenever it succeeds, and is not equal to a null pointer, it is guaranteed to stay the same value for the specified process, whether for the current process, or an external PID. If the associated executable file was moved or deleted from its original location, at any point in time, between the time the executable file spawned its process, until the time of this function being called, the function will fail and return a null pointer. 

When returning the path name to the current executable file, it is recommended to call this function at the very beginning of your program, and save its return value into a global string for later use. If the first time you call the function is not at the very beginning of your program, that allows the executable to potentially be moved or deleted first, thus causing the function to fail and return a null pointer, which is undesirable behavior.

On Solaris and illumos, there are certain conditions where you may need a process filesystem mounted in the current session in order for the function to not fail and return a null pointer. Linux requires the process filesystem mounted in the current session for this function to not fail and return a null pointer, under all circumstances. 

OpenBSD relies on `libkvm` calls to make an attempt at guessing the executable path name, and be aware this platform has the most failure cases, where the function will return a null poiner; OpenBSD is the least reliable platform, due to not having a platform-provided API, thus causing the need to write one ourselves using workarounds.

The OpenBSD function verifies the executable path names it guesses, by checking the `ino_t` and `dev_t` and seeing if they match up with the ones provided by `libkvm`, and this will guarantee, whenever the function succeeds, and does not return a null pointer, it will return a valid hard link to the correct executable file, without erroneous results. 

If the executable file has mulitple hard links pointing to it on disk, and `argv[0]` was modified to be a different hard link than the location that spawned the process, and that hard link still points to the same file on disk that the process spawned from, the path name returned could be one different than the path name the process spawned from.

On all platforms, when successful, the executable path name returned by this function is a case-sensitive, absolute, and normalized path name, with no dot, no dot-dot, and no consecutive path separators; on Windows, backward slashes are used for all path separators, where on Unix-likes, forward slashes are used. All symbolic links to the executable path name are resolved, and the process's executable path name will be guaranteed to end in a null terminator.

`__getprogname()` follows the same exact code logic as `__getexecname()`, except instead of returning the absolute path name to the executable file, it only returns the executable file's base name when successful. `__getprogname()` is inspired by the `getprogname()` function that first appeared in NetBSD 1.6, and is available on most other modern Unix-likes, even macOS. 

As the documentation for the original `getprogname()` function points out, some platforms set the return value of `getprogname()` automatically to be the base name of the current executable file, and it sets this return value on process startup, before the `main()` function is called.

However, this behavior can not be relied on portably, because some platforms do not have any string return value set for the function, at least not until after a call to `setprogname()` is made, to set that string to a valid base name, and something besides an empty string. If a relative or absolute path name is used instead of a base name for the argument of `setprogname()`, only the base name contained within that path name will be used for setting the return value of `getprogname()`. 

For platforms that do not have a default string return value set, for `getprogname()` on startup, before the call to `main()`, one may call the `__getprogname()` function, on the platforms it supports, to retrieve the current executable file's base name, and that can then be passed to `setprogname()` for use with further calls to `getprogname()`.

Calls to `__getprogname()` are slow, due to relying on the same underlying code logic as `__getexecname()`, so saving its return value with `setprogname()`, on platforms where it is possible, and then later calling `getprogname()` to retrieve that return value, that instead is preferred over calling `__getprogname()` multiple times within the life of your program. 

Alternatively, for platforms which do not have a built-in `setprogname()` and `getprogname()` function, copying the return value of `__getprogname()` to a global string is preferable for later use. When including `__getprogname/external.h` and `__getexecname/external.h`, passing a PID argument to the `__getprogname()` and `__getexecname()` functions is particularly useful when writing process monitoring software.
