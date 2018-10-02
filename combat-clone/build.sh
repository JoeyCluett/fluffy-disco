g++ -g -c -o main.o main.cpp -I../ -std=c++11 -march=native -O3

g++ -o main main.o ../libs/FloatRect.o ../libs/Polygon.o ../libs/HitBox.o \
../libs/Projectile.o ../libs/MineExplosion.o ../libs/TextTag.o \
-std=c++11 -march=native -O3 -lSDL -lSDL_gfx -lSDL_ttf
