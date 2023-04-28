all: compile link run

compile:
	g++ -c src/*.cpp src/Pixels/*.cpp -IC:\libraries\SFML-2.5.1\include -DSFML_STATIC


link:	# add "-mwindows" on the end when you actually wanna build it, Kyle
	g++ -g -Wall *.o -o main.exe -LC:\libraries\SFML-2.5.1\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main

run:
	.\main.exe

clean:
	del *.o *.exe