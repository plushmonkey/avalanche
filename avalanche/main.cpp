#include "getopt.h"
#include "Avalanche.h"

#include <mclib/core/Client.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <cstdio>
#include <cstdlib>

avalanche::OptionMap ReadOptions(int argc, char* argv[]);
void DisplayUsage();

int main(int argc, char* argv[]) {
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks();
    avalanche::OptionMap options = ReadOptions(argc, argv);
    avalanche::Avalanche avalanche;

    try {
        if (!avalanche.Initialize(options)) {
            DisplayUsage();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    avalanche.Run();

    return 0;
}

void DisplayUsage() {
    std::cout << "./avalanche --username [username] --password [password] --server [server:port] --count [count] --json [filename]" << std::endl;
}

avalanche::OptionMap ReadOptions(int argc, char* argv[]) {
    avalanche::OptionMap options;

    while (1) {
        static struct option long_options[] = {
            { "username", required_argument, 0, 0 },
            { "password", required_argument, 0, 0 },
            { "server", required_argument, 0, 0 },
            { "count", required_argument, 0, 0 },
            { "behavior", required_argument, 0, 0 },
            { "json", required_argument, 0, 0 },
            { 0, 0, 0, 0 }
        };

        int option_index = 0;
        int c;

        c = getopt_long(argc, argv, ":", long_options, &option_index);

        if (c == -1)
            break;

        if (c == 0) {
            std::string option_name = long_options[option_index].name;

            if (!optarg)
                continue;

            options[option_name] = optarg;
        }
    }

    return options;
}
