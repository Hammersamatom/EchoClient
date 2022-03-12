#include <fmt/core.h>
#include <socks/socks.hpp>

#include <vector>
#include <cstdint>

#include "commandline.h"

int main(int argc, char* argv[])
{
    Commandline c_io;

    // Error out if there isn't an address to connect to, and a name isn't selected
    if (argc < 3)
    {
        c_io.write("Missing parameters: IP and Name");
        return 1;
    }

    c_io.write("Initializing...");
    c_io.set_prompt(fmt::format("{}: ", argv[2]));

    sks::socket server_socket(sks::IPv4, sks::stream);
    sks::address server_addr(argv[1]);

    c_io.write(fmt::format("Server address is {}, attempting connection", server_addr.name()));

    try {
        server_socket.connect(server_addr);
    }
    catch (std::exception& e)
    {
        c_io.write(e.what());
        return 1;
    }





    // Infinite loop LMAO
    while (true)
    {
        if (c_io.has_command())
        {
            std::string hold = c_io.get_command();

            //c_io.write(fmt::format("{}: {}", argv[2], hold));
            if (hold == "Break")
            {
                break;
            }
            else
            {
                if (server_socket.writeReady())
                {
                    std::string msg = std::string(argv[2]) + ": " + hold;
                    std::vector<uint8_t> bytes = std::vector<uint8_t>(msg.begin(), msg.end());
                    server_socket.send(bytes);
                }
            }
        }

        if (server_socket.readReady())
        {
            std::vector<uint8_t> bytes = server_socket.receive();
            c_io.write(std::string(bytes.begin(), bytes.end()));
        }
    }

    return 0;
}