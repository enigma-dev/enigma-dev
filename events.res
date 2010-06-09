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
	Mode: Spec-none
	Case: 2

roomstart: 7		# This event is executed from within the code that loads a new room
	Mode: Spec-none
	Case: 4



# Here marks the start of events that are actually executed in place

step: 3 
	Mode: Special
	Case: 1
alarm: 2
	Mode: Stacked
	Sub Check: { if ((alarm[%1] == -1) or (alarm[%1]--)) return 0; }


# Keyboard events. These are simple enough.
keyboard: 5
	Mode: Inline
	Super Check: keyboard_check(%1)
keypress: 9
	Mode: Inline
	Super Check: keyboard_check_pressed(%1)
keyrelease: 10
	Mode: Inline
	Super Check: keyboard_check_released(%1)


# There are a million different specialized mouse events.
mouse: 6
	Mode: Special
	Super Check: mouse_check


# Finally, some general-purpose eventage
step: 3
	Mode: Special
	Case: 0
	Sub Check: { x += hspeed; y += vspeed; speed -= friction; vspeed += sin(gravity_direction * pi/180); hspeed += cos(gravity_direction * pi/180); }


# Lump of "Other" events.
pathend: 7
	Mode: Special
	Case: 8
	Super Check: false #Paths are not yet implemented.
outsideroom: 7
	Mode: Special
	Case: 0
	Sub Check: (bbox_right < 0) or (bbox_left > room_width) or (bbox_bottom < 0) or (bbox_top > room_height)
boundary: 7
	Mode: Special
	Case: 1
	Super Check: (bbox_left < 0) or (bbox_right > room_width) or (bbox_top < 0) or (bbox_bottom > room_height)

# Collisions stuck here for some reason, possibly so that you
# can deduct lives/health right before the "No more Lives" event
collision: 4
	Mode: Stacked
	Super Check: instance_number(%1)
	Sub Check: place_meeting(x,y,%1)

# Check for detriment from collision events above
nomorelives: 7
	Mode: Special
	Case: 6
	Sub Check: lives <= 0
nomorehealth: 7
	Mode: Special
	Case: 9
	Sub Check: health <= 0

# General purpose once again!
endstep: 3
	Mode: Special
	Case: 2

# Why this comes after "end step," I do not know. One would think it'd be with pathend
animationend: 7
	Mode: Special
	Case: 7
	Sub Check: !(int(image_index) % image_count)


# End of in-linked events
# These are later-executed, special-triggered events that are also not listed for iteration

roomend: 7
	Mode: Spec-none
	Case: 5

gameend: 7
	Mode: Spec-none
	Case: 3

parent_endstep: -1


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


