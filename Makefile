default:
	g++ /usr/local/include/GLFW/deps/glad.c *.cc -I/usr/local/include/GLFW/deps -I/usr/local/include/GLFW/ -L/usr/lib/x86_64-linux-gnu/ -Wl,--no-as-needed -lglfw3 -lGLU -lGL -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -std=c++11 -o simple2

clic:
	g++ /home/cs4160/glfw/deps/glad.c *.cc -I/home/cs4160/glfw/include/ -I/home/cs4160/glfw/deps -L/usr/lib/x86_64-linux-gnu/ -L/home/cs4160/glfw/lib/ -Wl,--no-as-needed -lglfw3 -lGLU -lGL -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -std=c++11 -o simple2

