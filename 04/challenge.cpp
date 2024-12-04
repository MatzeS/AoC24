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


    uint32_t sum = 0;
    for(uint32_t y = 0; y < data.size(); y++){
        auto const length = data[y].size();
        for(uint32_t x = 0; x < length; x++){

            std::array<std::array<int, 2>, 8> directions{
                std::array<int, 2>{ 0,  1},
                std::array<int, 2>{ 0, -1},
                std::array<int, 2>{ 1,  0},
                std::array<int, 2>{-1,  0},
                std::array<int, 2>{ 1,  1},
                std::array<int, 2>{-1,  1},
                std::array<int, 2>{ 1, -1},
                std::array<int, 2>{-1, -1}
            };

            auto const word = std::string_view{"XMAS"};
            assert(word.size() == 4);

            for(auto const direction : directions){
                int32_t px = x;
                int32_t py = y;

                // std::cout << "new" << y << "/" << x << std::endl;
                for(uint8_t i = 0; i < word.size(); i++){
                    if(word[i] != data.at(py).at(px)){ 
                        // std::cout << word[i] << "/" << data[y][x] << "  " << py << "/" << px << std::endl;
                        break;
                    }
                    
                    if(i == word.size() - 1){
                        sum++;
                        break;
                    }

                    py += direction[0];
                    px += direction[1];


                    if(py < 0) break;
                    if(px < 0) break;
                    if(py == data.size()) break;
                    if(px == data[y].size()) break;

                }
            }

        }
    }


    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}