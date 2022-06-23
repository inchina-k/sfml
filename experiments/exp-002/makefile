src = $(wildcard *.cpp)
hdr = $(wildcard *.hpp)

CXXFLAGS = -g -std=c++17 -Wall -W -Wextra -Wshadow -DAUCA_DEBUG -D_GLIBCXX_DEBUG
CXXRELEASEFLAGS = -O2 -std=c++17 -Wall -W -Wextra -Wshadow
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lopengl32 -lwinmm -lgdi32 -lfreetype -lopenal -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32

main: $(src) $(hdr)
	$(CXX) -o main $(CXXFLAGS) $(src) $(SFMLLIBS)

.PHONY: release
release:
	$(CXX) -o main $(CXXRELEASEFLAGS) $(src) -mwindows $(SFMLLIBS) 

.PHONY: clean
clean:
	rm -f *.exe