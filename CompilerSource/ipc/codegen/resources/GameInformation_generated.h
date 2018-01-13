// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GAMEINFORMATION_H_
#define FLATBUFFERS_GENERATED_GAMEINFORMATION_H_

#include "flatbuffers/flatbuffers.h"

struct GameInformation;

struct GameInformation FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_BACKGROUND_COLOR = 4,
    VT_EMBED_GAME_WINDOW = 6,
    VT_FORM_CAPTION = 8,
    VT_LEFT = 10,
    VT_TOP = 12,
    VT_WIDTH = 14,
    VT_HEIGHT = 16,
    VT_SHOW_BORDER = 18,
    VT_ALLOW_RESIZE = 20,
    VT_STAY_ON_TOP = 22,
    VT_PAUSE_GAME = 24,
    VT_TEXT = 26
  };
  int32_t background_color() const {
    return GetField<int32_t>(VT_BACKGROUND_COLOR, 0);
  }
  bool embed_game_window() const {
    return GetField<uint8_t>(VT_EMBED_GAME_WINDOW, 0) != 0;
  }
  const flatbuffers::String *form_caption() const {
    return GetPointer<const flatbuffers::String *>(VT_FORM_CAPTION);
  }
  int32_t left() const {
    return GetField<int32_t>(VT_LEFT, 0);
  }
  int32_t top() const {
    return GetField<int32_t>(VT_TOP, 0);
  }
  int32_t width() const {
    return GetField<int32_t>(VT_WIDTH, 0);
  }
  int32_t height() const {
    return GetField<int32_t>(VT_HEIGHT, 0);
  }
  bool show_border() const {
    return GetField<uint8_t>(VT_SHOW_BORDER, 0) != 0;
  }
  bool allow_resize() const {
    return GetField<uint8_t>(VT_ALLOW_RESIZE, 0) != 0;
  }
  bool stay_on_top() const {
    return GetField<uint8_t>(VT_STAY_ON_TOP, 0) != 0;
  }
  bool pause_game() const {
    return GetField<uint8_t>(VT_PAUSE_GAME, 0) != 0;
  }
  const flatbuffers::String *text() const {
    return GetPointer<const flatbuffers::String *>(VT_TEXT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_BACKGROUND_COLOR) &&
           VerifyField<uint8_t>(verifier, VT_EMBED_GAME_WINDOW) &&
           VerifyOffset(verifier, VT_FORM_CAPTION) &&
           verifier.Verify(form_caption()) &&
           VerifyField<int32_t>(verifier, VT_LEFT) &&
           VerifyField<int32_t>(verifier, VT_TOP) &&
           VerifyField<int32_t>(verifier, VT_WIDTH) &&
           VerifyField<int32_t>(verifier, VT_HEIGHT) &&
           VerifyField<uint8_t>(verifier, VT_SHOW_BORDER) &&
           VerifyField<uint8_t>(verifier, VT_ALLOW_RESIZE) &&
           VerifyField<uint8_t>(verifier, VT_STAY_ON_TOP) &&
           VerifyField<uint8_t>(verifier, VT_PAUSE_GAME) &&
           VerifyOffset(verifier, VT_TEXT) &&
           verifier.Verify(text()) &&
           verifier.EndTable();
  }
};

struct GameInformationBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_background_color(int32_t background_color) {
    fbb_.AddElement<int32_t>(GameInformation::VT_BACKGROUND_COLOR, background_color, 0);
  }
  void add_embed_game_window(bool embed_game_window) {
    fbb_.AddElement<uint8_t>(GameInformation::VT_EMBED_GAME_WINDOW, static_cast<uint8_t>(embed_game_window), 0);
  }
  void add_form_caption(flatbuffers::Offset<flatbuffers::String> form_caption) {
    fbb_.AddOffset(GameInformation::VT_FORM_CAPTION, form_caption);
  }
  void add_left(int32_t left) {
    fbb_.AddElement<int32_t>(GameInformation::VT_LEFT, left, 0);
  }
  void add_top(int32_t top) {
    fbb_.AddElement<int32_t>(GameInformation::VT_TOP, top, 0);
  }
  void add_width(int32_t width) {
    fbb_.AddElement<int32_t>(GameInformation::VT_WIDTH, width, 0);
  }
  void add_height(int32_t height) {
    fbb_.AddElement<int32_t>(GameInformation::VT_HEIGHT, height, 0);
  }
  void add_show_border(bool show_border) {
    fbb_.AddElement<uint8_t>(GameInformation::VT_SHOW_BORDER, static_cast<uint8_t>(show_border), 0);
  }
  void add_allow_resize(bool allow_resize) {
    fbb_.AddElement<uint8_t>(GameInformation::VT_ALLOW_RESIZE, static_cast<uint8_t>(allow_resize), 0);
  }
  void add_stay_on_top(bool stay_on_top) {
    fbb_.AddElement<uint8_t>(GameInformation::VT_STAY_ON_TOP, static_cast<uint8_t>(stay_on_top), 0);
  }
  void add_pause_game(bool pause_game) {
    fbb_.AddElement<uint8_t>(GameInformation::VT_PAUSE_GAME, static_cast<uint8_t>(pause_game), 0);
  }
  void add_text(flatbuffers::Offset<flatbuffers::String> text) {
    fbb_.AddOffset(GameInformation::VT_TEXT, text);
  }
  explicit GameInformationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GameInformationBuilder &operator=(const GameInformationBuilder &);
  flatbuffers::Offset<GameInformation> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GameInformation>(end);
    return o;
  }
};

inline flatbuffers::Offset<GameInformation> CreateGameInformation(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t background_color = 0,
    bool embed_game_window = false,
    flatbuffers::Offset<flatbuffers::String> form_caption = 0,
    int32_t left = 0,
    int32_t top = 0,
    int32_t width = 0,
    int32_t height = 0,
    bool show_border = false,
    bool allow_resize = false,
    bool stay_on_top = false,
    bool pause_game = false,
    flatbuffers::Offset<flatbuffers::String> text = 0) {
  GameInformationBuilder builder_(_fbb);
  builder_.add_text(text);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_top(top);
  builder_.add_left(left);
  builder_.add_form_caption(form_caption);
  builder_.add_background_color(background_color);
  builder_.add_pause_game(pause_game);
  builder_.add_stay_on_top(stay_on_top);
  builder_.add_allow_resize(allow_resize);
  builder_.add_show_border(show_border);
  builder_.add_embed_game_window(embed_game_window);
  return builder_.Finish();
}

inline flatbuffers::Offset<GameInformation> CreateGameInformationDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t background_color = 0,
    bool embed_game_window = false,
    const char *form_caption = nullptr,
    int32_t left = 0,
    int32_t top = 0,
    int32_t width = 0,
    int32_t height = 0,
    bool show_border = false,
    bool allow_resize = false,
    bool stay_on_top = false,
    bool pause_game = false,
    const char *text = nullptr) {
  return CreateGameInformation(
      _fbb,
      background_color,
      embed_game_window,
      form_caption ? _fbb.CreateString(form_caption) : 0,
      left,
      top,
      width,
      height,
      show_border,
      allow_resize,
      stay_on_top,
      pause_game,
      text ? _fbb.CreateString(text) : 0);
}

inline flatbuffers::TypeTable *GameInformationTypeTable();

inline flatbuffers::TypeTable *GameInformationTypeTable() {
  static flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 }
  };
  static const char *names[] = {
    "background_color",
    "embed_game_window",
    "form_caption",
    "left",
    "top",
    "width",
    "height",
    "show_border",
    "allow_resize",
    "stay_on_top",
    "pause_game",
    "text"
  };
  static flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 12, type_codes, nullptr, nullptr, names, nullptr
  };
  return &tt;
}

inline const GameInformation *GetGameInformation(const void *buf) {
  return flatbuffers::GetRoot<GameInformation>(buf);
}

inline bool VerifyGameInformationBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<GameInformation>(nullptr);
}

inline void FinishGameInformationBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<GameInformation> root) {
  fbb.Finish(root);
}

#endif  // FLATBUFFERS_GENERATED_GAMEINFORMATION_H_
