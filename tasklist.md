This is a list of current tasks needing completed or that have been completed.

#Needs implemented

- [ ] Branch the draw_SHAPE functions into sets of outline_SHAPE and fill_SHAPE functions to eliminate* `bool outline` (concerning SHELL/Graphics_Systems/OpenGL/GSstdraw.h). Alternative: draw_SHAPE_outline/draw_SHAPE_fill. (* Not really eliminate, just replace the need for. The outline argument would become optional)
- [ ] Change the tabs in the Makefiles to spaces that actually line up properly.  The beginnings of lines require tabs, but the commands themselves should use spaces to align actual characters - otherwise, it looks weird depending on the terminal that you're using.
- [x] Lay out a super class for ds_ structures. It should implement only essentials (which may be none) to store and iterate through all created data structures. The ds_ functions should blindly cast from ds_parent to its children (ie, ds_list) except in debug mode. (- [ ]ifdef DEBUG_MODE)
- [ ] Go over the other resource-access-related functions adding anti-segfault checks in - [ ]ifdef DEBUG_MODE directives.
- [ ] Allow forcing 32-bit or 64-bit building of the game. There may be issues with Rebuilding that need to be corrected in order for all object files to be built in the correct architecture.
- [ ] Implement an attribute in [[compiler.ey]] files for defining additional binaries (for resource linker at very least)
- [x] Implement tiles using [[GL lists]]
- [ ] Implement DirectSound and DirectX, somehow (This is hard because MinGW doesn't get along with Microsoft's exceptionally propriety headers)
- [ ] Improve the instance system; add a second set of double links to iterators to allow for smooth updating of destroyed instances
- [ ] Finish implementing the DEBUG_MODE variable (var read checks, more access violation checking)
- [x] Implement new graphics system using modern GL calls
- [ ] Pretty-printers need written for each supported language.
- [ ] Complete the [[Extension system]]. The extension system needs spread to each system. The extension system should provide a way of "hooking" the load system to add support for more resource formats.

#Odds and ends

This is a list of bits and pieces that need to be done or aren't working
- [ ] make cursor sprite change when hovering over resizeable window border 
- [x] screen_refresh should be placed in screen_redraw as this is what GM does
- [ ] d3d - shapes need normals adding to them, d3d_set_zwriteable, d3d_set_depth, d3d_save, d3d_load needs to be rewritten properly not using the file_text functions; also the function can be overloaded to add support for other model formats  (ENIGMAsystem\SHELL\Graphics_Systems\OpenGL\GSd3d.cpp)
- [ ] game setting functions need to be added and implemented properly. At the moment LGM has a bug hindering their implementation: https://github.com/IsmAvatar/LateralGM/issues/23  (CompilerSource\compiler\components\write_defragged_events.cpp)
- [ ] Allow client to specify audio format and rate (ENIGMAsystem\SHELL\Audio_Systems\audio_mandatory.h)
- [ ] Add fastcall support to FFI  (ENIGMAsystem\Additional\Windows\include\ffitarget.h)
- [ ] When ENIGMA generates configuration files support endian.h for drawing efficiency  (ENIGMAsystem\SHELL\Graphics_Systems\OpenGL\GSstdraw.cpp)
- [ ] sprite_create_from_surface - toggling precise for generated sprite  (ENIGMAsystem\SHELL\Graphics_Systems\OpenGL\GSsurface.cpp)
- [ ] organization of ENIGMA namespace  (ENIGMAsystem\SHELL\Platforms\Win32\WINDOWSmain.cpp)
- [ ] implement map_resource_ids function  (ENIGMAsystem\SHELL\Universal_System\resource_data.cpp)
- [ ] Look at why the width of the space glyph is not always provided  (ENIGMAsystem\SHELL\Graphics_Systems\OpenGL\GSfont.cpp)
- [ ] reimplement untexture macro as function  (ENIGMAsystem\SHELL\Platforms\Win32\WINDOWSfonts.cpp)
- [ ] sort out GAME_GLOBALS.h  (ENIGMAsystem\SHELL\Universal_System\GAME_GLOBALS.h)
- [ ] move show_message function  (ENIGMAsystem\SHELL\Platforms\xlib\XLIBdialog.cpp)
- [ ] move some draw functions from WINDOWSstd  (ENIGMAsystem\SHELL\Platforms\Win32\WINDOWSstd.cpp)

#Compatibility

This is a list of things needed for GM compatibility 

- [ ] The remaining GM functions -> Unimplemented GM Function Page
- [x] A few GM variables/arrays also need to be added still -> Unimplemented GM variables/arrays page
- [ ] Event inheritance
- [ ] Timelines
- [ ] File Includes
- [ ] Constants
- [ ] Game Settings
- [ ] Triggers
- [ ] GM Extensions
- [ ] There are some DLL incompatibilities that need to be worked out
- [x] Windows widget system needs to work again
- [ ] C++ functions not conflicting when used within EDL
- [ ] Resource name conflicts resolved. Since resources are allowed the same name in GM, also functions can be overloaded as scripts.
- [ ] A list of known GM incompatibilities which are likely to remain -> GM Incompatibilities Page 

