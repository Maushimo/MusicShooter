g++ *.cpp -F/Library/Frameworks -framework SDL2 -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_gfx -std=c++11 -o game

if [[ $? -eq 0 ]]; then
	./game
fi
