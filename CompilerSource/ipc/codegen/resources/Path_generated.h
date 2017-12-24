// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PATH_H_
#define FLATBUFFERS_GENERATED_PATH_H_

#include "flatbuffers/flatbuffers.h"

struct PathPoint;

struct PathEditorMetadata;

struct Path;

MANUALLY_ALIGNED_STRUCT(4) PathPoint FLATBUFFERS_FINAL_CLASS {
 private:
  int32_t x_;
  int32_t y_;
  int32_t speed_;

 public:
  PathPoint() {
    memset(this, 0, sizeof(PathPoint));
  }
  PathPoint(int32_t _x, int32_t _y, int32_t _speed)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        speed_(flatbuffers::EndianScalar(_speed)) {
  }
  int32_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int32_t y() const {
    return flatbuffers::EndianScalar(y_);
  }
  int32_t speed() const {
    return flatbuffers::EndianScalar(speed_);
  }
};
STRUCT_END(PathPoint, 12);

struct PathEditorMetadata FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_BACKGROUND_ROOM_NAME = 4,
    VT_SNAP_X = 6,
    VT_SNAP_Y = 8
  };
  const flatbuffers::String *background_room_name() const {
    return GetPointer<const flatbuffers::String *>(VT_BACKGROUND_ROOM_NAME);
  }
  int32_t snap_x() const {
    return GetField<int32_t>(VT_SNAP_X, 0);
  }
  int32_t snap_y() const {
    return GetField<int32_t>(VT_SNAP_Y, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BACKGROUND_ROOM_NAME) &&
           verifier.Verify(background_room_name()) &&
           VerifyField<int32_t>(verifier, VT_SNAP_X) &&
           VerifyField<int32_t>(verifier, VT_SNAP_Y) &&
           verifier.EndTable();
  }
};

struct PathEditorMetadataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_background_room_name(flatbuffers::Offset<flatbuffers::String> background_room_name) {
    fbb_.AddOffset(PathEditorMetadata::VT_BACKGROUND_ROOM_NAME, background_room_name);
  }
  void add_snap_x(int32_t snap_x) {
    fbb_.AddElement<int32_t>(PathEditorMetadata::VT_SNAP_X, snap_x, 0);
  }
  void add_snap_y(int32_t snap_y) {
    fbb_.AddElement<int32_t>(PathEditorMetadata::VT_SNAP_Y, snap_y, 0);
  }
  explicit PathEditorMetadataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  PathEditorMetadataBuilder &operator=(const PathEditorMetadataBuilder &);
  flatbuffers::Offset<PathEditorMetadata> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<PathEditorMetadata>(end);
    return o;
  }
};

inline flatbuffers::Offset<PathEditorMetadata> CreatePathEditorMetadata(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> background_room_name = 0,
    int32_t snap_x = 0,
    int32_t snap_y = 0) {
  PathEditorMetadataBuilder builder_(_fbb);
  builder_.add_snap_y(snap_y);
  builder_.add_snap_x(snap_x);
  builder_.add_background_room_name(background_room_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<PathEditorMetadata> CreatePathEditorMetadataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *background_room_name = nullptr,
    int32_t snap_x = 0,
    int32_t snap_y = 0) {
  return CreatePathEditorMetadata(
      _fbb,
      background_room_name ? _fbb.CreateString(background_room_name) : 0,
      snap_x,
      snap_y);
}

struct Path FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_ID = 6,
    VT_EDITOR_METADATA = 8,
    VT_SMOOTH = 10,
    VT_CLOSED = 12,
    VT_PRECISION = 14,
    VT_POINTS = 16
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  const PathEditorMetadata *editor_metadata() const {
    return GetPointer<const PathEditorMetadata *>(VT_EDITOR_METADATA);
  }
  bool smooth() const {
    return GetField<uint8_t>(VT_SMOOTH, 0) != 0;
  }
  bool closed() const {
    return GetField<uint8_t>(VT_CLOSED, 0) != 0;
  }
  int32_t precision() const {
    return GetField<int32_t>(VT_PRECISION, 0);
  }
  const flatbuffers::Vector<const PathPoint *> *points() const {
    return GetPointer<const flatbuffers::Vector<const PathPoint *> *>(VT_POINTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyOffset(verifier, VT_EDITOR_METADATA) &&
           verifier.VerifyTable(editor_metadata()) &&
           VerifyField<uint8_t>(verifier, VT_SMOOTH) &&
           VerifyField<uint8_t>(verifier, VT_CLOSED) &&
           VerifyField<int32_t>(verifier, VT_PRECISION) &&
           VerifyOffset(verifier, VT_POINTS) &&
           verifier.Verify(points()) &&
           verifier.EndTable();
  }
};

struct PathBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Path::VT_NAME, name);
  }
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Path::VT_ID, id, 0);
  }
  void add_editor_metadata(flatbuffers::Offset<PathEditorMetadata> editor_metadata) {
    fbb_.AddOffset(Path::VT_EDITOR_METADATA, editor_metadata);
  }
  void add_smooth(bool smooth) {
    fbb_.AddElement<uint8_t>(Path::VT_SMOOTH, static_cast<uint8_t>(smooth), 0);
  }
  void add_closed(bool closed) {
    fbb_.AddElement<uint8_t>(Path::VT_CLOSED, static_cast<uint8_t>(closed), 0);
  }
  void add_precision(int32_t precision) {
    fbb_.AddElement<int32_t>(Path::VT_PRECISION, precision, 0);
  }
  void add_points(flatbuffers::Offset<flatbuffers::Vector<const PathPoint *>> points) {
    fbb_.AddOffset(Path::VT_POINTS, points);
  }
  explicit PathBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  PathBuilder &operator=(const PathBuilder &);
  flatbuffers::Offset<Path> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Path>(end);
    return o;
  }
};

inline flatbuffers::Offset<Path> CreatePath(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t id = 0,
    flatbuffers::Offset<PathEditorMetadata> editor_metadata = 0,
    bool smooth = false,
    bool closed = false,
    int32_t precision = 0,
    flatbuffers::Offset<flatbuffers::Vector<const PathPoint *>> points = 0) {
  PathBuilder builder_(_fbb);
  builder_.add_points(points);
  builder_.add_precision(precision);
  builder_.add_editor_metadata(editor_metadata);
  builder_.add_id(id);
  builder_.add_name(name);
  builder_.add_closed(closed);
  builder_.add_smooth(smooth);
  return builder_.Finish();
}

inline flatbuffers::Offset<Path> CreatePathDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t id = 0,
    flatbuffers::Offset<PathEditorMetadata> editor_metadata = 0,
    bool smooth = false,
    bool closed = false,
    int32_t precision = 0,
    const std::vector<const PathPoint *> *points = nullptr) {
  return CreatePath(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      id,
      editor_metadata,
      smooth,
      closed,
      precision,
      points ? _fbb.CreateVector<const PathPoint *>(*points) : 0);
}

inline const Path *GetPath(const void *buf) {
  return flatbuffers::GetRoot<Path>(buf);
}

inline bool VerifyPathBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Path>(nullptr);
}

inline void FinishPathBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Path> root) {
  fbb.Finish(root);
}

#endif  // FLATBUFFERS_GENERATED_PATH_H_
