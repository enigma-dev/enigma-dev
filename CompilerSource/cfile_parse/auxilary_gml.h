//splash
if place_meeting(x,y,obj_water_path) and splash=true and climbing=false
{
    sprite_index=spr_gregtrevor_splash
} 
//swimming
else if place_meeting(x,y,obj_water_path) and splash=false 
{
	if keyboard_check(vk_up) && keyboard_check(vk_right) && global.shooting="_shoot"
	{
		sprite_index=sprite_player("_swim_up_R")  
	}  
	else if keyboard_check(vk_up) &&  keyboard_check(vk_left) && global.shooting="_shoot"
	{
		sprite_index=sprite_player("_swim_up_L")  
    }  
	else if keyboard_check(vk_up) && facing=0 && !keyboard_check(vk_left) && !keyboard_check(vk_right) && global.shooting="_shoot"
	{
		sprite_index=sprite_player("_swim_up_still_R")  
	}  
	else if keyboard_check(vk_up) && facing=1 && !keyboard_check(vk_left) && !keyboard_check(vk_right) && global.shooting="_shoot" 
	{
		sprite_index=sprite_player("_swim_up_still_L")  
	}  
	else if keyboard_check(vk_down)
	{
	   sprite_index=spr_underwater
    }
	else if keyboard_check(vk_right)
	{
	    sprite_index=sprite_player("_swim_R")
    }
	else if keyboard_check(vk_left)
	{
        	sprite_index=sprite_player("_swim_L")
    }
	else if facing=0
	{
	    sprite_index=sprite_player("_swim_R")
    }
	else if facing=1
	{
        	sprite_index=sprite_player("_swim_L")
    }
}
//if pressing up
else if keyboard_check(vk_up) and vspeed=0  and climbing=false 
{
	if !keyboard_check(vk_left) && !keyboard_check(vk_right)
	{
		if facing==0
		{
			sprite_index=sprite_player("_up_still_R")
		}
		else
		{
			sprite_index=sprite_player("_up_still_L")
		}
	} 
	else if keyboard_check(vk_right) && keyboard_check(vk_left)
	{
		sprite_index=sprite_player("_up_still_R")
	}
	else
	{
		if keyboard_check(vk_right)
		{
			sprite_index=sprite_player("_up_R")
		}
		else if keyboard_check(vk_left)
		{
			sprite_index=sprite_player("_up_L")
		}
	}
}
//if down is pressed 
else if keyboard_check(vk_down) and vspeed=0  and climbing=false 
{
    if !keyboard_check(vk_left) && !keyboard_check(vk_right)
    {
        if facing==0
        {
            sprite_index=sprite_player("_crouch_R")
        }
        else
        {
        sprite_index=sprite_player("_crouch_L")
        }
    } 
    else if keyboard_check(vk_right) && keyboard_check(vk_left)
    {
        sprite_index=sprite_player("_crouch_R")
    }
    else
    {
        if keyboard_check(vk_right)
        {
        sprite_index=sprite_player("_down_R")
        }
        else if keyboard_check(vk_left)
        {
        sprite_index=sprite_player("_down_L")
        }
    }
}
//if falling
else if downing==true or (jumping=false and abs(vspeed)>0) 
{ 
	if facing==0
	{
		sprite_index=sprite_player("_fall_R")
	}
	else
	{
		sprite_index=sprite_player("_fall_L")
	}
}
//if jumping
else if downing=false && jumping=true && gravity > 0 and climbing=false
{
	sprite_index=sprite_player("_jump")
}
//general right/left movement
else if vspeed=0  and climbing=false 
{
	if keyboard_check(vk_right) && keyboard_check(vk_left) 
	{
	   sprite_index=sprite_player("_still_R")
	}
	else if keyboard_check(vk_right)
	{
	   sprite_index=sprite_player("_run_R")
	}
	else if keyboard_check(vk_left)
	{
		sprite_index=sprite_player("_run_L")
	}
	else if facing=0
	{
	   sprite_index=sprite_player("_still_R")
	}
	else if facing=1
	{
		sprite_index=sprite_player("_still_L")
	}
}
