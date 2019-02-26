/** Copyright (C) 2018 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "action.h"
#include "Util.h"

std::string Argument2Code(const buffers::resources::Argument& arg) {
  using buffers::resources::ArgumentKind;
  std::string val = arg.string();

  switch (arg.kind()) {
    case ArgumentKind::ARG_BOTH:
      // treat as literal if starts with quote (")
      if (val[0] == '"' || val[0] == '\'') return val;
      // else fall through
    case ArgumentKind::ARG_STRING:
      return '\"' + string_replace_all(string_replace_all(val, "\\", "\\\\"), "\"", "\"+'\"'+\"") + '\"';
    case ArgumentKind::ARG_BOOLEAN:
      return std::to_string(val != "0");
    case ArgumentKind::ARG_SPRITE:
      return arg.has_sprite() ? arg.sprite() : "-1";
    case ArgumentKind::ARG_SOUND:
      return arg.has_sound() ? arg.sound() : "-1";
    case ArgumentKind::ARG_BACKGROUND:
      return arg.has_background() ? arg.background() : "-1";
    case ArgumentKind::ARG_PATH:
      return arg.has_path() ? arg.path() : "-1";
    case ArgumentKind::ARG_SCRIPT:
      return arg.has_script() ? arg.script() : "-1";
    case ArgumentKind::ARG_OBJECT:
      return arg.has_object() ? arg.object() : "-1";
    case ArgumentKind::ARG_ROOM:
      return arg.has_room() ? arg.room() : "-1";
    case ArgumentKind::ARG_FONT:
      return arg.has_font() ? arg.font() : "-1";
    case ArgumentKind::ARG_TIMELINE:
      return arg.has_timeline() ? arg.timeline() : "-1";
    case ArgumentKind::ARG_MENU:
    case ArgumentKind::ARG_COLOR:
    default:
      if (val.empty()) {
        if (arg.kind() == ArgumentKind::ARG_STRING)
          return "\"\"";
        else
          return "0";
      }
      // else fall all the way through
  }

  return val;
}

std::string Actions2Code(const std::vector< buffers::resources::Action >& actions) {
  using buffers::resources::ActionKind;
  using buffers::resources::ActionExecution;
  std::string code = "";

  int numberOfBraces = 0; // gm ignores brace actions which are in the wrong place or missing
  int numberOfIfs = 0; // gm allows multipe else actions after 1 if, so its important to track the number

  for (const auto &action : actions) {
    const auto &args = action.arguments();

    bool in_with = action.use_apply_to() && action.who_name() != "self";
    if (in_with)
      code += "with (" + action.who_name() + ")\n";

    switch (action.kind()) {
      case ActionKind::ACT_BEGIN:
        code += '{';
        numberOfBraces++;
        break;
      case ActionKind::ACT_END:
        if (numberOfBraces > 0) {
          code += '}';
          numberOfBraces--;
        }
        break;
      case ActionKind::ACT_ELSE:
        if (numberOfIfs > 0) {
          code += "else ";
          numberOfIfs--;
        }
        break;
      case ActionKind::ACT_EXIT:
        code += "exit;";
        break;
      case ActionKind::ACT_REPEAT:
        code += "repeat (" + args.Get(0).string() + ")";
        break;
      case ActionKind::ACT_VARIABLE:
        code += args.Get(0).string();
        if (action.relative())
          code += " += ";
        else
          code += " = ";
        code += args.Get(1).string();
        break;
      case ActionKind::ACT_CODE:
        code += "{\n" + args.Get(0).string() + "\n/**/\n}";
        break;
      case ActionKind::ACT_NORMAL:
        if (action.exe_type() == ActionExecution::EXEC_NONE) break;

        if (action.is_question()) {
          code += "__if__ = ";
          numberOfIfs++;
        }

        if (action.is_not())
          code += "!";

        if (action.relative()) {
          if (action.is_question())
            code += "(argument_relative = " + std::to_string(action.relative()) + ", ";
          else
            code += "{\nargument_relative = " + std::to_string(action.relative()) + ";\n";
        }

        if (action.is_question() && action.exe_type() == ActionExecution::EXEC_CODE)
          code += action.code_string();
        else
          code += action.function_name();

        if (action.exe_type() == ActionExecution::EXEC_FUNCTION) {
          code += '(';
          for (int i = 0; i < args.size(); i++) {
            if (i != 0)
              code += ',';
            code += Argument2Code(args.Get(i));
          }
          code += ')';
        }

        if (action.relative())
          code += action.is_question() ? ");" : "\n}";
        if (action.is_question()) {
          code += "\nif (__if__)";
        }
        break;
      default:
        break;
    }
    code += '\n';
  }

  // someone forgot the closing block action
  if (numberOfBraces > 0)
    for (int i = 0; i < numberOfBraces; i++)
      code += "\n}";

  if (numberOfIfs > 0)
    code = "var __if__ = false;\n" + code;

  return code;
}
