#include "3FG.hpp"

#include <fstream>
#include <iostream>

bool read_3fg(const std::string &input_file, Game *game) {
  std::string create, step, draw;
  if (std::ifstream f{input_file + "/create.edl"}) {
    create = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (std::ifstream f{input_file + "/step.edl"}) {
    step = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (std::ifstream f{input_file + "/draw.edl"}) {
    draw = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (create.empty() && step.empty() && draw.empty()) {
    std::cerr << "Error: Failed to read input \"" << input_file << "\". "
                 "Is the game empty?" << std::endl;
    return false;
  }
  game->AddSimpleObject(create, step, draw);
  game->AddDefaultRoom();
  return true;
}
