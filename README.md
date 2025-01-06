Vulkan & Graphics Learning project
===

I'm trying to learn many things within one project: 
- C++ 
- Vulkan
- How graphics are working
- How a game engine works
- Create some experiments while improving / rewriting parts of the engine

If you would like to spare some time with Vulkan or you feel you can reuse something from the engine feel free, currently I'm not intending to close the source at all, maybe just a future game I would like to work on will be closed source, but never the engine. 
**I have no intent to create a widely used engine, this is just a disclaimer for you, so everything is as is.** There are a lot of publicly available engines if you would like to touch on game development, I would advise you to take a look at those. 

If you are still here that means you want to try it out maybe. I'll try not to forget to update the below instructions in the future. 

# Prerequisites
Using C++ so you'll need to have a working compiler and C++ environment being set up, I won't detail it, it's your decision how to do it. 
This project uses CMake, please install it before proceeding.  
In order to use Vulkan you'll need to install the [SDK](https://vulkan.lunarg.com/sdk/home) first for your OS. 

If your setup is working correctly you should be able to build the project. (Be aware, the following commands are for MacOS but should be easy to adapt to your OS of choice) 
1. create a directory called `build`
2. go to `build` directory
3. `cmake ../`

The build should be done without errors, if something wrong happens it's probably due to your Vulkan or CMake setup ("probably", I'm still a noob on this field).
Now we'll need to compile the two shaders. 

4. Inside `build` directory create a `shaders` sub-dir. 
5. While staying in `build`

```sh
glslc ../shaders/triangle.vert -o shaders/vert.spv
glslc ../shaders/triangle.frag -o shaders/frag.spv
```

6. Make textures & models available 
```sh
cp -R ../{models,textures} .
```

> **NOTE** 
> `glslc` should come with VulkanSDK. Check your setup of the SDK if it's unavailable.

7. Run DarkestPlanet binary. 

As of this moment the "engine" is in a state how the Vulkan tutorial helped me, meaning you have two shaders: a fragment shader and a vertex shader, you'll need to compile them and copy them over to the build folder.



