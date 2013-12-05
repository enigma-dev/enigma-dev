x = max(view_xview,x) //sets view obj to follow behind player

footmsk.x=obj_player.x

if sprite_index=choose("_jump") /* && */ + alarm[4]>-1 
{ 
    footmsk.y = y+35
}
else
{
    footmsk.y = y
}

//general facing vars
if keyboard_check(vk_right)
{
    facing = 0
}
if keyboard_check(vk_left)
{
    facing = 1
}

//swimming vars
if place_meeting(x,y,water) 
{
    swimming=true
}
if place_meeting(x,y,ground_default) && !place_meeting(x,y,water) 
{
    swimming=false
    climbing=false
}
if place_meeting(x,y,ground_default) && !place_meeting(x,y,water) 
{
    splash=true
}

//falling through ground
if !(keyboard_check(vk_left) ^ keyboard_check(vk_right)) 
{
    if !place_meeting(x,y,water) && keyboard_check(vk_down) && keyboard_check_pressed(3)
    {
        script_execute(fall_through,id)
    } 
}

//jumping
if !keyboard_check(vk_down) && keyboard_check_pressed(3) && vspeed = 0 && !place_meeting(x,y,water) 
{
        vspeed = -11 //jump height
        alarm[4]=35 //timer for moving foot_mask y
        jumping=true
}
if vspeed=0 && jumping=1 //unset jump after landing
{
    jumping=false
}
vspeed = min(vspeed,6) //maximum vspeed


//if pressing right && left at same time, || are underwater, && are not jumping set hspeed to 0
if (keyboard_check(vk_left) && keyboard_check(vk_right) && sprite_index!=choose("_jump")) || sprite_index=underwater || place_meeting(x,y,obj_house)
{
   hspeed = 0 + movingplt_hspeed
}
else if keyboard_check(vk_right)
{
    hspeed = 2 + movingplt_hspeed
}
else if keyboard_check(vk_left)
{
    hspeed = -2 + movingplt_hspeed
}
else if sprite_index!=choose("_jump") && climbing=false {
    hspeed=0 +movingplt_hspeed
}

// push player off ground
if abs(vspeed)>0 && facing=0 && jumping=false && downing=false 
{
    hspeed=1
}
if abs(vspeed)>0 && facing=1 && jumping=false && downing=false 
{
    hspeed=-1
}

//splash
if place_meeting(x,y,water) && splash=true && climbing=false
{
    sprite_index=gregtrevor_splash
} 
//swimming
else if place_meeting(x,y,water) && splash=false 
{
	if keyboard_check(vk_up) && keyboard_check(vk_right) && global.shooting="_shoot"
	{
		sprite_index=choose("_swim_up_R")  
	}  
	else if keyboard_check(vk_up) &&  keyboard_check(vk_left) && global.shooting="_shoot"
	{
		sprite_index=choose("_swim_up_L")  
    }  
	else if keyboard_check(vk_up) && facing=0 && !keyboard_check(vk_left) && !keyboard_check(vk_right) && global.shooting="_shoot"
	{
		sprite_index=choose("_swim_up_still_R")  
	}  
	else if keyboard_check(vk_up) && facing=1 && !keyboard_check(vk_left) && !keyboard_check(vk_right) && global.shooting="_shoot" 
	{
		sprite_index=choose("_swim_up_still_L")  
	}  
	else if keyboard_check(vk_down)
	{
	   sprite_index=underwater
    	}
	else if keyboard_check(vk_right)
	{
	    sprite_index=choose("_swim_R")
    	}
	else if keyboard_check(vk_left)
	{
        	sprite_index=choose("_swim_L")
    	}
	else if facing=0
	{
	    sprite_index=choose("_swim_R")
    	}
	else if facing=1
	{
        	sprite_index=choose("_swim_L")
    	}
}
//if pressing up
else if keyboard_check(vk_up) && vspeed=0  && climbing=false 
{
	if !keyboard_check(vk_left) && !keyboard_check(vk_right)
	{
		if facing==0
		{
			sprite_index=choose("_up_still_R")
		}
		else
		{
			sprite_index=choose("_up_still_L")
		}
	} 
	else if keyboard_check(vk_right) && keyboard_check(vk_left)
	{
		sprite_index=choose("_up_still_R")
	}
	else
	{
		if keyboard_check(vk_right)
		{
			sprite_index=choose("_up_R")
		}
		else if keyboard_check(vk_left)
		{
			sprite_index=choose("_up_L")
		}
	}
}
//if down is pressed 
else if keyboard_check(vk_down) && vspeed=0  && climbing=false 
{
    if !keyboard_check(vk_left) && !keyboard_check(vk_right)
    {
        if facing==0
        {
            sprite_index=choose("_crouch_R")
        }
        else
        {
        sprite_index=choose("_crouch_L")
        }
    } 
    else if keyboard_check(vk_right) && keyboard_check(vk_left)
    {
        sprite_index=choose("_crouch_R")
    }
    else
    {
        if keyboard_check(vk_right)
        {
        sprite_index=choose("_down_R")
        }
        else if keyboard_check(vk_left)
        {
        sprite_index=choose("_down_L")
        }
    }
}
//if falling
else if downing==true || (jumping=false && abs(vspeed)>0) 
{ 
	if facing==0
	{
		sprite_index=choose("_fall_R")
	}
	else
	{
		sprite_index=choose("_fall_L")
	}
}
//if jumping
else if downing=false && jumping=true && gravity > 0 && climbing=false
{
	sprite_index=choose("_jump")
}
//general right/left movement
else if vspeed=0  && climbing=false 
{
	if keyboard_check(vk_right) && keyboard_check(vk_left) 
	{
	   sprite_index=choose("_still_R")
	}
	else if keyboard_check(vk_right)
	{
	   sprite_index=choose("_run_R")
	}
	else if keyboard_check(vk_left)
	{
		sprite_index=choose("_run_L")
	}
	else if facing=0
	{
	   sprite_index=choose("_still_R")
	}
	else if facing=1
	{
		sprite_index=choose("_still_L")
	}
}
