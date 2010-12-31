int **gem_board;
gem_board = new int[(gem_xmax - gem_xmin)/48+1][(gem_ymax - gem_ymin)/48+1];

for (int x = gem_xmin; x < gem_xmax; x+=48)
for (int y = gem_ymin; y < gem_ymax; y+=48)
instance_create(x,y,obj_gem);



if collision_bbox_rect(obj_gem,x,y+49,x+32,y+51) or y+48>room_height-32
{
  if (!still)
  {
    still = true;
    gem_board[(x - gem_xmin)/48][(y - gem_ymin)/48];
    gems_moving--;
  }
  vspeed = 0, y=round((y-gem_ymin)/48)*48+gem_ymin;
}
else
{
  if (still)
  {
    gem_board[(x - gem_xmin)/48][(y - gem_ymin)/48];
    gems_moving++;
    still = false;
  }
  vspeed += (vspeed < 16);
}

