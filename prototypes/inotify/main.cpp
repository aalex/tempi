#include <stdio.h>
#include <string.h>
#include <iostream>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <unistd.h> // sleep

/*
 * libinotifytools example program.
 * Compile with g++ -linotifytools example.c
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: ./run filename" << std::endl;
        return 1;
    }
    std::string fileName(argv[1]);

    if (! inotifytools_initialize())
    {
        std::cerr << "ERROR: "; // calling inotifytools_initialize(): ";
        std::cerr << strerror(inotifytools_error()) << std::endl;
        return 1;
    }
    int event_types = IN_CLOSE_WRITE; // IN_MODIFY

    std::cout << "Will watch file \"" << fileName << "\"." << std::endl;
    if (! inotifytools_watch_file(fileName.c_str(), event_types))
    {
        std::cerr << "ERROR: "; // << calling inotifytools_watch_file(\"" <<
        //    fileName << "\", " << event_types << "): ";
        std::cerr << strerror(inotifytools_error()) << std::endl;
        return 1;
    }

    //// set time format to 24 hour time, HH:MM:SS
    inotifytools_set_printf_timefmt((char *) "%T");

    // Output all events as "<timestamp> <path> <events>"
    struct inotify_event *event = NULL;
    std::cout << "watching file \"" << fileName << "\"..." << std::endl;
    while (true)
    {
        event = inotifytools_next_event(0); // 0 = non-blocking
        if (event)
        {
            std::cout << "EVENT: ";
            inotifytools_printf(event, (char *) "%T %w%f %e\n");
        }
        else
        {
            std::cout << "no event...\n";
        }
        event = NULL;
        // std::cout << "zzz ";
        //sleep(1);
    }
    return 0;
}

