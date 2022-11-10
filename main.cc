#include <string>

#include "nbi.h"

int main(int argc, char** argv)
{
    std::string nbi_path = "NOPATH";
    if (argc>1)
    {
        nbi_path = std::string(argv[1]);
        print("Root path:", nbi_path);
    }
    else
    {
        print("ERROR: did not specify root path.");
    }
    nbi::root_window_t app;
    return 0;
}