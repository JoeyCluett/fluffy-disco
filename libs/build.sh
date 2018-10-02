g++ -c -o FloatRect.o  FloatRect.cpp        -I../ -std=c++11 -march=native -O3 -lSDL
echo "FloatRect compiled..."

g++ -c -o Polygon.o    Polygon.cpp          -I../ -std=c++11 -march=native -O3 -lSDL
echo "Polygon compiled..."

g++ -c -o HitBox.o     HitBox.cpp           -I../ -std=c++11 -march=native -O3 -lSDL
echo "HitBox compiled..."

g++ -c -o Projectile.o Projectile.cpp       -I../ -std=c++11 -march=native -O3 -lSDL
echo "Projectile compiled..."

g++ -c -o MineExplosion.o MineExplosion.cpp Animation.o -I../ -std=c++11 -march=native -O3 -lSDL
echo "MineExplosion compiled..."

g++ -c -o TextTag.o TextTag.cpp       -I../ -std=c++11 -march=native -O3 -lSDL
echo "TextTag compiled..."

