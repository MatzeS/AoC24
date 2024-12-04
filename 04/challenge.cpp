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
#include <array>

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


    std::array stencils{
        std::array{
            std::array{'M', '.', 'M'},
            std::array{'.', 'A', '.'},
            std::array{'S', '.', 'S'}
        },
        std::array{
            std::array{'S', '.', 'M'},
            std::array{'.', 'A', '.'},
            std::array{'S', '.', 'M'}
        },
        std::array{
            std::array{'M', '.', 'S'},
            std::array{'.', 'A', '.'},
            std::array{'M', '.', 'S'}
        },
        std::array{
            std::array{'S', '.', 'S'},
            std::array{'.', 'A', '.'},
            std::array{'M', '.', 'M'}
        }
    };



    uint32_t sum = 0;
    for(uint32_t y = 0; y < data.size() - 2; y++){
        auto const length = data[y].size();
        for(uint32_t x = 0; x < length - 2; x++){

            for(auto stencil : stencils){

                for(uint32_t sy = 0; sy < 3; sy++){
                    for(uint32_t sx = 0; sx < 3; sx++){
                        if(stencil[sy][sx] == '.') continue;
                        if(stencil[sy][sx] != data[y+sy][x+sx]){
                            sx = 4;
                            sy = 4;
                            break;
                        }
                    
                        if(sy == 2 && sx == 2){
                            sum++;
                        }
                    }
                }

            }

        }
    }


    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}