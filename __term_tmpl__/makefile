src = $(wildcard *.cpp)
hdr = $(wildcard *.hpp)

CXXFLAGS = -g -std=c++17 -Wall -Wextra -Wshadow -pedantic -D_GLIBCXX_DEBUG
CXXRLSFLAGS = -O2 -std=c++17 -Wall -Wextra -Wshadow -pedantic

main: $(src) $(hdr)
	$(CXX) -o main $(CXXFLAGS) $(src)

.PHONY: release
release:
	$(CXX) -o main $(CXXRLSFLAGS) $(src)

.PHONY: clean
clean:
	rm -f main