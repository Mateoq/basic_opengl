build-dir := ../Build
deps-lib-dir := ../Dependencies/lib
deps-inc-dir := ../Dependencies/include
includes := -I$(deps-inc-dir)
CFLAGS := -std=c++11
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include $(includes)
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OUTPUT := -g -o $(build-dir)/simpleGame
LINKER_FLAGS = -framework OpenGL -lglfw3
init:
	g++ $(CFLAGS) -c Main.cpp $(includes) -o $(build-dir)/main.o
	g++ $(CFLAGS) -c $(deps-lib-dir)/glad.cpp $(includes) -o $(build-dir)/glad.o
	g++ $(CFLAGS)  $(build-dir)/main.o $(build-dir)/glad.o $(LIBRARY_PATHS) $(LINKER_FLAGS) $(includes) $(INCLUDE_PATHS) $(OUTPUT)
clean:
	rm -rf $(build-dir)/*.o
