//on mouse over
if (point_distance(x,y,mouse_x,mouse_y) < 10) {
 x = random(room_width - 20) + 10
 y = random(room_height - 20) + 10
 direction = random(360)
 speed = 2
 score += 1
 room_caption = "Score: " + string(score)
}

//since speed/direction aren't implemented yet...
x += lengthdir_x(speed,direction)
y += lengthdir_y(speed,direction)

//bounce off window edges
if ((x <= 10) || (x >= (room_width - 10)))
 hspeed = -hspeed
if ((y <= 10) || (y >= room_height - 10)))
 vspeed = -vspeed
