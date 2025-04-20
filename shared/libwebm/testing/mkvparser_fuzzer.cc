// Copyright (c) 2022 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

#include "mkvparser/mkvparser.h"
#include "mkvparser/mkvreader.h"

namespace {

class MemoryReader : public mkvparser::IMkvReader {
 public:
  MemoryReader(const uint8_t* data, size_t size) : data_(data), size_(size) {}

  int Read(long long pos, long len, unsigned char* buf) override {
    if (pos < 0 || len < 0) {
      abort();
    }
    if (pos >= size_ || size_ - pos < len) {
      return -1;
    }
    memcpy(buf, data_ + pos, len);
    return 0;
  }

  int Length(long long* total, long long* available) override {
    if (total != nullptr) {
      *total = size_;
    }
    if (available != nullptr) {
      *available = size_;
    }
    return 0;
  }

 private:
  const uint8_t* data_;
  size_t size_;
};

void ParseCues(const mkvparser::Segment& segment) {
  const mkvparser::Cues* const cues = segment.GetCues();
  if (cues == nullptr) {
    return;
  }

  while (!cues->DoneParsing()) {
    cues->LoadCuePoint();
  }
}

void ParseCluster(const mkvparser::Cluster& cluster) {
  const mkvparser::BlockEntry* block_entry;
  long status = cluster.GetFirst(block_entry);
  if (status != 0) {
    return;
  }

  while (block_entry != nullptr && !block_entry->EOS()) {
    const mkvparser::Block* const block = block_entry->GetBlock();
    if (block == nullptr) {
      return;
    }

    status = cluster.GetNext(block_entry, block_entry);
    if (status != 0) {
      return;
    }
  }
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  MemoryReader reader(data, size);

  long long int pos = 0;
  std::unique_ptr<mkvparser::EBMLHeader> ebml_header(
      new (std::nothrow) mkvparser::EBMLHeader());  // NOLINT
  if (ebml_header->Parse(&reader, pos) < 0) {
    return 0;
  }

  mkvparser::Segment* temp_segment;
  if (mkvparser::Segment::CreateInstance(&reader, pos, temp_segment) != 0) {
    return 0;
  }
  std::unique_ptr<mkvparser::Segment> segment(temp_segment);

  if (segment->Load() < 0) {
    return 0;
  }

  const mkvparser::Cluster* cluster = segment->GetFirst();
  while (cluster != nullptr && !cluster->EOS()) {
    ParseCluster(*cluster);
    cluster = segment->GetNext(cluster);
  }
  ParseCues(*segment);

  return 0;
}
