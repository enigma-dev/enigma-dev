var display_width,display_height,viewport_width,viewport_height;

display_width = 640;
display_height = 480;

viewport_width = display_width - 64;
viewport_height = display_height - 64;

room_set_view(rm_splash_screen,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_splash_screen,1,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);

room_set_view(rm_title_screen,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_title_screen,1,1,32,32,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,0,0,-1);

room_set_view(rm_level_1,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_1,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_2,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_2,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_3,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_3,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_4,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_4,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_5,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_5,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_6,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_6,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_7,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_7,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_8,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_8,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);

room_set_view(rm_level_9,0,1,0,0,display_width,display_height,0,0,display_width,display_height,0,0,0,0,-1);
room_set_view(rm_level_9,1,1,0,0,viewport_width,viewport_height,(display_width/2)-(viewport_width/2),(display_height/2)-(viewport_height/2),viewport_width,viewport_height,viewport_width/2,viewport_height/2,-1,-1,obj_character);
