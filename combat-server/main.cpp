#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <thread>

// custom server libs
#include <lib/TcpServer.h>

// custom libs elsewhere
#include <tcp/Client.h>
#include <tcp/Packet.h>

using namespace std;

void serverCallback(int fd);
vector<int> fd_list;

int main(int argc, char* argv[]) {

    TcpServer server("localhost", 13000);
    server.setConnectionCallback([](int fd) -> void {
        cout << "Client fd: " << fd << endl;
        
        int i_arr[3];
        string hw;

        Packet packet;
        packet.Read(fd);

        // chain together multiple reads (including an array!)
        packet >> ARR(INTEGER_P, 3, i_arr) >> hw;

        cout << "Client sends: ";
        for(int i = 0; i < 3; i++)
            cout << i_arr[i] << ' ';
        cout << hw;
        cout << endl;

    });

    thread newThread([](TcpServer* server) -> void {
        server->start(2);
    }, &server);

    // give server time to spin up
    usleep(1000000);

    thread clientThread([](void) -> void {
        TcpClient client("localhost", 13000);

        Packet packet;

        // chain together multiple writes
        packet << 1 << 2 << 3 << "They're building their house over there...";
        packet.Write(client.getFd());

        while(1) {
            // sleep until program termination
            sleep(1000000);
        }
    });

    cout << "LOOP END\n";
    usleep(3000000);

    return 0;
}

void serverCallback(int fd) {
    cout << "Client FD: " << fd << endl;
    fd_list.push_back(fd);
}
