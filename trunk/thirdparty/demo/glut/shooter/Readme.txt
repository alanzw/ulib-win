===============================================
=====  First Person Shooter  ==================
===============================================

______________________________________
== About =============================

This demo is the start of a simple First Person Shooter game.  There are no monsters, zombies, Nazis, or 2D sprites to shoot yet -- just you in a room by yourself with 2 big guns.

You may freely distribute this First Person Shooter demo and use the source code for research, or even pieces of it in your own program.  If you do use my code in your own program, please give me credit. 
 - Thanks, Steve Wortham

Read the "GNU License.txt" file for more information on distribution.

______________________________________
== What's New? =======================

== 4/12/2002 
- Released the source code. =)
- Slightly more practical collision detection (not quite as much hard coding)
- New textures (several 512x512 sized textures; older video cards may not support them)

______________________________________
== Controls ==========================

Arrow Keys ---------- movement
W ------------------- walk forward
S ------------------- walk backward
A ------------------- strafe left
D ------------------- strafe right
Left Mouse Click ---- fire
Spacebar ------------ fire (rapid fire)
F ------------------- toggles frames per second limit (about 60 fps -- limit is on by default)
I ------------------- toggles inverted mouse axis 
1 ------------------- M16
2 ------------------- Shock Rifle
ESC ----------------- Exit Program

I recommend using W,A,S,D and the mouse for movement.  Ignore the arrow keys -- the days of Wolfenstein 3D are over!

______________________________________
== Technology ========================

I used radiosity to render the lightmaps for this demo, and I used multitexturing to apply the lightmaps to the affected surfaces.  

All collision detection in this demo is hardcoded.  The main flaw in this demo is that there is no map loading.  I had to hardcode the map that you see with a couple of boxes.  

There is a relatively complicated particle engine used for the M16 ricochet, and there is some dynamic lightmapping used for the shock rifle.

______________________________________
== Known Bugs ========================

- Sometimes it will start up and the mouse will not respond.  If that happens to you, then you can click the mouse 3 times, and the demo should flicker and then refresh and then it should work just fine.  This doesn't seem to be a problem with my newly installed Windows 2000. =)

- 512x512 textures are not supported by all video cards.  You can resize them to 256x256 with an imaging program if you have problems.

______________________________________
last modified 4/12/2002 by:
Steve Wortham
website: www.gldomain.com
email: steve@gldomain.com