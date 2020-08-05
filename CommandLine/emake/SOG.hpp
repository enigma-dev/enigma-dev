#ifndef EMAKE_3FG_HPP
#define EMAKE_3FG_HPP
#include "Game.hpp"
#include "event_reader/event_parser.h"

#include <string>

bool ReadSOG(const std::string &input_file, Game *game, const EventData* event_data);

#endif
