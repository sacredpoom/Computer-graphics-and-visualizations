# Computer-graphics-and-visualizations
A basic 3D scene with textured primitives and Phong model lighting constructed with OpenGL.

![FinalScreen](https://github.com/sacredpoom/Computer-graphics-and-visualizations/assets/20672168/27454834-cfeb-43e0-b608-ea1c3af87441)
![3D_scene](https://github.com/sacredpoom/Computer-graphics-and-visualizations/assets/20672168/2c8229da-b5fa-4ce6-9130-a47df297bdff)


## About:
The objective of this project was to employ OpenGL for the purpose of generating a 3D environment, relying solely on basic geometric shapes to construct intricate objects. A Phong lighting system, encompassing ambient, diffuse, and specular lighting calculations, was used to introduce two light sources within the scene. Custom shape classes handle vertex array and buffer objects with modularized render function to add primitives individually. Users are able to navigate the scene using W, A, S, D keys as well as vertical camera movement with Q and E keys. View of the scene can be switched from perspective projection to orthographic with the P key. Mouse handles view direction as well as speed using the scroll-wheel.

## Tools Used: 
* OpenGL - Used for rendering 3D scene
* Visual Studio - Program written in C++ and GLSL for shaders
* GLAD - loader generator
* GLEW - provides acces to GL entrypoints 
* GLFW - handles window creation and event handler
* glm - used for vector calculations
  
_How do I approach designing software?_ \
The first thing necessary when developing software is establishing and understanding all requirements. This project was broken down into smaller parts where each successive build was presented to the 'client' and had feedback before continuing. Developing modular code helped with testing and debugging, allowing issues to be pinpointed and addressed quickly. Utilizing prototypes for gathering client feedback promotes engagement and results in an end product matching what the client desires. The incremental development process utilized in this work will prove a useful strategy in future projects. 

_How do I approach developing programs?_ \
I approach new projects by outlining all requirements before beginning. I figure out what sort of frameworks and libraries will be applicable and serve the project needs. Once development begins I stick to coding best practices such as utilizing frequent comments and formatting for readability, modular design, and object-oriented principles such as encapsulation, inheritance, and polymorphism. For this particular project all object data was abstracted to their own classes which helped to clean up the rendering function and maintain readable code. Aligning the primitives, adding textures, and implementing lighting into the scene was an iterative process which helped identify issues early on in development.  

_How can computer science help me in reaching my goals?_ \
This career promotes critical thinking and problem solving. The field is constantly changing with new technologies and better ways of doing things, requiring computer scientists to constantly adapt and continually learn throughout their career. Computer science helps to build these skills and promote an active learner mindset that will benefit in any future career position. This project gives a great foundation for working with data visualization and modeling tools in future projects. This was the largest project worked on so far and was a great experience for practicing iterative development as each stage was developed and tested before working on the next. This experience will lead to better project organization and documentation in the future.  
