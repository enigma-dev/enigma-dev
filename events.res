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
	
closebutton: 7		# This event is executed from within code when the game window close button is hit
	Name: Close Button
	Mode: Spec-sys
	Case: 30
	
asyncimageloaded: 7		# This event is executed from within code when an image finishes loading
	Name: Image Loaded
	Mode: Spec-sys
	Case: 60
	
asyncsoundloaded: 7		# This event is executed from within code when a sound finishes loading
	Name: Sound Loaded
	Mode: Spec-sys
	Case: 61
	
asynchttp: 7			# This event is executed from within code when an asynchronous http event is triggered
	Name: HTTP
	Mode: Spec-sys
	Case: 62
	
asyncdialog: 7		# This event is executed from within code when an asynchronous dialog is resolved
	Name: Dialog
	Mode: Spec-sys
	Case: 63
	
asynciap: 7		# This event is executed from within code when an asynchronous In-App purchase is triggered
	Name: IAP
	Mode: Spec-sys
	Case: 66
	
asynccloud: 7		# This event is executed from within code when an asynchronous cloud event is triggered
	Name: Cloud
	Mode: Spec-sys
	Case: 67
	
asyncnetworking: 7	# This event is executed from within code when an asynchronous networking event is triggered
	Name: Networking
	Mode: Spec-sys
	Case: 68
	
asyncsteam: 7		# This event is executed from within code when an asynchronous Steam event is triggered
	Name: Steam
	Mode: Spec-sys
	Case: 69
	
asyncsocial: 7		# This event is executed from within code when an asynchronous social event is triggered
	Name: Social
	Mode: Spec-sys
	Case: 70

roomstart: 7		# This event is executed from within the code that loads a new room
	Name: Room Start
	Mode: Spec-sys
	Case: 4
	
create: 0			# This event is performed as a ctor: immediately as the instance is created
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
	Constant: {xprevious = x; yprevious = y; if (sprite_index != -1) image_index = fmod((image_speed < 0)?(sprite_get_number(sprite_index) + image_index - fmod(abs(image_speed),sprite_get_number(sprite_index))):(image_index + image_speed), sprite_get_number(sprite_index));}

alarm: 2
	Group: Alarm
	Name: Alarm %1
	Mode: Stacked
	Sub Check: { alarm[%1] = (int)alarm[%1]; return !((alarm[%1] == -1) or (alarm[%1]--)); }


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
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

rightbutton: 6
	Name: Right Button
	Mode: Special
	Case: 1
	Super Check: mouse_check_button(mb_right)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

middlebutton: 6
	Name: Middle Button
	Mode: Special
	Case: 2
	Super Check: mouse_check_button(mb_middle)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

nobutton: 6
	Name: No Button
	Mode: Special
	Case: 3
	Sub Check:   mouse_check_button(mb_none)

leftpress: 6
	Name: Left Press
	Mode: Special
	Case: 4
	Super Check: mouse_check_button_pressed(mb_left)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

rightpress: 6
	Name: Right Press
	Mode: Special
	Case: 5
	Super Check: mouse_check_button_pressed(mb_right)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

middlepress: 6
	Name: Middle Press
	Mode: Special
	Case: 6
	Super Check: mouse_check_button_pressed(mb_middle)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

leftrelease: 6
	Name: Left Release
	Mode: Special
	Case: 7
	Super Check: mouse_check_button_released(mb_left)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

rightrelease: 6
	Name: Right Release
	Mode: Special
	Case: 8
	Super Check: mouse_check_button_released(mb_right)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

middlerelease: 6
	Name: Middle Release
	Mode: Special
	Case: 9
	Super Check: mouse_check_button_released(mb_middle)
	Sub Check:   position_meeting(mouse_x, mouse_y, id)

mouseenter: 6
	Name: Mouse Enter
	Mode: Special
	Case: 10
	Locals: bool $innowEnter = false;
	Sub Check: { const bool wasin = $innowEnter; $innowEnter = position_meeting(mouse_x, mouse_y, id); return !(!$innowEnter or wasin); }

mouseleave: 6
	Name: Mouse Leave
	Mode: Special
	Case: 11
	Locals: bool $innowLeave = false;
	Sub Check: { const bool wasin = $innowLeave; $innowLeave = position_meeting(mouse_x, mouse_y, id); return !($innowLeave or !wasin); }

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
	Constant: { if (timeline_running && timeline_speed!=0) advance_curr_timeline(); }

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
	Sub Check: (bbox_right < 0) || (bbox_left > room_width) || (bbox_bottom < 0) || (bbox_top > room_height)
boundary: 7
	Name: Intersect Boundary
	Mode: Special
	Case: 1
	Sub Check: (bbox_left < 0) || (bbox_right > room_width) || (bbox_top < 0) || (bbox_bottom > room_height)
outsideviewzero: 7
	Name: Outside View 0
	Mode: Special
	Case: 40
	Sub Check: (!view_enabled || !view_visible[0]) ? false : (bbox_right < view_xview[0]) || (bbox_left > view_xview[0] + view_wview[0]) || (bbox_bottom < view_yview[0]) || (bbox_top > view_yview[0] + view_hview[0])
outsideviewone: 7
	Name: Outside View 1
	Mode: Special
	Case: 41
	Sub Check: (!view_enabled || !view_visible[1]) ? false : (bbox_right < view_xview[1]) || (bbox_left > view_xview[1] + view_wview[1]) || (bbox_bottom < view_yview[1]) || (bbox_top > view_yview[1] + view_hview[1])
outsideviewtwo: 7
	Name: Outside View 2
	Mode: Special
	Case: 42
	Sub Check: (!view_enabled || !view_visible[2]) ? false : (bbox_right < view_xview[2]) || (bbox_left > view_xview[2] + view_wview[2]) || (bbox_bottom < view_yview[2]) || (bbox_top > view_yview[2] + view_hview[2])
outsideviewthree: 7
	Name: Outside View 3
	Mode: Special
	Case: 43
	Sub Check: (!view_enabled || !view_visible[3]) ? false : (bbox_right < view_xview[3]) || (bbox_left > view_xview[3] + view_wview[3]) || (bbox_bottom < view_yview[3]) || (bbox_top > view_yview[3] + view_hview[3])
outsideviewfour: 7
	Name: Outside View 4
	Mode: Special
	Case: 44
	Sub Check: (!view_enabled || !view_visible[4]) ? false : (bbox_right < view_xview[4]) || (bbox_left > view_xview[4] + view_wview[4]) || (bbox_bottom < view_yview[4]) || (bbox_top > view_yview[4] + view_hview[4])
outsideviewfive: 7
	Name: Outside View 5
	Mode: Special
	Case: 45
	Sub Check: (!view_enabled || !view_visible[5]) ? false : (bbox_right < view_xview[5]) || (bbox_left > view_xview[5] + view_wview[5]) || (bbox_bottom < view_yview[5]) || (bbox_top > view_yview[5] + view_hview[5])
outsideviewsix: 7
	Name: Outside View 6
	Mode: Special
	Case: 46
	Sub Check: (!view_enabled || !view_visible[6]) ? false : (bbox_right < view_xview[6]) || (bbox_left > view_xview[6] + view_wview[6]) || (bbox_bottom < view_yview[6]) || (bbox_top > view_yview[6] + view_hview[6])
outsideviewseven: 7
	Name: Outside View 7
	Mode: Special
	Case: 47
	Sub Check: (!view_enabled || !view_visible[7]) ? false : (bbox_right < view_xview[7]) || (bbox_left > view_xview[7] + view_wview[7]) || (bbox_bottom < view_yview[7]) || (bbox_top > view_yview[7] + view_hview[7])

boundaryviewzero: 7
	Name: Boundary View 0
	Mode: Special
	Case: 50
	Sub Check: (!view_enabled || !view_visible[0]) ? false : (bbox_left < view_xview[0]) || (bbox_right > view_xview[0] + view_wview[0]) || (bbox_top < view_yview[0]) || (bbox_bottom > view_yview[0] + view_hview[0])
boundaryviewone: 7
	Name: Boundary View 1
	Mode: Special
	Case: 51
	Sub Check: (!view_enabled || !view_visible[1]) ? false : (bbox_left < view_xview[1]) || (bbox_right > view_xview[1] + view_wview[1]) || (bbox_top < view_yview[1]) || (bbox_bottom > view_yview[1] + view_hview[1])
boundaryviewtwo: 7
	Name: Boundary View 2
	Mode: Special
	Case: 52
	Sub Check: (!view_enabled || !view_visible[2]) ? false : (bbox_left < view_xview[2]) || (bbox_right > view_xview[2] + view_wview[2]) || (bbox_top < view_yview[2]) || (bbox_bottom > view_yview[2] + view_hview[2])
boundaryviewthree: 7
	Name: Boundary View 3
	Mode: Special
	Case: 53
	Sub Check: (!view_enabled || !view_visible[3]) ? false : (bbox_left < view_xview[3]) || (bbox_right > view_xview[3] + view_wview[3]) || (bbox_top < view_yview[3]) || (bbox_bottom > view_yview[3] + view_hview[3])
boundaryviewfour: 7
	Name: Boundary View 4
	Mode: Special
	Case: 54
	Sub Check: (!view_enabled || !view_visible[4]) ? false : (bbox_left < view_xview[4]) || (bbox_right > view_xview[4] + view_wview[4]) || (bbox_top < view_yview[4]) || (bbox_bottom > view_yview[4] + view_hview[4])
boundaryviewfive: 7
	Name: Boundary View 5
	Mode: Special
	Case: 55
	Sub Check: (!view_enabled || !view_visible[5]) ? false : (bbox_left < view_xview[5]) || (bbox_right > view_xview[5] + view_wview[5]) || (bbox_top < view_yview[5]) || (bbox_bottom > view_yview[5] + view_hview[5])
boundaryviewsix: 7
	Name: Boundary View 6
	Mode: Special
	Case: 56
	Sub Check: (!view_enabled || !view_visible[6]) ? false : (bbox_left < view_xview[6]) || (bbox_right > view_xview[6] + view_wview[6]) || (bbox_top < view_yview[6]) || (bbox_bottom > view_yview[6] + view_hview[6])
boundaryviewseven: 7
	Name: Boundary View 7
	Mode: Special
	Case: 57
	Sub Check: (!view_enabled || !view_visible[7]) ? false : (bbox_left < view_xview[7]) || (bbox_right > view_xview[7] + view_wview[7]) || (bbox_top < view_yview[7]) || (bbox_bottom > view_yview[7] + view_hview[7])

# Collisions stuck here for some reason, possibly so that you
# can deduct lives/health right before the "No more Lives" event

beforecollisionautomaticcollisionhandling: 100000
	Name: Before collision automatic collision handling
	Mode: None
	Default: ;
	Instead: enigma::perform_callbacks_before_collision_event();

collision: 4
	Group: Collision
	Name: %1
	Type: Object
	Mode: Stacked
	Super Check: instance_number(%1)
	Sub Check: (instance_other = enigma::place_meeting_inst(x,y,%1)) # Parenthesize assignment used as truth
	prefix: for (enigma::iterator it = enigma::fetch_inst_iter_by_int(%1); it; ++it) {int $$$internal$$$ = %1; instance_other = *it; if (enigma::place_meeting_inst(x,y,instance_other->id)) {if(enigma::glaccess(int(other))->solid && enigma::place_meeting_inst(x,y,instance_other->id)) x = xprevious, y = yprevious;
	suffix: if (enigma::glaccess(int(other))->solid) {x += hspeed; y += vspeed; if (enigma::place_meeting_inst(x, y, $$$internal$$$)) {x = xprevious; y = yprevious;}}}}
# Check for detriment from collision events above

nomorelives: 7
	Name: No More Lives
	Mode: Special
	Case: 6
  Super Check: enigma::update_lives_status_and_return_zeroless()
nomorehealth: 7
	Name: No More Health
	Mode: Special
	Case: 9
  Locals: bool $out_of_health = 0;
	Sub Check: { bool OoH = $out_of_health; if (health <= 0) { $out_of_health = true; return !OoH; } $out_of_health = false; return false; }
  Suffix: if (health > 0) { $out_of_health = 0; }


# General purpose once again!

endstep: 3
	Name: End Step
	Mode: Special
	Case: 2
	Constant: { if (timeline_running && timeline_loop && timeline_speed!=0) loop_curr_timeline(); }

particlesystemsupdate: 100000
	Name: Particle Systems Update
	Mode: None
	Default: ;
	Instead: enigma::perform_callbacks_particle_updating();


# Fun fact: Draw comes after End Step.
draw: 8
	Name: Draw
	Mode: Special
	Case: 0
	Sub Check: visible
	Iterator-declare: /* Draw is handled by depth */
	Iterator-initialize: /* Draw is initialized in the constructor */
	Iterator-remove: depth.remove();
	Iterator-delete: /* Draw will destruct with this */
	Default: if (visible && sprite_index != -1) draw_sprite_ext(sprite_index,image_index,x,y,image_xscale,image_yscale,image_angle,image_blend,image_alpha);
	Instead: if (automatic_redraw) screen_redraw(); # We never want to iterate draw; we let screen_redraw() handle it.
	
#Draw GUI event is processed after all draw events iterating objects by depth and first resetting the projection to orthographic, ignoring views
drawgui: 8
	Name: Draw GUI
	Mode: Special
	Case: 64
	Sub Check: visible
	
#Draw Resize event is processed whenever a resize to the game window occurs, basically so you can resize the GUI layer and shit,
#why is it not under other along with the removed close button event? Good fucking question, well, it goes like this young Timmy,
#there once was a man named Yolo, who came the fuck out of nowhere and destroyed Game Maker with evil capitalism, the fucking end, now go to bed.
drawresize: 8
	Name: Draw Resize
	Mode: Spec-sys
	Case: 65


# Why this comes after "end step," I do not know. One would think it'd be back there with pathend.
animationend: 7
	Name: Animation End
	Mode: Special
	Case: 7
	Sub Check: { return !(image_index + image_speed < sprite_get_number(sprite_index)); }


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
userfifteen: 7
	Name: User defined 15
	Mode: Special
	Case: 25

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
