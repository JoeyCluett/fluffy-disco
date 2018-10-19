#include <iostream>
#include <thread>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

using namespace std;

void server(void);

int main(int argc, char* argv[]) {

    if(SDL_Init(0) == -1) {
        cout << "SDL error: " << SDL_GetError() << endl;
        return 1;
    }

    if(SDLNet_Init() == -1) {
        cout << "SDL_net error: " << SDLNet_GetError() << endl;
        return 1;
    }

    SDL_version compile_version;
    const SDL_version* sdl_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);

    cout << "Compiled with: " << endl;
    cout 
            << (int)compile_version.major << '.'
            << (int)compile_version.minor << '.'
            << (int)compile_version.patch << "\n\n";
    
    cout << "Linked with: " << endl;
    cout
            << (int)sdl_version->major << '.'
            << (int)sdl_version->minor << '.'
            << (int)sdl_version->patch << "\n\n";

    thread server_thread(server);

    SDL_Delay(500); // give server a little bit of time to spin up

    IPaddress ip_address;
    TCPsocket tcp_socket;

    if(SDLNet_ResolveHost(&ip_address, "localhost", 13000) == -1) {
        cout << "SDLNet_ResolveHost : main error -> " << SDLNet_GetError() << endl;
        exit(1);
    }

    

    SDLNet_Quit();
    SDL_Quit();

    return 0;
}

void server(void) {
    IPaddress ip_address;
    TCPsocket tcp_socket;

    // this thread is a server listening to port 13000
    int ret = SDLNet_ResolveHost(&ip_address, NULL, 13000);
    if(ret == -1) {
        cout << "SDLNet_ResolveHost : server error -> " << SDLNet_GetError() << endl;
        exit(1);
    }



}

