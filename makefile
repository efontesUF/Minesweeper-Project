build:
	g++ *.cpp \
	-lsfml-graphics \
	-lsfml-window \
	-lsfml-system \
	--std=c++11 \
	-o sfml-app
