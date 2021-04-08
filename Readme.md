
# ENIGMA
Prototype for Collision Detection System. 

## Files Changed
The following files are added/edited for this prototype:

``` ENIGMAsystem/SHELL/Universal_System/Object_Tiers/collision_object.h 
ENIGMAsystem/SHELL/Universal_System/Object_Tiers/collision_object.cpp
ENIGMAsystem/SHELL/Universal_System/Resources/sprites_internal.h
ENIGMAsystem/SHELL/Universal_System/Resources/sprites_internal.cpp
ENIGMAsystem/SHELL/Universal_System/Resources/sprites.h
ENIGMAsystem/SHELL/Universal_System/Resources/sprites.cpp
ENIGMAsystem/SHELL/Universal_System/Resources/polygon_internal.h
ENIGMAsystem/SHELL/Universal_System/Resources/polygon.cpp
ENIGMAsystem/SHELL/Universal_System/Resources/polygon.h
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Info/About.ey
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Info/collision_info.h
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Makefile
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Polygonfuncs.cpp
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Polygonimpl.cpp
ENIGMAsystem/SHELL/Collision_Systems/Polygon/Polygonimpl.h
ENIGMAsystem/SHELL/Collision_Systems/Polygon/include.h
ENIGMAsystem/SHELL/Collision_Systems/Polygon/placeholderlinks.cpp
ENIGMAsystem/SHELL/Collision_Systems/Precise/Info/About.ey
ENIGMAsystem/SHELL/SHELLmain.cpp
```
### polygon_internal.h
Provides the ``Polygon`` , ``Point``, and ``MinMaxProjection`` classes. These classes are provide all the mathematical functions for storing polygons. 

The ``Point`` class behaves as a 2D Vector, and thus has properties like (x, y) coordinate pairs alongside magnitude, angle, unit vectors, norms etc. 

The ``Polygon`` class holds the points, and provide functions to compute normals along the vertices. The functions are also overloaded to provide the calculations using the offsets of the instance, which is crucial when computing accurate results. 


### polygon.h
Provides functions for ``enigma_user`` namespace. The functions provides are:


- ``` int  polygon_add(int  height, int  width);```
- ``` int  polygon_add_point(int  id, int  x, int  y);```
##### polygon_add
Function to Add a new Polygon resource. Care must be taken as the height and width of this polygon should match the Sprite to be used. Returns the ID for the polygon that was added, as it uses the AssetArray internally to store and access the resource.
##### polygon_add_point
Function to add a new point inside the polygon resource. The id for the polygon is provides, which fetches the polygon and adds the point inside of it. The system currently will work only when the points of the polygon are added in a clockwise manner. 

### sprites.h
Provides the following functions for ``enigma_user``:

- ``int  sprite_attach_polygon(int  spr_id, int  poly_id);``
##### sprite_attach_polygon
The function attaches the sprite index to the polygon index provided. This is helpful when retrieving the polygon for collision detection later on. However, this method may not be the best suitable for it. 

### Polygonimpl.h
The code inside the Polygon Collision System is copied from Precise to provide stub code for the prototype to work. All functions are therefore using the Precise Collision System. The main function for collision detection, however, is implemented using the new SAT algorithm, which is 
```
enigma::object_collisions* const  collide_inst_inst(int  object, bool  solid_only, bool  notme, double  x, double  y);
```
This is the main function for collision detection, and is therefore implemented.

