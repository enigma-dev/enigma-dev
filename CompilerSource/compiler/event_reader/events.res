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

rightbutton: 6
	Name: Right Button
	Mode: Special
	Case: 1

middlebutton: 6
	Name: Middle Button
	Mode: Special
	Case: 2

nobutton: 6
	Name: No Button
	Mode: Special
	Case: 3

leftpress: 6
	Name: Left Press
	Mode: Special
	Case: 4

rightpress: 6
	Name: Right Press
	Mode: Special
	Case: 5

middlepress: 6
	Name: Middle Press
	Mode: Special
	Case: 6

leftrelease: 6
	Name: Left Release
	Mode: Special
	Case: 7

rightrelease: 6
	Name: Right Release
	Mode: Special
	Case: 8

middlerelease: 6
	Name: Middle Release
	Mode: Special
	Case: 9

mouseenter: 6
	Name: Mouse Enter
	Mode: Special
	Case: 10

mouseleave: 6
	Name: Mouse Leave
	Mode: Special
	Case: 11

mousewheelup: 6
	Name: Mouse Wheel Up
	Mode: Special
	Case: 60

mousewheeldown: 6
	Name: Mouse Wheel Down
	Mode: Special
	Case: 61

globalleftbutton: 6
	Name: Global Left Button
	Mode: Special
	Case: 50

globalrightbutton: 6
	Name: Global Right Button
	Mode: Special
	Case: 51

globalmiddlebutton: 6
	Name: Global Middle Button
	Mode: Special
	Case: 52

globalleftpress: 6
	Name: Global Left Press
	Mode: Special
	Case: 53

globalrightpress: 6
	Name: Global Right Press
	Mode: Special
	Case: 54

globalmiddlepress: 6
	Name: Global Middle press
	Mode: Special
	Case: 55

globalleftrelease: 6
	Name: Global Left Release
	Mode: Special
	Case: 56

globalrightrelease: 6
	Name: Global Right Release
	Mode: Special
	Case: 57

globalmiddlerelease: 6
	Name: Global Middle Release
	Mode: Special
	Case: 57

# Finally, some general-purpose events

step: 3
	Name: Step
	Mode: Special
	Case: 0
	Constant: { 
		x += hspeed, y += vspeed;
		speed -= friction; 
		vspeed += sin(gravity_direction * pi/180),
		hspeed += cos(gravity_direction * pi/180);
	}


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
	Sub Check: (bbox_right < 0) or (bbox_left > room_width) or (bbox_bottom < 0) or (bbox_top > room_height)
boundary: 7
	Name: Intersect Boundary
	Mode: Special
	Case: 1
	Super Check: (bbox_left < 0) or (bbox_right > room_width) or (bbox_top < 0) or (bbox_bottom > room_height)


# Collisions stuck here for some reason, possibly so that you
# can deduct lives/health right before the "No more Lives" event

collision: 4
	Group: Collision
	Name: %1
	Type: Object
	Mode: Stacked
	Super Check: instance_number(%1)
	Sub Check: place_meeting(x,y,%1)


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
	Mode: Special
	Case: 0
	Instead: screen_redraw(); screen_refresh(); # We never want to iterate draw; we let screen_redraw() handle it.
	
#Draw GUI event is processed after all draw events iterating objects by depth and first resetting the projection to orthographic, ignoring views
drawgui: 8
	Name: Draw GUI
	Mode: Special
	Case: 64
	Sub Check: visible


# Why this comes after "end step," I do not know. One would think it'd be back there with pathend.
animationend: 7
	Name: Animation End
	Mode: Special
	Case: 7
	Sub Check: !(int(image_index) % image_count)


# End of in-linked events
# These are later-executed, special-triggered events that are also not listed for iteration

roomend: 7
	Name: Room End
	Mode: Spec-sys
	Case: 5

gameend: 7
	Name: Game End
	Mode: Spec-sys
	Case: 3


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


