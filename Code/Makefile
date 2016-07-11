build-dir := ../Build
deps-lib-dir := ../Dependencies/lib
deps-inc-dir := ../Dependencies/include
includes := -I$(deps-inc-dir)
CFLAGS := -std=c++11 -m64
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include $(includes)
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OUTPUT := -g -o $(build-dir)/simpleGame
LINKER_FLAGS = -framework OpenGL -lglfw3 -lassimp -m64
BUILD_DEPS = $(build-dir)/Main.o $(build-dir)/glad.o $(build-dir)/Shader.o $(build-dir)/Texture.o $(build-dir)/Camera.o $(build-dir)/Mesh.o $(build-dir)/Model.o
start:
	g++ $(CFLAGS) -c Main.cpp $(includes) -o $(build-dir)/Main.o
	g++ $(CFLAGS) -c Shader.cpp $(includes) -o $(build-dir)/Shader.o
	g++ $(CFLAGS) -c Texture.cpp $(includes) -o $(build-dir)/Texture.o
	g++ $(CFLAGS) -c Camera.cpp $(includes) -o $(build-dir)/Camera.o
	g++ $(CFLAGS) -c Mesh.cpp $(includes) -o $(build-dir)/Mesh.o
	g++ $(CFLAGS) -c Model.cpp $(includes) -o $(build-dir)/Model.o
	g++ $(CFLAGS) -c $(deps-lib-dir)/glad.cpp $(includes) -o $(build-dir)/glad.o
	g++ $(CFLAGS) $(BUILD_DEPS)  $(LIBRARY_PATHS) -L$(deps-lib-dir) $(LINKER_FLAGS) $(includes) $(INCLUDE_PATHS) $(OUTPUT)
clean:
	rm -rf $(build-dir)/*.o
