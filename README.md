# UltimateOpenGL 

UltimateOpenGL is a 3D motor used to make 3D games made by a beginner. It's designed to be "code only". it mean you don't have (at this time) any GUI  to help you create scene with your mouse.
Futher more, it need Ultimate++ Framework (A powerful C++ Framework) and the ide used by it : [TheIDE](https://www.ultimatepp.org/index.html)

here is the Ultimate++ post : https://www.ultimatepp.org/forums/index.php?t=msg&goto=52755&#msg_52755

At this time, UOGL allow Scene creation, insertion of  multiple 'GameObject'(Shapes, Lights), insertion of multiple camera per scene. 

Things are coming : 
	Insertion of 3D Models using Assimp librairie
	Gestion of physics using Bullets3
	(Maybe) Création of sound object 

## Prerequisites 

First be sure to have Ultimate++ Framework and TheIDE.
 
You also need some extern librairies : 
GLM (OpenGL Mathematique) https://glm.g-truc.net/0.9.9/index.html

Glad (Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs) used to properly load OpenGL
http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c&loader=on

Assimp (The Open-Asset-Importer-Lib) Used to load 3D Models
http://www.assimp.org  (You must compile it by yourself on your computer, However if you have some trouble doing it, then ask me I will do a quick tutorial)

Glfw used to generate the screen and render the openGL Scene https://www.glfw.org

Bullet3 used for physique engine. https://github.com/bulletphysics/bullet3 (you must compile it by yourself as well)



## Getting Started and Showing a cube 

Here you can find a look of what it should look in your Builder setting (based on TheIDE)
![how my lib is setting](https://i.imgur.com/KfHdK6N.png)

First thing to do is create a new Project using Ultimate++ Core then add UltimateOpenGl to it :

![project starting](https://i.imgur.com/J2KJRjo.png)

Second thing is importing UltimateOpenGl and GLFW and create UltimateOpenGL context object as follow :
```
#include <Core/Core.h>
#include <UltimateOpenGL_V2/UltimateOpenGL.h>
#include <GLFW/glfw3.h>

using namespace Upp;

UltimateOpenGL_Context context; //Context carrying all scene and object (Basicly context is a game)

CONSOLE_APP_MAIN
{
}
```
You also need to add GLFW lib to linker setting of the project. To do this, right click your package and goto Package organiser, then right click the package option field and select "new Librairie" then type in ":" fields : 'GLFW3' as follow :
![Adding Glfw3](https://i.imgur.com/P9eo1Al.png)


here we go, we are now ready to do some code, first of all we need to set up all the glfw window setup. (I won't dig the process, if you want to know more about glfw window and is initialisation, you must go here :https://learnopengl.com/Getting-started/Hello-Window )
```
#include <Core/Core.h>
#include <UltimateOpenGL_V2/UltimateOpenGL.h>
#include <GLFW/glfw3.h>

using namespace Upp;

UltimateOpenGL_Context context; //Context carrying all scene and object (Basicly context is a game)

CONSOLE_APP_MAIN
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    context.SetSceneSize(800.0f,600.0f); // tel the context size of our window
	
    GLFWwindow* window = glfwCreateWindow(800.0f, 600.0f,"Hello UltimateOpenGL", NULL, NULL);
    if (window == NULL)
    {
        Cout() << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        Exit(-1);
    }
    glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        Cout()  << "Failed to initialize GLAD" << "\n";;
        Exit(-1);
    }
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.4f,0.5f,0.8f, 1.0f); //Set background color of our window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear all pixel on the window
		
	    glfwSwapBuffers(window); 
	    glfwPollEvents();
	}
	
	glfwTerminate();
	
}
```

If you now try the code, you should see a blue window :
![BlueWindow](https://i.imgur.com/ARVgBCC.png)

Let's now add A simple cube to this window :

```
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        Cout()  << "Failed to initialize GLAD" << "\n";;
        Exit(-1);
    }
    
    
    //************New code ***********//
    Scene& presentation = context.AddScene("presentation"); //Create Scene 
    unsigned int camera = presentation.AddCamera(); //Adding camera to the scene
    Object3D& cube = presentation.CreateGameObject<Object3D>("cube",CubeVertices); //Adding the cube
    cube.GetTransform().SetNewPosition(glm::vec3(0.0f,0.0f,-1.0f)); // move the cube forward the camera

    presentation.Load(); //Loading the scene
    

    while(!glfwWindowShouldClose(window)) {
        
        glClearColor(0.4f,0.5f,0.8f, 1.0f); //Set background color of our window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear all pixel on the window
		
		context.Draw(); //Drawing the context (active scene)
	
	    glfwSwapBuffers(window); 
	    glfwPollEvents(); 
	}
	
	glfwTerminate();
```

Well, it's not that's graphic, we must make an even to let him rotate every frame !

```
    presentation.Load(); //Loading the scene
    
    
    //***Adding event on Draw to the cube
    cube.SetOnDrawFunction([](GameObject& myGameObject){
    	myGameObject.GetTransform().RotateFromEulerAngles(0.001f,glm::vec3(1.0f,1.0f,1.0f)); // rotating of 0.001 degree every frame
    });
    
```

now, you should see a rotating cube on the screen !

![Rotating](https://i.imgur.com/3hBANRs.png)

This exemple is available here : https://github.com/Xemuth/SimpleExampleUltimateOpenGL

You can find another exemple of working code here : https://github.com/Xemuth/ExempleUltimateOpenGL_V2

![Ex1](https://i.imgur.com/bI2kepS.png)
![Ex2](https://i.imgur.com/KL2ixM6.png)

## Documentation and deep example 

Incoming

## Authors

Clément Hamon


## License

Read Licenses.txt 

## Acknowledgments

Thanks to https://learnopengl.com
Thanks to All external librairies owner 
Thanks to the Upp Team


