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
#include "strings_util.h"
#include <stack>
#include <fstream>
#include <sstream>
#include <chrono>

std::string Argument2Code(const buffers::resources::Argument& arg) {
  using buffers::resources::ArgumentKind;
  std::string val = arg.string();
  
  // #region agent log
  {
    std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
    if (log.is_open()) {
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::stringstream ss;
      ss << "{\"id\":\"log_" << now << "_arg\",\"timestamp\":" << now << ",\"location\":\"action.cpp:22\",\"message\":\"Argument2Code entry\",\"data\":{\"kind\":" << static_cast<int>(arg.kind()) << ",\"string\":\"" << val << "\",\"has_sprite\":" << (arg.has_sprite() ? "true" : "false") << ",\"has_object\":" << (arg.has_object() ? "true" : "false") << "},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n";
      log << ss.str();
    }
  }
  // #endregion

  switch (arg.kind()) {
    case ArgumentKind::ARG_BOTH:
      // treat as literal if starts with quote (")
      if (!val.empty() && (val[0] == '"' || val[0] == '\'')) return val;
      // else fall through
    case ArgumentKind::ARG_STRING:
      return '\"' + string_replace_all(string_replace_all(val, "\\", "\\\\"), "\"", "\"+'\"'+\"") + '\"';
    case ArgumentKind::ARG_BOOLEAN:
      return std::to_string(val != "0");
    case ArgumentKind::ARG_SPRITE:
      return (arg.has_sprite() && !arg.sprite().empty()) ? arg.sprite() : "-1";
    case ArgumentKind::ARG_SOUND:
      return (arg.has_sound() && !arg.sound().empty()) ? arg.sound() : "-1";
    case ArgumentKind::ARG_BACKGROUND:
      return (arg.has_background() && !arg.background().empty()) ? arg.background() : "-1";
    case ArgumentKind::ARG_PATH:
      return (arg.has_path() && !arg.path().empty()) ? arg.path() : "-1";
    case ArgumentKind::ARG_SCRIPT:
      return (arg.has_script() && !arg.script().empty()) ? arg.script() : "-1";
    case ArgumentKind::ARG_OBJECT:
      return (arg.has_object() && !arg.object().empty()) ? arg.object() : "-1";
    case ArgumentKind::ARG_ROOM:
      return (arg.has_room() && !arg.room().empty()) ? arg.room() : "-1";
    case ArgumentKind::ARG_FONT:
      return (arg.has_font() && !arg.font().empty()) ? arg.font() : "-1";
    case ArgumentKind::ARG_TIMELINE:
      return (arg.has_timeline() && !arg.timeline().empty()) ? arg.timeline() : "-1";
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

  // #region agent log
  {
    std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
    if (log.is_open()) {
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::stringstream ss;
      ss << "{\"id\":\"log_" << now << "_argret\",\"timestamp\":" << now << ",\"location\":\"action.cpp:66\",\"message\":\"Argument2Code exit\",\"data\":{\"result\":\"" << val << "\"},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n";
      log << ss.str();
    }
  }
  // #endregion
  
  return val;
}

/// A poor man's <tt> std::span </tt> over <tt> const std::vector< buffers::resources::Action >& actions </tt>
///
/// The second pointer being null implies the end of the actions buffer was reached.
using ActionVectorSpan = std::pair<const buffers::resources::Action*, const buffers::resources::Action*>;

/// Return a span over the vector of actions which consist of the next action (or block of actions) in the sequence.
///
/// The pointers are inclusive at both ends, so the first pointer points to the starting action and the second pointer
/// points to the ending action. It also considers the action (or block of actions) that come after an @c if
/// (conditional/question) action to be included with the @c if.
///
/// A return value of <tt> {nullptr, nullptr} </tt> indicates the end of the sequence.
ActionVectorSpan GetNextAction(const std::vector<buffers::resources::Action> &actions, std::size_t &index) {
  using buffers::resources::ActionKind;

  if (index >= actions.size()) {
    return {nullptr, nullptr};
  }

  const buffers::resources::Action *begin = &actions[index++];

  // Conditional/Question/If actions and ACT_REPEAT actions have a body
  if (begin->is_question()) {
    ActionVectorSpan body = GetNextAction(actions, index);

    // Consider the @c else of an if as part of its body
    if (index < actions.size() && actions[index].kind() == ActionKind::ACT_ELSE) {
      // Skip over the ACT_ELSE and set the index pointer to the first action of the else
      index += 1;
      body = GetNextAction(actions, index);
    }
    // Return the start of the if action and the end of its body as the sequence of actions.
    return {begin, body.second};
  } else if (begin->kind() == ActionKind::ACT_REPEAT) {
    ActionVectorSpan body = GetNextAction(actions, index);
    // Return the start of the repeat action and the end of its body as the sequence of actions.
    return {begin, body.second};
  } else if (begin->kind() == ActionKind::ACT_BEGIN) {
    while (index < actions.size() && actions[index].kind() != ActionKind::ACT_END) {
      index++;
    }

    // If the block was not delineated properly, this ends up consuming all the actions in the sequence.
    if (index >= actions.size()) {
      return {begin, &actions.back()};
    } else {
      const buffers::resources::Action *end_ptr = &actions[index];
      index++;  // consume ACT_END so caller (e.g. outer if) sees next action (ACT_ELSE)
      return {begin, end_ptr};
    }
  }

  return {begin, begin};
}

/// Push the given action's target to the @c who_name_stack if required
///
/// \return true If a new target was pushed onto the stack
/// \return false If a new target was not pushed onto the stack
bool PushIfRequired(std::string &code, std::stack<std::string> &who_name_stack, const buffers::resources::Action &action) {
  // If we have a relative action nested inside another action, we need to check if the action we are inside doesn't
  // already apply relative to the object we are applying to
  //
  // For applying relative to self, we only need to check if there exists a target before us in the stack which is
  // already being applied to, which is handled in the else-if branch
  if (action.use_apply_to() && action.who_name() != "self") {
    if (who_name_stack.empty() || who_name_stack.top() != action.who_name()) {
      code += "with (" + action.who_name() + ")\n";
      who_name_stack.push(action.who_name());
      return true;
    }
  } else if (!who_name_stack.empty() && who_name_stack.top() != "self") {
    // Otherwise, if we are applying to "self", we need to check if the action we are within doesn't apply to some
    // object other than "self"
    code += "with (self)\n";
    who_name_stack.push("self");
    return true;
  }
  // Otherwise, the stack is empty and we are applying to self, in which case we need to do nothing at all
  return false;
}

std::string Action2Code(ActionVectorSpan span, int &numberOfBraces, int &numberOfIfs, std::stack<std::string> &who_name_stack);

/// A question action is one which is used for conditional flow of control, i.e. an "if"-action.
///
/// The problem with these is that they require special scoping of the @c __if__ variable which actually stores the
/// results of the evaluation of the condition.
std::pair<std::string, std::size_t> Question2Code(ActionVectorSpan span, int &numberOfBraces, int &numberOfIfs, std::stack<std::string> &who_name_stack) {
  using buffers::resources::ActionExecution;
  using buffers::resources::ActionKind;

  std::string code;
  bool pushed_to_stack = PushIfRequired(code, who_name_stack, *span.first);
  code += "{\n";

  const auto &action = *span.first;
  const auto &args = action.arguments();

  code += "__if__ = ";
  numberOfIfs++;

  if (action.is_not())
    code += "!";

  if (action.use_relative()) {
      code += std::string{"(argument_relative = "} + (action.relative() ? "true" : "false") + ", ";
  }

  if (action.exe_type() == ActionExecution::EXEC_CODE)
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

  if (action.use_relative())
    code += ");";

  code += "\nif (__if__)\n";

  // Find the ACT_ELSE action if it exists
  const buffers::resources::Action *else_action = nullptr;
  const buffers::resources::Action *if_body_end = span.second;
  
  for (const buffers::resources::Action *iter = span.first + 1; iter <= span.second; iter++) {
    if (iter->kind() == ActionKind::ACT_ELSE) {
      else_action = iter;
      if_body_end = iter - 1;
      break;
    }
  }

  // Emit then and else each inside explicit braces so "else" always pairs with "if"
  code += "{\n";
  if (if_body_end >= span.first + 1) {
    code += Action2Code({span.first + 1, if_body_end}, numberOfBraces, numberOfIfs, who_name_stack);
  }
  code += "}\n";

  if (else_action != nullptr) {
    code += "else {\n";
    numberOfIfs--; // The else consumes one if
    const buffers::resources::Action *else_body_start = else_action + 1;
    if (else_body_start <= span.second) {
      code += Action2Code({else_body_start, span.second}, numberOfBraces, numberOfIfs, who_name_stack);
    }
    code += "}\n";
  }

  code += "}\n";

  // We have already created the code for the body of the @c if, thus we need to tell the @c Action2Code which called us
  // to skip over the actions we have already emitted code for.
  std::size_t displacement = std::distance(span.first, span.second);

  if (pushed_to_stack) {
    who_name_stack.pop();
  }

  return {code, displacement};
}

/// Convert a sequence of actions to a code string.
std::string Action2Code(ActionVectorSpan span, int &numberOfBraces, int &numberOfIfs, std::stack<std::string> &who_name_stack) {
  using buffers::resources::ActionKind;
  using buffers::resources::ActionExecution;

  std::string code;

  // #region agent log
  {
    std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
    if (log.is_open()) {
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::stringstream ss;
      if (span.first != nullptr) {
        ss << "{\"id\":\"log_" << now << "_action2code\",\"timestamp\":" << now << ",\"location\":\"action.cpp:238\",\"message\":\"Action2Code entry\",\"data\":{\"kind\":" << static_cast<int>(span.first->kind()) << ",\"is_question\":" << (span.first->is_question() ? "true" : "false") << ",\"who_name\":\"" << span.first->who_name() << "\",\"function_name\":\"" << span.first->function_name() << "\"},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"C\"}\n";
      }
      log << ss.str();
    }
  }
  // #endregion

  bool pushed_to_stack = PushIfRequired(code, who_name_stack, *span.first);

  const buffers::resources::Action *iter = span.first;
  while (iter <= span.second) {
    const auto &action = *iter;
    const auto &args = action.arguments();

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
        code += "exit ";
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
        code += args.Get(1).string() + "\n";
        break;
      case ActionKind::ACT_CODE:
        code += "{\n" + args.Get(0).string() + "\n/**/\n}";
        break;
      case ActionKind::ACT_NORMAL:
        if (action.exe_type() == ActionExecution::EXEC_NONE) break;

        // #region agent log
        {
          std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
          if (log.is_open()) {
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            std::stringstream ss;
            ss << "{\"id\":\"log_" << now << "_actnormal\",\"timestamp\":" << now << ",\"location\":\"action.cpp:285\",\"message\":\"ACT_NORMAL processing\",\"data\":{\"function_name\":\"" << action.function_name() << "\",\"args_size\":" << args.size() << ",\"exe_type\":" << static_cast<int>(action.exe_type()) << "},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"C\"}\n";
            log << ss.str();
          }
        }
        // #endregion

        if (action.is_question()) {
          auto question = Question2Code({iter, span.second}, numberOfBraces, numberOfIfs, who_name_stack);
          code += question.first; // The code generated for the if
          iter += question.second; // The displacement required as a result of processing the if's body
          break;
        }

        if (action.use_relative()) {
          code += std::string{"{argument_relative = "} + (action.relative() ? "true" : "false") + "; ";
        }

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

        if (action.use_relative()) {
          code += "\n}";
        }
        code += '\n';  /* newline between actions to avoid "action1(...){rel action2(...)}" */
        break;
      default:
        break;
    }

    iter++;
  }

  if (pushed_to_stack) {
    who_name_stack.pop();
  }

  return code;
}

std::string Actions2Code(const std::vector< buffers::resources::Action >& actions) {
  using buffers::resources::ActionKind;
  using buffers::resources::ActionExecution;
  std::string code = "";

  // #region agent log
  {
    std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
    if (log.is_open()) {
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::stringstream ss;
      ss << "{\"id\":\"log_" << now << "_actions2code\",\"timestamp\":" << now << ",\"location\":\"action.cpp:329\",\"message\":\"Actions2Code entry\",\"data\":{\"action_count\":" << actions.size() << "},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"C\"}\n";
      log << ss.str();
    }
  }
  // #endregion

  int numberOfBraces = 0; // gm ignores brace actions which are in the wrong place or missing
  int numberOfIfs = 0; // gm allows multipe else actions after 1 if, so its important to track the number

  std::size_t i = 0;
  // This is used to track which object's scope we are currently in, so that we know when its required to emit "with()".
  std::stack<std::string> who_stack{};

  while (i < actions.size()) {
    ActionVectorSpan action = GetNextAction(actions, i);
    // #region agent log
    {
      std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
      if (log.is_open()) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::stringstream ss;
        ss << "{\"id\":\"log_" << now << "_getnext\",\"timestamp\":" << now << ",\"location\":\"action.cpp:342\",\"message\":\"GetNextAction result\",\"data\":{\"i\":" << i << ",\"size\":" << actions.size() << ",\"action_ptr\":" << (action.first != nullptr ? "valid" : "null") << "},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"C\"}\n";
        log << ss.str();
      }
    }
    // #endregion
    if (action.first != nullptr) {
      code += Action2Code(action, numberOfBraces, numberOfIfs, who_stack);
    }
  }

  // someone forgot the closing block action
  if (numberOfBraces > 0)
    for (int i = 0; i < numberOfBraces; i++)
      code += "\n}";

  if (numberOfIfs > 0)
    code = "var __if__ = false;\n" + code;

  // #region agent log
  {
    std::ofstream log("/Users/gregwilliamson/RadialGM/Submodules/enigma-dev/.cursor/debug.log", std::ios::app);
    if (log.is_open()) {
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::stringstream ss;
      ss << "{\"id\":\"log_" << now << "_actions2coderet\",\"timestamp\":" << now << ",\"location\":\"action.cpp:356\",\"message\":\"Actions2Code exit\",\"data\":{\"code_length\":" << code.length() << ",\"numberOfBraces\":" << numberOfBraces << ",\"numberOfIfs\":" << numberOfIfs << ",\"code_preview\":\"" << (code.length() > 200 ? code.substr(0, 200) : code) << "\"},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"C\"}\n";
      log << ss.str();
    }
  }
  // #endregion

  return code;
}
