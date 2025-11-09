// For all networking
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <windows.h> // For console colors

using boost::asio::ip::tcp;
using namespace std;

// Color defines
#define GREEN    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RED      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY)
#define YELLOW   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CYAN     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define MAGENTA  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define RESET    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

map<string, tcp::socket*> users;
mutex lock_users;

void broadcast(string msg) {
    lock_guard<mutex> lock(lock_users);
    for (auto const & [username, client_socket] : users) {
        boost::asio::write(*client_socket, boost::asio::buffer(msg));
    }
}

void clients (tcp::socket client_socket) {
    string username;
    try {
        char data[1024];
        size_t length = client_socket.read_some(boost::asio::buffer(data, sizeof(data)));
        username.assign(data, length);

        {
            lock_guard<mutex> lock(lock_users);
            users[username] = &client_socket;
        }

        broadcast(username + " has joined the chat. \n");
        GREEN; cout << "[INFO] " << username << " connected." << endl; RESET;

        while (true) {
            length = client_socket.read_some(boost::asio::buffer(data, sizeof(data)));
            string message(data, length);
            if (message[0] == '@') {
                size_t pos = message.find(' ');
                string touser = message.substr(1, pos -1 );
                string private_message = "Private Message from " + touser  + " -> " + message.substr(pos + 1) + "\n";
                {
                    lock_guard<mutex> lock(lock_users);

                    if (users.count(touser)) {
                        boost::asio::write(*users[touser], boost::asio::buffer(private_message));

                    } else {
                        string err = "User not found -> " + touser;
                        RED; cout << "[ERROR] " << username << " tried to message " << touser << endl; RESET;
                        boost::asio::write(client_socket, boost::asio::buffer(err));
                    }
                }
            } else {
                broadcast("[" + username + "] -> " + message + "\n");
            }
        }
    } catch (exception& e) {
        RED; cout << "[INFO] " << username << " disconnected." << endl; RESET;
        {
            lock_guard<mutex> lock(lock_users);
            users.erase(username);
        }
        broadcast(username + " has left the chat.");
    }
}

int main() {
    int port = 8085;
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

    GREEN; cout << "============================================" << endl; RESET;
    CYAN; cout << " SERVER STARTED" << endl; RESET;
    YELLOW; cout << " Listening on port: " << port << endl; RESET;
    GREEN; cout << "============================================" << endl; RESET;

    while (true) {
        tcp::socket socket (io_context);
        acceptor.accept(socket);

        CYAN; cout << "[INFO] New connection accepted." << endl; RESET;

        thread (clients, move(socket)).detach();
    }
    system("PAUSE");
    return 0;
}
