// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_IMAGE_H_
#define FLATBUFFERS_GENERATED_IMAGE_H_

#include "flatbuffers/flatbuffers.h"

struct Image;

struct Image FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_WIDTH = 4,
    VT_HEIGHT = 6,
    VT_DATA = 8
  };
  int32_t width() const {
    return GetField<int32_t>(VT_WIDTH, 0);
  }
  int32_t height() const {
    return GetField<int32_t>(VT_HEIGHT, 0);
  }
  const flatbuffers::Vector<int8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<int8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_WIDTH) &&
           VerifyField<int32_t>(verifier, VT_HEIGHT) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.Verify(data()) &&
           verifier.EndTable();
  }
};

struct ImageBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_width(int32_t width) {
    fbb_.AddElement<int32_t>(Image::VT_WIDTH, width, 0);
  }
  void add_height(int32_t height) {
    fbb_.AddElement<int32_t>(Image::VT_HEIGHT, height, 0);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<int8_t>> data) {
    fbb_.AddOffset(Image::VT_DATA, data);
  }
  explicit ImageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ImageBuilder &operator=(const ImageBuilder &);
  flatbuffers::Offset<Image> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Image>(end);
    return o;
  }
};

inline flatbuffers::Offset<Image> CreateImage(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t width = 0,
    int32_t height = 0,
    flatbuffers::Offset<flatbuffers::Vector<int8_t>> data = 0) {
  ImageBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_height(height);
  builder_.add_width(width);
  return builder_.Finish();
}

inline flatbuffers::Offset<Image> CreateImageDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t width = 0,
    int32_t height = 0,
    const std::vector<int8_t> *data = nullptr) {
  return CreateImage(
      _fbb,
      width,
      height,
      data ? _fbb.CreateVector<int8_t>(*data) : 0);
}

inline const Image *GetImage(const void *buf) {
  return flatbuffers::GetRoot<Image>(buf);
}

inline bool VerifyImageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Image>(nullptr);
}

inline void FinishImageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Image> root) {
  fbb.Finish(root);
}

#endif  // FLATBUFFERS_GENERATED_IMAGE_H_
