/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 forthevin                                                **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef ENIGMA_COLLISIONTYPE
#define ENIGMA_COLLISIONTYPE

namespace enigma
{
  enum collision_type { //The order fits with the MaskShape enum in org.lateralgm.resources.Sprite.
    ct_precise, //Data is the image, collision shape is the precise image inside the bounding box.
    ct_bbox, //Data should be ignored, collision shape is the bounding box.
    ct_ellipse, //Data should be ignored, collision shape is an ellipse fitting the bounding box.
    ct_diamond, //Data should be ignored, collision shape is a diamond fitting the bounding box.
    ct_polygon,
    ct_circle //Data should be ignored, collision shape is a circle fitting inside the middle of the bounding box.
  };
}

#endif // ENIGMA_COLLISIONTYPE

