
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace movie_parser::utils
{
    static int progress_row = -1;

    void print_progress_top_right(int movies, int tags, int ratings, int movies_index, int tags_index, int ratings_index) {
        // Build progress string
        std::ostringstream oss;
        oss << "Movies: " << std::setw(3) << movies << "%"
            << " Tags: " << std::setw(3) << tags << "%"
            << " Ratings: " << std::setw(3) << ratings << "%"
            << " | M-Idx: " << std::setw(3) << movies_index << "%"
            << " T-Idx: " << std::setw(3) << tags_index << "%"
            << " R-Idx: " << std::setw(3) << ratings_index << "%";
        std::string text = oss.str();

        static size_t last_len = 0;
        size_t cur_len = text.size();
        if (cur_len < last_len) {
            text.append(last_len - cur_len, ' ');
        }
        last_len = cur_len;


        // Detect terminal width
        int width = 80; // fallback default
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
#else
        struct winsize w {};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
            width = w.ws_col;
        }
#endif

        int col = (width - static_cast<int>(text.size()) + 1);
        if (col < 1) col = 1;

        if (progress_row < 0) {
            // fallback: put it at the top row
            progress_row = 1;
        }

        // Save cursor
        std::cout << "\033[s";

        // Move to row 1, correct col
        std::cout << "\033[" << progress_row << ";" << col << "H";

        // Print text
        std::cout << text;

        // Restore cursor
        std::cout << "\033[u" << std::flush;
    }



    void init_progress_row() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            progress_row = csbi.dwCursorPosition.Y + 1; // Windows is 0-based
        }
#else
        struct termios saved, raw;
        tcgetattr(STDIN_FILENO, &saved);
        raw = saved;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);

        std::cout << "\033[6n" << std::flush; // Request cursor position

        int row, col;
        if (scanf("\033[%d;%dR", &row, &col) == 2) {
            progress_row = row;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &saved);
#endif
    }
}


