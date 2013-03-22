INFO
----
Levels made using GLEED2D : http://gleed2d.codeplex.com/
Level loader implemented using rapidxml : http://rapidxml.sourceforge.net/
Authors: Elliot Morris, Mark Thompson.
For the level loader to work properly, the structure of level in the editor must follow the *strict* rules below.
View the 'Default.xml' level file for an example of a correctly formatted and structured level.



LAYERS
------
Layers must be named and in the order Background > Platforms > Foreground > Bounds.
All layers should only contain textures, except the Bounds layer which should only contain rectangular primitives.

Background 		- 	Rendered behind all game objects.
Platforms 		- 	Rendered behind some game objects.
Foreground 		- 	Used for decals and interactive objects. Rendered in front of game objects.
Bounds 			- 	Defines the of the collision bounds used for server collision detection. Not rendered.
				
				
				
PROPERTIES
----------
SpawnPoint: To create a spawn point, drag the desired texture onto the Foreground layer then give it a custom property named "SpawnPoint" of type "Free text".
Visiblity: To make a texture invisible you must add a custom property to it named "Invisible" of type "Free text".
Scale: You can scale an image within the editor without having to modify the original image by changing the Scale parameter.



GLEED2D SETTINGS
----------------
Make sure to set the DefaultContentRootFolder via Settings to the projects assets folder you are working from.
In addition to this, you must manually edit the .xml file <ContentRootFolder> tag with this as well (as pointed out by the editor).