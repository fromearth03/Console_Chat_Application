// ================= CLIENT =================
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>
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

void receive_messages(tcp::socket& socket) {
    try {
        char data[1024];
        while (true) {
            size_t length = socket.read_some(boost::asio::buffer(data, sizeof(data)));
            string msg(data, length);

            if (msg.find("joined the chat") != string::npos) { GREEN; cout << msg; RESET; }
            else if (msg.find("left the chat") != string::npos) { RED; cout << msg; RESET; }
            else if (msg.find("Private Message") != string::npos) { MAGENTA; cout << msg; RESET; }
            else { CYAN; cout << msg; RESET; }
        }
    } catch (exception& e) {
        RED; cout << "[INFO] Disconnected from server." << endl; RESET;
    }
}

int main() {
    string host = "127.0.0.1";
    int port = 8085;

    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::make_address(host), port));

        YELLOW; cout << "Enter your username: "; RESET;
        string username;
        getline(cin, username);

        boost::asio::write(socket, boost::asio::buffer(username));

        thread(receive_messages, ref(socket)).detach();

        GREEN; cout << "[INFO] Connected to chat. Type messages below." << endl; RESET;
        YELLOW; cout << "Use '@username message' for private messages." << endl; RESET;

        while (true) {
            string message;
            getline(cin, message);
            if (message == "/quit") break;
            boost::asio::write(socket, boost::asio::buffer(message));
        }

        socket.close();
    } catch (exception& e) {
        RED; cout << "Error: " << e.what() << endl; RESET;
    }

    return 0;
}
