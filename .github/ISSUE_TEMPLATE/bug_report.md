**LateralGM / RadialGM Bugs**
If your bug relates specifically to the IDE please file it on the appropriate tracker.

**Describe the bug**
A clear and concise description of what the bug is.

**Steps To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'

**Parse & Compiler Bugs**
If you have a parsing or syntax error please upload the build full log from LateralGM or emake as well as any GML snippet causing it. Some common / known issues include:
* Variables / Resource names that conflict with c/c++ namespace. Example: ```var time = 0;``` where time is C keyword.
* Resources with duplicate names.

** Crashes**
In the event of a crash or segfault please post a backtrace from gdb if possible. To do this: 
1: Build you game in debug mode 
2: In you terminal (mysys or otherwise) run gdb /path/to/game.exe
3: type "r" in gdb console and get the game to crash
4: type "bt" at the gdb prompt
6: Include the output of the above commands in your issue.

**Expected behavior**
A clear and concise description of what you expected to happen.

**Game files & Screenshots**
If applicable, upload a a simple game file and/or screenshots demonstrating the issue. Please upload any files directly to the GitHub issue so the links do not die,

**ENIGMA Configuration:**
* The OS you're on (Windows, Linux, etc)
* If possible please test your issue against multiple systems / configurations. The configuration can be changed in the "enigma settings" panel inside LateralGM or through flags passed to emake.
* If your issue is specific to one system. eg: SDL or OpenAL

**Additional context**
Add any other context about the problem here.
