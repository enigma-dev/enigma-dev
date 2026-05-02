# Cross-Platform: Executable Path Name from Self or PID
```c
// Pseudocode for current executable's path:
#include <__getexecname/internal.h>
const char *internal = __getexecname(void);
```

```c
// Pseudocode for executable path from PID:
#include <__getexecname/external.h>
const char *external = __getexecname(int pid = -1);
```

`__getexecname()` is a reimplementation of the Solaris and illumos `getexecname()` function for a wide variety of platforms. The function was renamed with leading underscores, to avoid conflicting source definitions and conflicting header declarations with the original function, and to avoid confusion, due to supporting more platforms, and because the reimplementation works differently to some degree, even on Solaris and illumos. 

It supports Windows, macOS, Linux, FreeBSD, DragonFly BSD, NetBSD, OpenBSD, Solaris, illumos, and Android. iOS, iPadOS, tvOS, watchOS, and visionOS are supported platforms as well, though to a limited extent, because only getting the executable path name from the current process is supported on those particular platforms, and not the executable path name of an external PID; iOS, iPadOS, tvOS, watchOS, and visionOS are the only platforms with this limitation.

If `__getexecname/internal.h` is included to provide the function, the function returns the executable path name of the current process. If `__getexecname/external.h` is included to provide the function, the function is given optional PID argument for returning an executable path name based on the given PID. If PID argument is equal to negative one, or omitted completely, the executable path name to the current process is returned.

If on Windows, the function's C-string return value is limited to a byte length of `MAX_PATH`, which is 260 bytes. On Unix-likes, the function's C-string return value is limited to a byte length of `PATH_MAX`, which is a number of bytes that varies and is platform-specific. If the expected executable path name requires more bytes than what is allowable for the current platorm, the function will fail and return a null pointer.

The return value of the function is static and persistent. The return value does not change beyond the first time it was called by your program, for the life time of the calling process. If the associated executable file was moved or deleted from its original location, at any point in time, between the time it spawned its process, until the time of the first call to this function, the function will fail and return a null pointer. 

When returning the path name to the current executable, it is recommended to call this function at the very beginning of your program, and save its return value into a global for later use. If the first time you call the function is not at the very beginning of your program, that allows the executable to potentially be moved or deleted first, thus causing the function to fail and return a null pointer, which is undesirable behavior.

On Solaris and illumos, there are certain conditions where you may need a process filesystem mounted in the current session in order for the function to not fail and return a null pointer. Linux requires the process filesystem mounted in the current session for this function to not fail and return a null pointer, under all circumstances. 

OpenBSD relies on `libkvm` calls to make an attempt at guessing the executable path name, and be aware this platform has the most failure cases, where the function will return a null poiner; OpenBSD is the least reliable platform, due to not having a platform-provided API, thus causing the need to write one ourselves using workarounds.

The OpenBSD function verifies the executable path names it guesses, by checking the `ino_t` and `dev_t` and seeing if they match up with the ones provided by `libkvm`, and this will guarantee, whenever the function succeeds, and does not return a null pointer, it will return a valid hard link to the correct executable file, without erroneous results. 

If the executable file has mulitple hard links pointing to it on disk, and `argv[0]` was modified to be a different hard link than the location that spawned the process, and that hard link still points to the same file on disk that the process spawned from, the path name returned could be one different than the path name the process spawned from.
