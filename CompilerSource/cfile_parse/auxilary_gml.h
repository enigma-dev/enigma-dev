int a = instance_create(x,y,particle);
room_caption = "id: " + string(a) + " instance_count: " + string(instance_count);
if (keyboard_check(vk_f4))
  window_set_fullscreen(!window_get_fullscreen())
