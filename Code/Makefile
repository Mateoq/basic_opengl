build-dir := ../Build
deps-lib-dir := ../Dependencies/lib
deps-inc-dir := ../Dependencies/include
includes := -I$(deps-inc-dir)
init:
	g++ -std=c++11 -c Shader.cpp $(includes) -o $(build-dir)/Shader.o
	g++ -std=c++11 -c main.cpp $(includes) -o $(build-dir)/main.o
	g++ -std=c++11 -c $(deps-lib-dir)/glad.cpp $(includes) -o $(build-dir)/glad.o
	g++ -std=c++11 $(build-dir)/Shader.o $(build-dir)/main.o $(build-dir)/glad.o -lSDL2 $(includes) -std=c++11 -o simpleGame
simpleGame: $(build-dir)/Shader.o $(build-dir)/main.o $(build-dir)/glad.o
	g++ -std=c++11 $(build-dir)/Shader.o $(build-dir)/main.o $(build-dir)/glad.o -lGL -lSDL2 $(includes) -std=c++11 -o simpleGame
clean:
	rm -rf $(build-dir)/*.o
