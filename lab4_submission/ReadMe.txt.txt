Author1: Shengtian Zhou
Author2: Boya Li
Group: 1

# the following readme explains each function in lcd.c and main.c
# In lcd.c

setPixel :
find the corresponding bar (8 pixel) the pixel is in and using bitwise operation to set the pixel to 1

clearePixel:
find the corresponding bar (8 pixel) the pixel is in and using bitwise operation to set the pixel to 0

drawString:
using drawchar to loop through each character

drawline:
a simple implementation of bresenham's algorithm

fillrect:
using drawline functions to accomplish this

drawrect:
essentially draw 4 lines

drawcircle:
using bresenham's circle algorithm

fillcircle:
set each pixel within the circle to be 1

# In main.c

drawball:
since the bresenham circle algorithm works best when the radius is large, this function uses draw 2 rectangles
to draw a circle like ball.

beep_tone:
when the ball hits either bounds or the paddle, a different sound it present by changing OCR1A

checkInput: 
this function reads X, Y, and Accelorameter's values, and convert X,Y properly.

updatebounds:
this function sets the upper bound and the lower bound for accelorameter's value, determining whether the use paddle's vertical moving direction

left:
update the ball's position when it goes left

right:
update the ball's position when it goes right

leftup:
update the ball's position when it goes left up

leftdown:
update the ball's position when it goes left down

rightup:
update the ball's position when it goes right up

rightdown:
update the ball's position when it goes right down

update:
updates ball's position, computer paddle, user paddle and scores during each iteration; besieds, it also resets everything and flip lcd lighting when detecting either score reaches 10

draw:
draw everything

checkCollision:
check if the ball hits the boundaries or paddle, or paddles and boundaries

readX:
reads the raw X value

readY:
reads the raw Y value

readAc:
reads the raw accelorameter X value

main:
setup led, setup adc, setup pwm, and run the program in a single while loop.
