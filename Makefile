
CFLAGS = -std=c++17 -g
LDFLAGS = -L/opt/local/lib -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
#LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -framework Cocoa -framework OpenGL -framework IOKit


VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
