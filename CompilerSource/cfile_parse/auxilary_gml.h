{ rad2 = argument4; }
draw_primitive_begin(pr_trianglefan); //x, y
draw_vertex(x,y);
double radinc = 2*pi/argument2; //Sides
int rad = argument3,rad1 = argument4; //r1
double start = argument5;
for (double i = start; i <= 2 * pi + start + .01; i += radinc)
{
  draw_vertex(argument0+rad*cos(i),argument1-rad*sin(i));
  draw_vertex(argument0+rad1*cos(i+radinc/2),argument1-rad2*sin(i+radinc/2));
}
draw_primitive_end();

/*for ;;;
for (;;);

if 0;
else;
if (0)
{
  if 1
    do {
      if a 
        if b
          if c
            do
            {
              if d
                { a=b=c; }
              else
                for ;;;
            }
            while d
          else
            c=d
        else
          b=c
      else
        a=b
    }
    until 0
  else
   b = c
}
else a=0;
do
while 0;
until 0
*/
