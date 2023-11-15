# Cheetah
Stealth-action game prototype

This project is inspired by: "Tom Clancy's Splinter Cell", "Hitman", "Mark of the Ninja", "Tenchu".

Objective - is to reach final location in the level that marked green color
Sneak past all enemies like a ghost or stealthily kill everyone on your way like a ~~panther~~ cheetah
You have a gun, but you can't kill enemies with it. You can only use the gun to destroy lamps that help enemies spot you.
In addition to lamps, enemies have personal flashlights, so try to avoid coming face to face with enemies as they will easily defeat you in direct confrontation.

Light sources such as lamps and enemy flashlights increase your illumination level. 
The amount of illumination depends on how close you are to the light source and how close you are to the direct direction of the light source.
Distance, of course, is more important - in a step away from the lamp, it almost doesn't matter how far you are from the line of direct direction of the light source, as long as you enter into the cone of light.
At longer distances this difference is more noticeable, but as the distance increases, the overall level of illumination decreases.
It also takes into account how many parts of your body enter into the cone of illumination.

There is also global light from the sun. 
The strength of its illumination also depends on the number of body parts caught in the light and on the height of the sun above the horizon. 
You can lower the sun completely or create shadows from objects in the desired direction by rotating the sun.

The overall level of your illumination can be observed by the brightness of the "Unreal" symbol on the main character's chest and by a small light sensor on the back.

Enemies will not be able to see you if you are completely in the shadows.
In addition to lighting, the detection of you by enemies is also affected by the distance between you, your distance from the enemy’s straight line of sight, your movement speed and whether you are crouched or not.
The effect of each parameter can be adjusted independently.
There is also an overall detection coefficient, which is used as the difficulty of the game.

An enemy can patrol along the points assigned to him either cyclically or from edge to edge.
Getting into the enemy's field of view increases his awareness of you depending on the level of detection.
When awareness reaches its maximum, the enemy will become suspicious, stop patrolling and head towards your location. 
If you are still in his view cone, his awareness that you are an intruder will increase. 
Once its maxed out, the enemy will become aggressive and run towards you to eliminate you.

If the enemy loses sight of you while being aggressive, he will search for you for a while, but then return to patrolling.
Lost sight of you being in suspicious mood (without realizing that you are an intruder), the enemy will check the last place he saw you and return to patrolling.

As already said, you can destroy the lamps to make the area safer.
But be careful, because even though your pistol is silenced, the explosion of the lamp creates a lot of noise and will attract the enemies attention.
Notice that if the enemy sees you, then extraneous sounds will not distract him from the chase.
Besides lamps, you can also distract enemies by throwing a rock. Unfortunately, you only have one, so you will have to find and pick it up for a new throw.
In addition to the lamps and rock, you can also make noise if you run. Enemies may not see you in the dark and through the wall, but they will hear you, so be careful!

Controls:
*WASD						Move
*Space					Jump
*Mouse					Look
*Ctrl						Crouch
*Shift					Run
*Mouse Wheel		Zoom
*Mouse RBT			Aim
*Mouse LBT			Shoot
*F							Kill
*G							Throw

### *Special thanks to Martin Walsh*
