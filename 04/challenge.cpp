#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <regex>
#include <iterator>

int main(int argc, char* argv[]){
    if (argc != 2){
        return EXIT_FAILURE;
    }

    auto const inputPath = argv[1];
    auto inputFilestream = std::ifstream{inputPath};

    std::stringstream input;
    input << inputFilestream.rdbuf();

    std::vector<std::vector<char>> data;

    auto lines = input.view() 
        | std::ranges::views::split('\n');
    uint32_t lineIndex = 0;
    for(auto line : lines) {

        data.push_back({});
        
        uint32_t characterIndex = 0;
        for(auto character : line){
            
            data.back().push_back(character);
            // std::cout << character << std::endl;

            characterIndex++;
        }

        lineIndex++;
    }


    uint32_t sum = 0;
    for(uint32_t y = 0; y < data.size(); y++){
        auto const length = data[y].size();
        for(uint32_t x = 0; x < length; x++){

            // read forward
            if(x < length - 4){
                if(
                    data[y][x] == 'X' &&
                    data[y][x] == 'M' &&
                    data[y][x] == 'A' &&
                    data[y][x] == 'X'
                ) {
                    sum++;
                }
            }

        }
    }


    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}