#include "gmusicapi/Module.h"
#include "gmusicapi/Mobileclient.h"
#include <algorithm>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#else
#include <termios.h> 
#include <unistd.h>
#endif

void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable)
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}


int main()
{
    std::string username, password;
    std::cout << "Email:";
    std::cin >> username;
    std::cout << "Password:";
    SetStdinEcho(false);
    std::cin >> password;
    std::cout << '\n';
    SetStdinEcho(true);

    try
    {
        GMusicApi::Module module;
        GMusicApi::Mobileclient mc(module);
        mc.login(username, password, "1234567890abcdef");

        auto library = mc.get_all_songs();
        GMusicApi::identifiers sweet_track_ids;
        for (auto&& song : library)
        {
            if (song.artist == "The Cat Empire")
            {
                sweet_track_ids.push_back(song.id);
            }
        }

        auto playlist_id = mc.create_playlist("Rad muzak");
        mc.add_songs_to_playlist(playlist_id, sweet_track_ids);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}