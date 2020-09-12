#define show_debug_message(msg, severity) show_debug_message_hidden(msg, severity)

namespace enigma_user {

inline void show_debug_message_hidden(std::string msg, MESSAGE_TYPE type = M_INFO) {
  #ifndef DO_NOT_ABORT_ERRORS
  if (severity == M_ERROR || severity == M_USER_ERROR ||
    severity == M_FATAL_ERROR || severity == M_FATAL_USER_ERROR) {
    exit(1);
  }
  #endif
  if (type == M_FATAL_USER_ERROR || type == M_USER_ERROR) {
    abort();
  }
}

} // namespace enigma_user
