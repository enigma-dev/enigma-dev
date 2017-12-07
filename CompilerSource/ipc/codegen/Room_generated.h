// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ROOM_H_
#define FLATBUFFERS_GENERATED_ROOM_H_

#include "flatbuffers/flatbuffers.h"

struct RoomInstance;

struct RoomTile;

struct RoomBackground;

struct RoomView;

struct RoomEditorMetadata;

struct Room;

MANUALLY_ALIGNED_STRUCT(8) RoomTile FLATBUFFERS_FINAL_CLASS {
 private:
  int32_t id_;
  int32_t background_id_;
  int32_t bg_x_;
  int32_t bg_y_;
  int32_t room_x_;
  int32_t room_y_;
  int32_t width_;
  int32_t height_;
  int32_t depth_;
  int32_t padding0__;
  double scale_x_;
  double scale_y_;
  int32_t color_;
  uint8_t locked_;
  int8_t padding1__;  int16_t padding2__;

 public:
  RoomTile() {
    memset(this, 0, sizeof(RoomTile));
  }
  RoomTile(int32_t _id, int32_t _background_id, int32_t _bg_x, int32_t _bg_y, int32_t _room_x, int32_t _room_y, int32_t _width, int32_t _height, int32_t _depth, double _scale_x, double _scale_y, int32_t _color, bool _locked)
      : id_(flatbuffers::EndianScalar(_id)),
        background_id_(flatbuffers::EndianScalar(_background_id)),
        bg_x_(flatbuffers::EndianScalar(_bg_x)),
        bg_y_(flatbuffers::EndianScalar(_bg_y)),
        room_x_(flatbuffers::EndianScalar(_room_x)),
        room_y_(flatbuffers::EndianScalar(_room_y)),
        width_(flatbuffers::EndianScalar(_width)),
        height_(flatbuffers::EndianScalar(_height)),
        depth_(flatbuffers::EndianScalar(_depth)),
        padding0__(0),
        scale_x_(flatbuffers::EndianScalar(_scale_x)),
        scale_y_(flatbuffers::EndianScalar(_scale_y)),
        color_(flatbuffers::EndianScalar(_color)),
        locked_(flatbuffers::EndianScalar(static_cast<uint8_t>(_locked))),
        padding1__(0),
        padding2__(0) {
    (void)padding0__;
    (void)padding1__;    (void)padding2__;
  }
  int32_t id() const {
    return flatbuffers::EndianScalar(id_);
  }
  int32_t background_id() const {
    return flatbuffers::EndianScalar(background_id_);
  }
  int32_t bg_x() const {
    return flatbuffers::EndianScalar(bg_x_);
  }
  int32_t bg_y() const {
    return flatbuffers::EndianScalar(bg_y_);
  }
  int32_t room_x() const {
    return flatbuffers::EndianScalar(room_x_);
  }
  int32_t room_y() const {
    return flatbuffers::EndianScalar(room_y_);
  }
  int32_t width() const {
    return flatbuffers::EndianScalar(width_);
  }
  int32_t height() const {
    return flatbuffers::EndianScalar(height_);
  }
  int32_t depth() const {
    return flatbuffers::EndianScalar(depth_);
  }
  double scale_x() const {
    return flatbuffers::EndianScalar(scale_x_);
  }
  double scale_y() const {
    return flatbuffers::EndianScalar(scale_y_);
  }
  int32_t color() const {
    return flatbuffers::EndianScalar(color_);
  }
  bool locked() const {
    return flatbuffers::EndianScalar(locked_) != 0;
  }
};
STRUCT_END(RoomTile, 64);

MANUALLY_ALIGNED_STRUCT(4) RoomBackground FLATBUFFERS_FINAL_CLASS {
 private:
  uint8_t visible_;
  uint8_t foreground_;
  int16_t padding0__;
  int32_t background_id_;
  int32_t x_;
  int32_t y_;
  uint8_t tile_horizontal_;
  uint8_t tile_vertical_;
  int16_t padding1__;
  int32_t speed_h_;
  int32_t speed_v_;
  uint8_t stretch_;
  int8_t padding2__;  int16_t padding3__;

 public:
  RoomBackground() {
    memset(this, 0, sizeof(RoomBackground));
  }
  RoomBackground(bool _visible, bool _foreground, int32_t _background_id, int32_t _x, int32_t _y, bool _tile_horizontal, bool _tile_vertical, int32_t _speed_h, int32_t _speed_v, bool _stretch)
      : visible_(flatbuffers::EndianScalar(static_cast<uint8_t>(_visible))),
        foreground_(flatbuffers::EndianScalar(static_cast<uint8_t>(_foreground))),
        padding0__(0),
        background_id_(flatbuffers::EndianScalar(_background_id)),
        x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        tile_horizontal_(flatbuffers::EndianScalar(static_cast<uint8_t>(_tile_horizontal))),
        tile_vertical_(flatbuffers::EndianScalar(static_cast<uint8_t>(_tile_vertical))),
        padding1__(0),
        speed_h_(flatbuffers::EndianScalar(_speed_h)),
        speed_v_(flatbuffers::EndianScalar(_speed_v)),
        stretch_(flatbuffers::EndianScalar(static_cast<uint8_t>(_stretch))),
        padding2__(0),
        padding3__(0) {
    (void)padding0__;
    (void)padding1__;
    (void)padding2__;    (void)padding3__;
  }
  bool visible() const {
    return flatbuffers::EndianScalar(visible_) != 0;
  }
  bool foreground() const {
    return flatbuffers::EndianScalar(foreground_) != 0;
  }
  int32_t background_id() const {
    return flatbuffers::EndianScalar(background_id_);
  }
  int32_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int32_t y() const {
    return flatbuffers::EndianScalar(y_);
  }
  bool tile_horizontal() const {
    return flatbuffers::EndianScalar(tile_horizontal_) != 0;
  }
  bool tile_vertical() const {
    return flatbuffers::EndianScalar(tile_vertical_) != 0;
  }
  int32_t speed_h() const {
    return flatbuffers::EndianScalar(speed_h_);
  }
  int32_t speed_v() const {
    return flatbuffers::EndianScalar(speed_v_);
  }
  bool stretch() const {
    return flatbuffers::EndianScalar(stretch_) != 0;
  }
};
STRUCT_END(RoomBackground, 32);

MANUALLY_ALIGNED_STRUCT(4) RoomView FLATBUFFERS_FINAL_CLASS {
 private:
  uint8_t visible_;
  int8_t padding0__;  int16_t padding1__;
  int32_t x_;
  int32_t y_;
  int32_t width_;
  int32_t height_;
  int32_t port_x_;
  int32_t port_y_;
  int32_t port_w_;
  int32_t port_h_;
  int32_t border_h_;
  int32_t border_v_;
  int32_t speed_h_;
  int32_t speed_v_;
  int32_t object_id_;

 public:
  RoomView() {
    memset(this, 0, sizeof(RoomView));
  }
  RoomView(bool _visible, int32_t _x, int32_t _y, int32_t _width, int32_t _height, int32_t _port_x, int32_t _port_y, int32_t _port_w, int32_t _port_h, int32_t _border_h, int32_t _border_v, int32_t _speed_h, int32_t _speed_v, int32_t _object_id)
      : visible_(flatbuffers::EndianScalar(static_cast<uint8_t>(_visible))),
        padding0__(0),
        padding1__(0),
        x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        width_(flatbuffers::EndianScalar(_width)),
        height_(flatbuffers::EndianScalar(_height)),
        port_x_(flatbuffers::EndianScalar(_port_x)),
        port_y_(flatbuffers::EndianScalar(_port_y)),
        port_w_(flatbuffers::EndianScalar(_port_w)),
        port_h_(flatbuffers::EndianScalar(_port_h)),
        border_h_(flatbuffers::EndianScalar(_border_h)),
        border_v_(flatbuffers::EndianScalar(_border_v)),
        speed_h_(flatbuffers::EndianScalar(_speed_h)),
        speed_v_(flatbuffers::EndianScalar(_speed_v)),
        object_id_(flatbuffers::EndianScalar(_object_id)) {
    (void)padding0__;    (void)padding1__;
  }
  bool visible() const {
    return flatbuffers::EndianScalar(visible_) != 0;
  }
  int32_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int32_t y() const {
    return flatbuffers::EndianScalar(y_);
  }
  int32_t width() const {
    return flatbuffers::EndianScalar(width_);
  }
  int32_t height() const {
    return flatbuffers::EndianScalar(height_);
  }
  int32_t port_x() const {
    return flatbuffers::EndianScalar(port_x_);
  }
  int32_t port_y() const {
    return flatbuffers::EndianScalar(port_y_);
  }
  int32_t port_w() const {
    return flatbuffers::EndianScalar(port_w_);
  }
  int32_t port_h() const {
    return flatbuffers::EndianScalar(port_h_);
  }
  int32_t border_h() const {
    return flatbuffers::EndianScalar(border_h_);
  }
  int32_t border_v() const {
    return flatbuffers::EndianScalar(border_v_);
  }
  int32_t speed_h() const {
    return flatbuffers::EndianScalar(speed_h_);
  }
  int32_t speed_v() const {
    return flatbuffers::EndianScalar(speed_v_);
  }
  int32_t object_id() const {
    return flatbuffers::EndianScalar(object_id_);
  }
};
STRUCT_END(RoomView, 56);

MANUALLY_ALIGNED_STRUCT(4) RoomEditorMetadata FLATBUFFERS_FINAL_CLASS {
 private:
  uint8_t remember_window_size_;
  int8_t padding0__;  int16_t padding1__;
  int32_t editor_width_;
  int32_t editor_height_;
  int32_t current_tab_;
  int32_t scroll_x_;
  int32_t scroll_y_;
  uint8_t show_grid_;
  uint8_t show_objects_;
  uint8_t show_tiles_;
  uint8_t show_backgrounds_;
  uint8_t show_views_;
  uint8_t delete_underlying_objects_;
  uint8_t delete_underlying_tiles_;
  int8_t padding2__;
  int32_t snap_x_;
  int32_t snap_y_;
  uint8_t isometric_;
  int8_t padding3__;  int16_t padding4__;

 public:
  RoomEditorMetadata() {
    memset(this, 0, sizeof(RoomEditorMetadata));
  }
  RoomEditorMetadata(bool _remember_window_size, int32_t _editor_width, int32_t _editor_height, int32_t _current_tab, int32_t _scroll_x, int32_t _scroll_y, bool _show_grid, bool _show_objects, bool _show_tiles, bool _show_backgrounds, bool _show_views, bool _delete_underlying_objects, bool _delete_underlying_tiles, int32_t _snap_x, int32_t _snap_y, bool _isometric)
      : remember_window_size_(flatbuffers::EndianScalar(static_cast<uint8_t>(_remember_window_size))),
        padding0__(0),
        padding1__(0),
        editor_width_(flatbuffers::EndianScalar(_editor_width)),
        editor_height_(flatbuffers::EndianScalar(_editor_height)),
        current_tab_(flatbuffers::EndianScalar(_current_tab)),
        scroll_x_(flatbuffers::EndianScalar(_scroll_x)),
        scroll_y_(flatbuffers::EndianScalar(_scroll_y)),
        show_grid_(flatbuffers::EndianScalar(static_cast<uint8_t>(_show_grid))),
        show_objects_(flatbuffers::EndianScalar(static_cast<uint8_t>(_show_objects))),
        show_tiles_(flatbuffers::EndianScalar(static_cast<uint8_t>(_show_tiles))),
        show_backgrounds_(flatbuffers::EndianScalar(static_cast<uint8_t>(_show_backgrounds))),
        show_views_(flatbuffers::EndianScalar(static_cast<uint8_t>(_show_views))),
        delete_underlying_objects_(flatbuffers::EndianScalar(static_cast<uint8_t>(_delete_underlying_objects))),
        delete_underlying_tiles_(flatbuffers::EndianScalar(static_cast<uint8_t>(_delete_underlying_tiles))),
        padding2__(0),
        snap_x_(flatbuffers::EndianScalar(_snap_x)),
        snap_y_(flatbuffers::EndianScalar(_snap_y)),
        isometric_(flatbuffers::EndianScalar(static_cast<uint8_t>(_isometric))),
        padding3__(0),
        padding4__(0) {
    (void)padding0__;    (void)padding1__;
    (void)padding2__;
    (void)padding3__;    (void)padding4__;
  }
  bool remember_window_size() const {
    return flatbuffers::EndianScalar(remember_window_size_) != 0;
  }
  int32_t editor_width() const {
    return flatbuffers::EndianScalar(editor_width_);
  }
  int32_t editor_height() const {
    return flatbuffers::EndianScalar(editor_height_);
  }
  int32_t current_tab() const {
    return flatbuffers::EndianScalar(current_tab_);
  }
  int32_t scroll_x() const {
    return flatbuffers::EndianScalar(scroll_x_);
  }
  int32_t scroll_y() const {
    return flatbuffers::EndianScalar(scroll_y_);
  }
  bool show_grid() const {
    return flatbuffers::EndianScalar(show_grid_) != 0;
  }
  bool show_objects() const {
    return flatbuffers::EndianScalar(show_objects_) != 0;
  }
  bool show_tiles() const {
    return flatbuffers::EndianScalar(show_tiles_) != 0;
  }
  bool show_backgrounds() const {
    return flatbuffers::EndianScalar(show_backgrounds_) != 0;
  }
  bool show_views() const {
    return flatbuffers::EndianScalar(show_views_) != 0;
  }
  bool delete_underlying_objects() const {
    return flatbuffers::EndianScalar(delete_underlying_objects_) != 0;
  }
  bool delete_underlying_tiles() const {
    return flatbuffers::EndianScalar(delete_underlying_tiles_) != 0;
  }
  int32_t snap_x() const {
    return flatbuffers::EndianScalar(snap_x_);
  }
  int32_t snap_y() const {
    return flatbuffers::EndianScalar(snap_y_);
  }
  bool isometric() const {
    return flatbuffers::EndianScalar(isometric_) != 0;
  }
};
STRUCT_END(RoomEditorMetadata, 44);

struct RoomInstance FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_X = 6,
    VT_Y = 8,
    VT_OBJECT_ID = 10,
    VT_FLIP_X = 12,
    VT_FLIP_Y = 14,
    VT_SCALE_X = 16,
    VT_SCALE_Y = 18,
    VT_ROTATION = 20,
    VT_COLOR = 22,
    VT_CREATION_CODE = 24,
    VT_LOCKED = 26
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  int32_t x() const {
    return GetField<int32_t>(VT_X, 0);
  }
  int32_t y() const {
    return GetField<int32_t>(VT_Y, 0);
  }
  int32_t object_id() const {
    return GetField<int32_t>(VT_OBJECT_ID, 0);
  }
  bool flip_x() const {
    return GetField<uint8_t>(VT_FLIP_X, 0) != 0;
  }
  bool flip_y() const {
    return GetField<uint8_t>(VT_FLIP_Y, 0) != 0;
  }
  double scale_x() const {
    return GetField<double>(VT_SCALE_X, 0.0);
  }
  double scale_y() const {
    return GetField<double>(VT_SCALE_Y, 0.0);
  }
  double rotation() const {
    return GetField<double>(VT_ROTATION, 0.0);
  }
  int32_t color() const {
    return GetField<int32_t>(VT_COLOR, 0);
  }
  const flatbuffers::String *creation_code() const {
    return GetPointer<const flatbuffers::String *>(VT_CREATION_CODE);
  }
  bool locked() const {
    return GetField<uint8_t>(VT_LOCKED, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int32_t>(verifier, VT_X) &&
           VerifyField<int32_t>(verifier, VT_Y) &&
           VerifyField<int32_t>(verifier, VT_OBJECT_ID) &&
           VerifyField<uint8_t>(verifier, VT_FLIP_X) &&
           VerifyField<uint8_t>(verifier, VT_FLIP_Y) &&
           VerifyField<double>(verifier, VT_SCALE_X) &&
           VerifyField<double>(verifier, VT_SCALE_Y) &&
           VerifyField<double>(verifier, VT_ROTATION) &&
           VerifyField<int32_t>(verifier, VT_COLOR) &&
           VerifyOffset(verifier, VT_CREATION_CODE) &&
           verifier.Verify(creation_code()) &&
           VerifyField<uint8_t>(verifier, VT_LOCKED) &&
           verifier.EndTable();
  }
};

struct RoomInstanceBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(RoomInstance::VT_ID, id, 0);
  }
  void add_x(int32_t x) {
    fbb_.AddElement<int32_t>(RoomInstance::VT_X, x, 0);
  }
  void add_y(int32_t y) {
    fbb_.AddElement<int32_t>(RoomInstance::VT_Y, y, 0);
  }
  void add_object_id(int32_t object_id) {
    fbb_.AddElement<int32_t>(RoomInstance::VT_OBJECT_ID, object_id, 0);
  }
  void add_flip_x(bool flip_x) {
    fbb_.AddElement<uint8_t>(RoomInstance::VT_FLIP_X, static_cast<uint8_t>(flip_x), 0);
  }
  void add_flip_y(bool flip_y) {
    fbb_.AddElement<uint8_t>(RoomInstance::VT_FLIP_Y, static_cast<uint8_t>(flip_y), 0);
  }
  void add_scale_x(double scale_x) {
    fbb_.AddElement<double>(RoomInstance::VT_SCALE_X, scale_x, 0.0);
  }
  void add_scale_y(double scale_y) {
    fbb_.AddElement<double>(RoomInstance::VT_SCALE_Y, scale_y, 0.0);
  }
  void add_rotation(double rotation) {
    fbb_.AddElement<double>(RoomInstance::VT_ROTATION, rotation, 0.0);
  }
  void add_color(int32_t color) {
    fbb_.AddElement<int32_t>(RoomInstance::VT_COLOR, color, 0);
  }
  void add_creation_code(flatbuffers::Offset<flatbuffers::String> creation_code) {
    fbb_.AddOffset(RoomInstance::VT_CREATION_CODE, creation_code);
  }
  void add_locked(bool locked) {
    fbb_.AddElement<uint8_t>(RoomInstance::VT_LOCKED, static_cast<uint8_t>(locked), 0);
  }
  explicit RoomInstanceBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  RoomInstanceBuilder &operator=(const RoomInstanceBuilder &);
  flatbuffers::Offset<RoomInstance> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RoomInstance>(end);
    return o;
  }
};

inline flatbuffers::Offset<RoomInstance> CreateRoomInstance(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t x = 0,
    int32_t y = 0,
    int32_t object_id = 0,
    bool flip_x = false,
    bool flip_y = false,
    double scale_x = 0.0,
    double scale_y = 0.0,
    double rotation = 0.0,
    int32_t color = 0,
    flatbuffers::Offset<flatbuffers::String> creation_code = 0,
    bool locked = false) {
  RoomInstanceBuilder builder_(_fbb);
  builder_.add_rotation(rotation);
  builder_.add_scale_y(scale_y);
  builder_.add_scale_x(scale_x);
  builder_.add_creation_code(creation_code);
  builder_.add_color(color);
  builder_.add_object_id(object_id);
  builder_.add_y(y);
  builder_.add_x(x);
  builder_.add_id(id);
  builder_.add_locked(locked);
  builder_.add_flip_y(flip_y);
  builder_.add_flip_x(flip_x);
  return builder_.Finish();
}

inline flatbuffers::Offset<RoomInstance> CreateRoomInstanceDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t x = 0,
    int32_t y = 0,
    int32_t object_id = 0,
    bool flip_x = false,
    bool flip_y = false,
    double scale_x = 0.0,
    double scale_y = 0.0,
    double rotation = 0.0,
    int32_t color = 0,
    const char *creation_code = nullptr,
    bool locked = false) {
  return CreateRoomInstance(
      _fbb,
      id,
      x,
      y,
      object_id,
      flip_x,
      flip_y,
      scale_x,
      scale_y,
      rotation,
      color,
      creation_code ? _fbb.CreateString(creation_code) : 0,
      locked);
}

struct Room FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_ID = 6,
    VT_EDITOR_METADATA = 8,
    VT_CAPTION = 10,
    VT_WIDTH = 12,
    VT_HEIGHT = 14,
    VT_SPEED = 16,
    VT_PERSISTENT = 18,
    VT_BACKGROUND_COLOR = 20,
    VT_DRAW_BACKGROUND_COLOR = 22,
    VT_CREATION_CODE = 24,
    VT_VIEWS_ENABLED = 26,
    VT_VIEWS = 28,
    VT_BACKGROUNDS = 30,
    VT_INSTANCES = 32,
    VT_TILES = 34
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  const RoomEditorMetadata *editor_metadata() const {
    return GetStruct<const RoomEditorMetadata *>(VT_EDITOR_METADATA);
  }
  const flatbuffers::String *caption() const {
    return GetPointer<const flatbuffers::String *>(VT_CAPTION);
  }
  int32_t width() const {
    return GetField<int32_t>(VT_WIDTH, 0);
  }
  int32_t height() const {
    return GetField<int32_t>(VT_HEIGHT, 0);
  }
  int32_t speed() const {
    return GetField<int32_t>(VT_SPEED, 0);
  }
  bool persistent() const {
    return GetField<uint8_t>(VT_PERSISTENT, 0) != 0;
  }
  int32_t background_color() const {
    return GetField<int32_t>(VT_BACKGROUND_COLOR, 0);
  }
  bool draw_background_color() const {
    return GetField<uint8_t>(VT_DRAW_BACKGROUND_COLOR, 0) != 0;
  }
  const flatbuffers::String *creation_code() const {
    return GetPointer<const flatbuffers::String *>(VT_CREATION_CODE);
  }
  bool views_enabled() const {
    return GetField<uint8_t>(VT_VIEWS_ENABLED, 0) != 0;
  }
  const flatbuffers::Vector<const RoomView *> *views() const {
    return GetPointer<const flatbuffers::Vector<const RoomView *> *>(VT_VIEWS);
  }
  const flatbuffers::Vector<const RoomBackground *> *backgrounds() const {
    return GetPointer<const flatbuffers::Vector<const RoomBackground *> *>(VT_BACKGROUNDS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<RoomInstance>> *instances() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<RoomInstance>> *>(VT_INSTANCES);
  }
  const flatbuffers::Vector<const RoomTile *> *tiles() const {
    return GetPointer<const flatbuffers::Vector<const RoomTile *> *>(VT_TILES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<RoomEditorMetadata>(verifier, VT_EDITOR_METADATA) &&
           VerifyOffset(verifier, VT_CAPTION) &&
           verifier.Verify(caption()) &&
           VerifyField<int32_t>(verifier, VT_WIDTH) &&
           VerifyField<int32_t>(verifier, VT_HEIGHT) &&
           VerifyField<int32_t>(verifier, VT_SPEED) &&
           VerifyField<uint8_t>(verifier, VT_PERSISTENT) &&
           VerifyField<int32_t>(verifier, VT_BACKGROUND_COLOR) &&
           VerifyField<uint8_t>(verifier, VT_DRAW_BACKGROUND_COLOR) &&
           VerifyOffset(verifier, VT_CREATION_CODE) &&
           verifier.Verify(creation_code()) &&
           VerifyField<uint8_t>(verifier, VT_VIEWS_ENABLED) &&
           VerifyOffset(verifier, VT_VIEWS) &&
           verifier.Verify(views()) &&
           VerifyOffset(verifier, VT_BACKGROUNDS) &&
           verifier.Verify(backgrounds()) &&
           VerifyOffset(verifier, VT_INSTANCES) &&
           verifier.Verify(instances()) &&
           verifier.VerifyVectorOfTables(instances()) &&
           VerifyOffset(verifier, VT_TILES) &&
           verifier.Verify(tiles()) &&
           verifier.EndTable();
  }
};

struct RoomBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Room::VT_NAME, name);
  }
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Room::VT_ID, id, 0);
  }
  void add_editor_metadata(const RoomEditorMetadata *editor_metadata) {
    fbb_.AddStruct(Room::VT_EDITOR_METADATA, editor_metadata);
  }
  void add_caption(flatbuffers::Offset<flatbuffers::String> caption) {
    fbb_.AddOffset(Room::VT_CAPTION, caption);
  }
  void add_width(int32_t width) {
    fbb_.AddElement<int32_t>(Room::VT_WIDTH, width, 0);
  }
  void add_height(int32_t height) {
    fbb_.AddElement<int32_t>(Room::VT_HEIGHT, height, 0);
  }
  void add_speed(int32_t speed) {
    fbb_.AddElement<int32_t>(Room::VT_SPEED, speed, 0);
  }
  void add_persistent(bool persistent) {
    fbb_.AddElement<uint8_t>(Room::VT_PERSISTENT, static_cast<uint8_t>(persistent), 0);
  }
  void add_background_color(int32_t background_color) {
    fbb_.AddElement<int32_t>(Room::VT_BACKGROUND_COLOR, background_color, 0);
  }
  void add_draw_background_color(bool draw_background_color) {
    fbb_.AddElement<uint8_t>(Room::VT_DRAW_BACKGROUND_COLOR, static_cast<uint8_t>(draw_background_color), 0);
  }
  void add_creation_code(flatbuffers::Offset<flatbuffers::String> creation_code) {
    fbb_.AddOffset(Room::VT_CREATION_CODE, creation_code);
  }
  void add_views_enabled(bool views_enabled) {
    fbb_.AddElement<uint8_t>(Room::VT_VIEWS_ENABLED, static_cast<uint8_t>(views_enabled), 0);
  }
  void add_views(flatbuffers::Offset<flatbuffers::Vector<const RoomView *>> views) {
    fbb_.AddOffset(Room::VT_VIEWS, views);
  }
  void add_backgrounds(flatbuffers::Offset<flatbuffers::Vector<const RoomBackground *>> backgrounds) {
    fbb_.AddOffset(Room::VT_BACKGROUNDS, backgrounds);
  }
  void add_instances(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RoomInstance>>> instances) {
    fbb_.AddOffset(Room::VT_INSTANCES, instances);
  }
  void add_tiles(flatbuffers::Offset<flatbuffers::Vector<const RoomTile *>> tiles) {
    fbb_.AddOffset(Room::VT_TILES, tiles);
  }
  explicit RoomBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  RoomBuilder &operator=(const RoomBuilder &);
  flatbuffers::Offset<Room> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Room>(end);
    return o;
  }
};

inline flatbuffers::Offset<Room> CreateRoom(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t id = 0,
    const RoomEditorMetadata *editor_metadata = 0,
    flatbuffers::Offset<flatbuffers::String> caption = 0,
    int32_t width = 0,
    int32_t height = 0,
    int32_t speed = 0,
    bool persistent = false,
    int32_t background_color = 0,
    bool draw_background_color = false,
    flatbuffers::Offset<flatbuffers::String> creation_code = 0,
    bool views_enabled = false,
    flatbuffers::Offset<flatbuffers::Vector<const RoomView *>> views = 0,
    flatbuffers::Offset<flatbuffers::Vector<const RoomBackground *>> backgrounds = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RoomInstance>>> instances = 0,
    flatbuffers::Offset<flatbuffers::Vector<const RoomTile *>> tiles = 0) {
  RoomBuilder builder_(_fbb);
  builder_.add_tiles(tiles);
  builder_.add_instances(instances);
  builder_.add_backgrounds(backgrounds);
  builder_.add_views(views);
  builder_.add_creation_code(creation_code);
  builder_.add_background_color(background_color);
  builder_.add_speed(speed);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_caption(caption);
  builder_.add_editor_metadata(editor_metadata);
  builder_.add_id(id);
  builder_.add_name(name);
  builder_.add_views_enabled(views_enabled);
  builder_.add_draw_background_color(draw_background_color);
  builder_.add_persistent(persistent);
  return builder_.Finish();
}

inline flatbuffers::Offset<Room> CreateRoomDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t id = 0,
    const RoomEditorMetadata *editor_metadata = 0,
    const char *caption = nullptr,
    int32_t width = 0,
    int32_t height = 0,
    int32_t speed = 0,
    bool persistent = false,
    int32_t background_color = 0,
    bool draw_background_color = false,
    const char *creation_code = nullptr,
    bool views_enabled = false,
    const std::vector<const RoomView *> *views = nullptr,
    const std::vector<const RoomBackground *> *backgrounds = nullptr,
    const std::vector<flatbuffers::Offset<RoomInstance>> *instances = nullptr,
    const std::vector<const RoomTile *> *tiles = nullptr) {
  return CreateRoom(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      id,
      editor_metadata,
      caption ? _fbb.CreateString(caption) : 0,
      width,
      height,
      speed,
      persistent,
      background_color,
      draw_background_color,
      creation_code ? _fbb.CreateString(creation_code) : 0,
      views_enabled,
      views ? _fbb.CreateVector<const RoomView *>(*views) : 0,
      backgrounds ? _fbb.CreateVector<const RoomBackground *>(*backgrounds) : 0,
      instances ? _fbb.CreateVector<flatbuffers::Offset<RoomInstance>>(*instances) : 0,
      tiles ? _fbb.CreateVector<const RoomTile *>(*tiles) : 0);
}

inline const Room *GetRoom(const void *buf) {
  return flatbuffers::GetRoot<Room>(buf);
}

inline bool VerifyRoomBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Room>(nullptr);
}

inline void FinishRoomBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Room> root) {
  fbb.Finish(root);
}

#endif  // FLATBUFFERS_GENERATED_ROOM_H_
