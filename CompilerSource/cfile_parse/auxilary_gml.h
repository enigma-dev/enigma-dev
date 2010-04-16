draw_circle_color(x,y,7,c_white,c_red,0);
for (int i = 0; i < 10; i++)
{
  draw_circle_color(lengthdir_x(64*sin(pl[i]),pa[i]),lengthdir_y(64*sin(pl[i]),pa[i]),3,c_white,c_aqua,0);
  pl[i] += pi/60;
  pa[i] += 6;
}
