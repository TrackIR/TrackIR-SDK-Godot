# About this Game

This game is based on the 3D Platformer demo game from the [Godot Demo Projects repository](https://github.com/godotengine/godot-demo-projects) maintained by the Godot Developers. The code is available under an MIT [license](https://github.com/godotengine/godot-demo-projects/blob/master/LICENSE.md). The 3D Platformer game can be found in the Godot Asset Library or in the Godot Demo Projects repository [here](https://github.com/godotengine/godot-demo-projects/tree/master/3d/platformer).

The only changes necessary to add TrackIR camera control were:
- Adding and `addons/` folder and installing the `trackir_plugin/` in it.
- Enabling the plugin in the Project Settings
- Duplicating the Camera3D node in the player scene, making it a child of the original and attaching the `trackir_camera_control.gd` script to it. Setting the "Current" property of the child Camera3D in the inspector and unchecking this property on the parent Camera3D node.