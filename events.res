# This file contains a list of all events in ENIGMA and the order in which they are executed.
#
# By "contains," I actually mean to imply "dictates." This file will let you re-order events,
#	change the behavior of existing events, and even define new events with new behaviours,
#	so long as the IDE supports adding them.
#
# That said, DO NOT MODIFY THE CONTENTS OF THIS FILE, UNLESS YOU HAVE READ THE DOCUMENTATION.
# Modifying this file CAN screw up ENIGMA's behavior.
#


# These events are executed outside the main event source at special moments

gamestart: 7		# This event is executed from within code at the start of the game
	Name: Game Start
	Mode: Spec-sys
	Case: 2

roomstart: 7		# This event is executed from within the code that loads a new room
	Name: Room Start
	Mode: Spec-sys
	Case: 4
create: 0			# This event is performed as a ctor: immedaitely as the instance is created
	Name: Create
	Mode: System

destroy: 1			# This event is performed as a dtor: immediately as the instance is "destroyed"
	Name: Destroy
	Mode: System


# Here marks the start of events that are actually executed in place

beginstep: 3
	Group: Step
	Name: Begin Step
	Mode: Special
	Case: 1
	Constant: {xprevious = x; yprevious = y;}

alarm: 2
	Group: Alarm
	Name: Alarm %1
	Mode: Stacked
	Sub Check: { if ((alarm[%1] == -1) or (alarm[%1]--)) return 0; }


# Keyboard events. These are simple enough.

keyboard: 5
	Group: Keyboard
	Name: Keyboard <%1>
	Type: Key
	Mode: Stacked
	Super Check: keyboard_check(%1)

keypress: 9
	Group: Key Press
	Name: Press <%1>
	Type: Key
	Mode: Stacked
	Super Check: keyboard_check_pressed(%1)

keyrelease: 10
	Group: Key Release
	Name: Release <%1>
	Type: Key
	Mode: Stacked
	Super Check: keyboard_check_released(%1)


# There are a million different specialized mouse events.

leftbutton: 6
	Group: Mouse
	Name: Left Button
	Mode: Special
	Case: 0
	Super Check: mouse_check_button(mb_left)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

rightbutton: 6
	Name: Right Button
	Mode: Special
	Case: 1
	Super Check: mouse_check_button(mb_right)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

middlebutton: 6
	Name: Middle Button
	Mode: Special
	Case: 2
	Super Check: mouse_check_button(mb_middle)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

nobutton: 6
	Name: No Button
	Mode: Special
	Case: 3
	Sub Check:   mouse_check_button(mb_none) && !(mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom)

leftpress: 6
	Name: Left Press
	Mode: Special
	Case: 4
	Super Check: mouse_check_button_pressed(mb_left)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

rightpress: 6
	Name: Right Press
	Mode: Special
	Case: 5
	Super Check: mouse_check_button_pressed(mb_right)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

middlepress: 6
	Name: Middle Press
	Mode: Special
	Case: 6
	Super Check: mouse_check_button_pressed(mb_middle)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

leftrelease: 6
	Name: Left Release
	Mode: Special
	Case: 7
	Super Check: mouse_check_button_released(mb_left)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

rightrelease: 6
	Name: Right Release
	Mode: Special
	Case: 8
	Super Check: mouse_check_button_released(mb_right)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

middlerelease: 6
	Name: Middle Release
	Mode: Special
	Case: 9
	Super Check: mouse_check_button_released(mb_middle)
	Sub Check:   mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom

mouseenter: 6
	Name: Mouse Enter
	Mode: Special
	Case: 10
	Locals: bool $innowEnter = false;
	Sub Check: { const bool wasin = $innowEnter; $innowEnter = mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom; if (!$innowEnter or wasin) return 0; }

mouseleave: 6
	Name: Mouse Leave
	Mode: Special
	Case: 11
	Locals: bool $innowLeave = false;
	Sub Check: { const bool wasin = $innowLeave; $innowLeave = mouse_x > x + bbox_left and mouse_x < x + bbox_right and mouse_y > y + bbox_top and mouse_y < y + bbox_bottom; if ($innowLeave or !wasin) return 0; }

mouseunknown: 6
	Name: Mouse Unknown (old? LGM doesn't even know!)
	Mode: Special
	Case: 12
	Super Check: 
mouseunknowntwo: 6
	Name: Mouse Unknown (old? LGM doesn't even know!)
	Mode: Special
	Case: 13
	Super Check: 

mousewheelup: 6
	Name: Mouse Wheel Up
	Mode: Special
	Case: 60
	Super Check: mouse_vscrolls > 0

mousewheeldown: 6
	Name: Mouse Wheel Down
	Mode: Special
	Case: 61
	Super Check: mouse_vscrolls < 0

globalleftbutton: 6
	Name: Global Left Button
	Mode: Special
	Case: 50
	Super Check: mouse_check_button(mb_left)

globalrightbutton: 6
	Name: Global Right Button
	Mode: Special
	Case: 51
	Super Check: mouse_check_button(mb_right)

globalmiddlebutton: 6
	Name: Global Middle Button
	Mode: Special
	Case: 52
	Super Check: mouse_check_button(mb_middle)

globalleftpress: 6
	Name: Global Left Press
	Mode: Special
	Case: 53
	Super Check: mouse_check_button_pressed(mb_left)

globalrightpress: 6
	Name: Global Right Press
	Mode: Special
	Case: 54
	Super Check: mouse_check_button_pressed(mb_right)

globalmiddlepress: 6
	Name: Global Middle press
	Mode: Special
	Case: 55
	Super Check: mouse_check_button_pressed(mb_middle)

globalleftrelease: 6
	Name: Global Left Release
	Mode: Special
	Case: 56
	Super Check: mouse_check_button_released(mb_left)

globalrightrelease: 6
	Name: Global Right Release
	Mode: Special
	Case: 57
	Super Check: mouse_check_button_released(mb_right)

globalmiddlerelease: 6
	Name: Global Middle Release
	Mode: Special
	Case: 58
	Super Check: mouse_check_button_released(mb_middle)

# Finally, some general-purpose events

step: 3
	Name: Step
	Mode: Special
	Case: 0

localsweep: 100000 
	Name: Locals sweep 
	Mode: Inline
	Constant: enigma::propagate_locals(this);


# Lump of "Other" events.

pathend: 7
	Name: Path End
	Mode: Special
	Case: 8
	Super Check: false #Paths are not yet implemented.
outsideroom: 7
	Name: Outside Room
	Mode: Special
	Case: 0
	Sub Check: (x+bbox_right < 0) || (x+bbox_left > room_width) || (y+bbox_bottom < 0) || (y+bbox_top > room_height)
boundary: 7
	Name: Intersect Boundary
	Mode: Special
	Case: 1
	Sub Check: (x+bbox_left < 0) or (x+bbox_right > room_width) or (y+bbox_top < 0) or (y+bbox_bottom > room_height)


# Collisions stuck here for some reason, possibly so that you
# can deduct lives/health right before the "No more Lives" event

collision: 4
	Group: Collision
	Name: %1
	Type: Object
	Mode: Stacked
	Super Check: instance_number(%1)
	Sub Check: (instance_other = enigma::place_meeting_inst(x,y,%1)) # Parenthesize assignment used as truth
	prefix: if (solid ||  enigma::glaccess(int(other))->solid) {x = xprevious; y = yprevious;}
# Check for detriment from collision events above

nomorelives: 7
	Name: No More Lives
	Mode: Special
	Case: 6
	Sub Check: lives <= 0
nomorehealth: 7
	Name: No More Health
	Mode: Special
	Case: 9
	Sub Check: health <= 0


# General purpose once again!

endstep: 3
	Name: End Step
	Mode: Special
	Case: 2


# Fun fact: Draw comes after End Step.
draw: 8
	Name: Draw
	Mode: Inline
	Sub Check: visible
	Iterator-declare: /* Draw is handled by depth */
	Iterator-initialize: /* Draw is initialized in the constructor */
	Iterator-remove: depth.remove();
	Iterator-delete: /* Draw will destruct with this */
	Default: if (image_single!=-1) {image_speed=0;image_index=image_single;} if (visible && sprite_index != -1) draw_sprite_ext(sprite_index,image_index,x,y,image_xscale,image_yscale,image_angle,image_blend,image_alpha);
	Constant: image_index = fmod(image_index + image_speed, sprite_get_number(sprite_index));
	Instead: screen_redraw(); screen_refresh(); # We never want to iterate draw; we let screen_redraw() handle it.


# Why this comes after "end step," I do not know. One would think it'd be back there with pathend.
animationend: 7
	Name: Animation End
	Mode: Special
	Case: 7
	Locals: float $iip = -1;
	Sub Check: { const bool r = ($iip <= (float)image_index); $iip = image_index; if (r) return 0; }


# End of in-linked events
# These are later-executed, special-triggered events that are also not listed for iteration

roomend: 7
	Name: Room End
	Mode: Spec-sys
	Case: 5n

gameend: 7
	Name: Game End
	Mode: Spec-sys
	Case: 3

# user events

userzero: 7
	Name: User defined 0
	Mode: Special
	Case: 10

userone: 7
	Name: User defined 1
	Mode: Special
	Case: 11
usertwo: 7
	Name: User defined 2
	Mode: Special
	Case: 12
userthree: 7
	Name: User defined 3
	Mode: Special
	Case: 13
userfour: 7
	Name: User defined 4
	Mode: Special
	Case: 14
userfive: 7
	Name: User defined 5
	Mode: Special
	Case: 15
usersix: 7
	Name: User defined 6
	Mode: Special
	Case: 16
userseven: 7
	Name: User defined 7
	Mode: Special
	Case: 17
usereight: 7
	Name: User defined 8
	Mode: Special
	Case: 18
usernine: 7
	Name: User defined 9
	Mode: Special
	Case: 19
userten: 7
	Name: User defined 10
	Mode: Special
	Case: 20
usereleven: 7
	Name: User defined 11
	Mode: Special
	Case: 21
usertwelve: 7
	Name: User defined 12
	Mode: Special
	Case: 22
userthirteen: 7
	Name: User defined 13
	Mode: Special
	Case: 23
userfourteen: 7
	Name: User defined 14
	Mode: Special
	Case: 24

#other mouse events
joystickoneleft: 6
	Name: Joystick 1 Left
	Mode: Special
	Case: 16
	Super Check: 
joystickoneright: 6
	Name: Joystick 1 Right
	Mode: Special
	Case: 17
	Super Check: 
joystickoneup: 6
	Name: Joystick 1 Up
	Mode: Special
	Case: 18
	Super Check: 
joystickonedown: 6
	Name: Joystick 1 Down
	Mode: Special
	Case: 19
	Super Check: 
joystickonebuttonone: 6
	Name: Joystick 1 Button 1
	Mode: Special
	Case: 21
	Super Check: 
joystickonebuttontwo: 6
	Name: Joystick 1 Button 2
	Mode: Special
	Case: 22
	Super Check: 
joystickonebuttonthree: 6
	Name: Joystick 1 Button 3
	Mode: Special
	Case: 23
	Super Check: 
joystickonebuttonfour: 6
	Name: Joystick 1 Button 4
	Mode: Special
	Case: 24
	Super Check: 
joystickonebuttonfive: 6
	Name: Joystick 1 Button 5
	Mode: Special
	Case: 25
	Super Check: 
joystickonebuttonsix: 6
	Name: Joystick 1 Button 6
	Mode: Special
	Case: 26
	Super Check: 
joystickonebuttonseven: 6
	Name: Joystick 1 Button 7
	Mode: Special
	Case: 27
	Super Check: 
joystickonebuttoneight: 6
	Name: Joystick 1 Button 8
	Mode: Special
	Case: 28
	Super Check: 

#joystick 2
joysticktwoleft: 6
	Name: Joystick 2 Left
	Mode: Special
	Case: 31
	Super Check: 
joysticktworight: 6
	Name: Joystick 2 Right
	Mode: Special
	Case: 32
	Super Check: 
joysticktwoup: 6
	Name: Joystick 2 Up
	Mode: Special
	Case: 33
	Super Check: 
joysticktwodown: 6
	Name: Joystick 2 Down
	Mode: Special
	Case: 34
	Super Check: 
joysticktwobuttonone: 6
	Name: Joystick 2 Button 1
	Mode: Special
	Case: 36
	Super Check: 
joysticktwobuttontwo: 6
	Name: Joystick 2 Button 2
	Mode: Special
	Case: 37
	Super Check: 
joysticktwobuttonthree: 6
	Name: Joystick 2 Button 3
	Mode: Special
	Case: 38
	Super Check: 
joysticktwobuttonfour: 6
	Name: Joystick 2 Button 4
	Mode: Special
	Case: 39
	Super Check: 
joysticktwobuttonfive: 6
	Name: Joystick 2 Button 5
	Mode: Special
	Case: 40
	Super Check: 
joysticktwobuttonsix: 6
	Name: Joystick 2 Button 6
	Mode: Special
	Case: 41
	Super Check: 
joysticktwobuttonseven: 6
	Name: Joystick 2 Button 7
	Mode: Special
	Case: 42
	Super Check: 
joysticktwobuttoneight: 6
	Name: Joystick 2 Button 8
	Mode: Special
	Case: 43
	Super Check: 


#  EV_BOUNDARY = 1,
#  EV_GAME_START = 2,
#  EV_GAME_END = 3,
#  EV_ROOM_START = 4,
#  EV_ROOM_END = 5,
#  EV_NO_MORE_LIVES = 6,
#  EV_NO_MORE_HEALTH = 9,
#  EV_ANIMATION_END = 7,
#  EV_END_OF_PATH = 8,
#
#keyboard
#keypress
#keyrelease
#
#mouse
#
#step
#pathend
#outsideroom
#boundary
#collision
#nomorelives
#nomorehealth
#endstep
#screen_redraw
#screen_refresh
#endanimation
#roomend
#gameend
#
#parent_endstep

