# Godot TrackIR Plugin

This plugin will allow you to use TrackIR head tracking in Godot projects. The TrackIR SDK for Godot includes sample games for easy examples. 

The SDK also includes a minimal package file in [releases](https://github.com/TrackIR/TrackIR-SDK-Godot/releases/latest) that you can add to your own Godot projects.

# Getting Started

## Example Project
Clone or download the repository to your local machine. Then, open the example project (found in the `example-project/` folder) in Godot. Make sure that the TrackIR software is running on your computer and then run the demo scene. You should see the headtracking data being used to control the camera movement. You can also run the game scene found in the `Platformer/` folder to see an example of how TrackIR head tracking can be used in a more complex setting.

## Adding the Plugin to your Project
Alternatively, you can simply import the Godot plugin into your own Godot project. The plugin contains all the necessary scripts and components to use the SDK in your own projects.

1. Create a Godot project. In the root directory of the project create an `addons/` folder.
2. Download the OptiTrack Plugin from GitHub. Follow the link to the latest release under the "Releases" section of the right sidebar. From the list of assets download the `optitrack_plugin.zip` archive.
3. Extract the zip archive and copy it into the `addons/` folder of your Godot project.
4. When you add these files into your project the Godot editor may report errors as it parses the files. This is because the plugin has not yet been enabled.
5. Open the project's settings by selecting **Project** > **Project Settings** from the main menu.  Switch to the **Plugins** tab. The OptiTrack plugin should be listed under **Installed Plugins**. Enable it by checking the box to the left of its name.
6. Reload the project by selecting **Project** > **Reload Current Project** from the menu.

# Using the Plugin

To use this SDK, you will need a TrackIR headset and the TrackIR software installed on your computer. You can download the TrackIR software from the website: https://www.trackir.com/developers.

Add the plugin to your own Godot project as described in the "Adding the Plugin to your Project" section above. In the `trackir_plugin/` folder you will find the `trackir_camera_control.gd` script. This is the script that gets data from the TrackIR software. Attaching this script to a Camera3D node will make it track head movements.

Typically, you will want more complex camera movement in your game. For example, it is very common to have the camera follow the player. The easiest way to accomplish this is to put your custom camera control behavior in a script attached to a Node3D node. Then add a Camera3D node as a child to that node and attach the `trackir_camera_control.gd` script. The Camera3D will inherit the movement of the Node3D node and the `trackir_camera_control.gd` script will add head tracking movements on top of it.

If TrackIR Software is not running when the game starts, the `trackir_camera_control.gd` script will not make any changes to camera movements, but other scripts (e.g., on the camera's parent node) can still provide camera motion. If tracking is lost while the game is running, the camera will recenter itself after a timeout period (default 3 seconds). If tracking is regained, the script will resume camera control.

Note: After installing the plugin, you may notice a global autoload named "TrackIR" in your project settings. This autoload is essential to the plugin's function. Do not disable it.

## Properties

After adding the `trackir_camera_control.gd` script to a node, three exported properties will be available to edit in the Inspector:
- **Max Offset**: This property controls how far the camera will move. This property only affects position. It does not affect rotation. 
- **Tracking Lost Timeout**: This property controls the number of seconds without new data from TrackIR before tracking is considered lost and the camera recenters itself.
- **Recenter Time**: This property controls how quickly the camera returns to center when tracking is lost.

# How to Start Developing

To start developing this plugin:

1. Download the latest version of Godot.
2. Obtain a copy of the source code from this repository (see GitHub's guide to [cloning a repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository) for help).
3. If you intend to edit the C++ source code of the plugin, you will need a C++ compiler and SCons, a Python-based build tool (for instructions for installing Scons refer to the [Scons GitHub repository](https://github.com/SCons/scons?tab=readme-ov-file#installation))

## Project Organization

The TrackIR plugin has a few major components:

The `optitrack_plugin/` folder contains the OptiTrack plugin. This plugin autoloads an instance of the TrackIRClient class into the project. Alongside the other files necessary for the plugin to work, this folder contains the `trackir_camera_control.gd` script. It is written in GDScript. The Godot editor provides the best support for GDScript development, so editing these files in Godot from the `example-project/addons/optitrack_plugin/` folder is recommended. SCons will copy the files from there to the `optitrack_plugin/` directory when you run the build script (`scons`).

The `src/` directory contains the code for the TrackIRClient class. This class is written in C++ using [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/cpp/gdextension_cpp_example.html). It handles the connection to TrackIR using OptiTrack's [TrackIR SDK](https://docs.trackir.com/trackir-sdk).

The `include/` directories are for headers files, i.e., those from the TrackIR SDK. 

The `godot-cpp/` is a submodule that contains all the code for Godot. It is necessary for compiling the GDExtension code (see the [GDExtension C++ tutorial](https://docs.godotengine.org/en/stable/tutorials/scripting/cpp/gdextension_cpp_example.html)).

## Resources

Godot has a lot of helpful documentation and tutorials for developing plugins. The following pages are especially useful for working on this plugin:
- [GDExtention C++](https://docs.godotengine.org/en/stable/tutorials/scripting/cpp/gdextension_cpp_example.html)
- [Making Plugins](https://docs.godotengine.org/en/stable/tutorials/plugins/editor/making_plugins.html)
- [Inspector Plugins](https://docs.godotengine.org/en/stable/tutorials/plugins/editor/inspector_plugins.html)
- [GDScript Reference](https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_basics.html)

# License

See the appropriate license file for more details. 
