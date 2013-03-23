INFO
----
Levels made using GLEED2D : http://gleed2d.codeplex.com/
Level loader implemented using rapidxml : http://rapidxml.sourceforge.net/
Authors: Elliot Morris, Mark Thompson.
For the level loader to work properly, the structure of level in the editor must follow the *strict* rules below.
View the 'Default.xml' level file for an example of a correctly formatted and structured level.



LAYERS
------
Layers must be named and in the order Background > Entities > Platforms > Foreground > Bounds.
All layers should only contain textures, except the Bounds layer which should only contain rectangular primitives. Special case: Entities layer - LevelBounds object can be defined with a primitive.

Background 		- 	Rendered behind all game objects.
Entities		- 	Contains spawnpoints and levelbounds.
Platforms 		- 	Rendered behind some game objects.
Foreground 		- 	Rendered in front of all game objects.
Bounds 			- 	Defines the collision bounds used for server collision detection. Not rendered.
				
				
				
PROPERTIES
----------
Lethal: Can only be used in the Bounds layer. Any players that collide with and object with this property defined will die. Custom property named "Lethal" of type "Free text".
LevelBounds: Can only be used in the Entities layer. This should only be defined on one primitive which defines the level bounds. This area defines the movement space of players. Custom property named "LevelBounds" of type "Free text".
SpawnPoint: Can only be used in the Entities layer. Defines player spawn locations within the map. Custom property named "SpawnPoint" of type "Free text".
Invisible: Prevents texture from being rendering in game. Custom property named "Invisible" of type "Free text".
Scale: Images can be scaled within the editor without having to modifiy the original image by altering the Scale property.



GLEED2D SETTINGS
----------------
Make sure to set the DefaultContentRootFolder via Settings to the projects assets folder you are working from.
In addition to this, you must manually edit the .xml file <ContentRootFolder> tag with this as well (as pointed out by the editor).