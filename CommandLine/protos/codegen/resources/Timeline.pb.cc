// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: resources/Timeline.proto

#include "resources/Timeline.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace buffers {
namespace resources {
class Timeline_MomentDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Timeline_Moment>
      _instance;
} _Timeline_Moment_default_instance_;
class TimelineDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Timeline>
      _instance;
} _Timeline_default_instance_;
}  // namespace resources
}  // namespace buffers
namespace protobuf_resources_2fTimeline_2eproto {
void InitDefaultsTimeline_MomentImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  protobuf_resources_2fEvent_2eproto::InitDefaultsEvent();
  {
    void* ptr = &::buffers::resources::_Timeline_Moment_default_instance_;
    new (ptr) ::buffers::resources::Timeline_Moment();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::buffers::resources::Timeline_Moment::InitAsDefaultInstance();
}

void InitDefaultsTimeline_Moment() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsTimeline_MomentImpl);
}

void InitDefaultsTimelineImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  protobuf_resources_2fTimeline_2eproto::InitDefaultsTimeline_Moment();
  {
    void* ptr = &::buffers::resources::_Timeline_default_instance_;
    new (ptr) ::buffers::resources::Timeline();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::buffers::resources::Timeline::InitAsDefaultInstance();
}

void InitDefaultsTimeline() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsTimelineImpl);
}

::google::protobuf::Metadata file_level_metadata[2];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline_Moment, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline_Moment, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline_Moment, step_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline_Moment, event_),
  1,
  0,
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline, name_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline, id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::buffers::resources::Timeline, moments_),
  0,
  1,
  ~0u,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 7, sizeof(::buffers::resources::Timeline_Moment)},
  { 9, 17, sizeof(::buffers::resources::Timeline)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::buffers::resources::_Timeline_Moment_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::buffers::resources::_Timeline_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "resources/Timeline.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\030resources/Timeline.proto\022\021buffers.reso"
      "urces\032\roptions.proto\032\025resources/Event.pr"
      "oto\"\271\001\n\010Timeline\022\014\n\004name\030\001 \001(\t\022\036\n\002id\030\002 \001"
      "(\005B\022\202\265\030\016GMX_DEPRECATED\022>\n\007moments\030\003 \003(\0132"
      "\".buffers.resources.Timeline.MomentB\t\202\265\030"
      "\005entry\032\?\n\006Moment\022\014\n\004step\030\001 \001(\005\022\'\n\005event\030"
      "\002 \001(\0132\030.buffers.resources.Event"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 271);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "resources/Timeline.proto", &protobuf_RegisterTypes);
  ::protobuf_options_2eproto::AddDescriptors();
  ::protobuf_resources_2fEvent_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_resources_2fTimeline_2eproto
namespace buffers {
namespace resources {

// ===================================================================

void Timeline_Moment::InitAsDefaultInstance() {
  ::buffers::resources::_Timeline_Moment_default_instance_._instance.get_mutable()->event_ = const_cast< ::buffers::resources::Event*>(
      ::buffers::resources::Event::internal_default_instance());
}
void Timeline_Moment::clear_event() {
  if (event_ != NULL) event_->Clear();
  clear_has_event();
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Timeline_Moment::kStepFieldNumber;
const int Timeline_Moment::kEventFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Timeline_Moment::Timeline_Moment()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_resources_2fTimeline_2eproto::InitDefaultsTimeline_Moment();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:buffers.resources.Timeline.Moment)
}
Timeline_Moment::Timeline_Moment(const Timeline_Moment& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_event()) {
    event_ = new ::buffers::resources::Event(*from.event_);
  } else {
    event_ = NULL;
  }
  step_ = from.step_;
  // @@protoc_insertion_point(copy_constructor:buffers.resources.Timeline.Moment)
}

void Timeline_Moment::SharedCtor() {
  _cached_size_ = 0;
  ::memset(&event_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&step_) -
      reinterpret_cast<char*>(&event_)) + sizeof(step_));
}

Timeline_Moment::~Timeline_Moment() {
  // @@protoc_insertion_point(destructor:buffers.resources.Timeline.Moment)
  SharedDtor();
}

void Timeline_Moment::SharedDtor() {
  if (this != internal_default_instance()) delete event_;
}

void Timeline_Moment::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Timeline_Moment::descriptor() {
  ::protobuf_resources_2fTimeline_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_resources_2fTimeline_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Timeline_Moment& Timeline_Moment::default_instance() {
  ::protobuf_resources_2fTimeline_2eproto::InitDefaultsTimeline_Moment();
  return *internal_default_instance();
}

Timeline_Moment* Timeline_Moment::New(::google::protobuf::Arena* arena) const {
  Timeline_Moment* n = new Timeline_Moment;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Timeline_Moment::Clear() {
// @@protoc_insertion_point(message_clear_start:buffers.resources.Timeline.Moment)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    GOOGLE_DCHECK(event_ != NULL);
    event_->Clear();
  }
  step_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Timeline_Moment::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:buffers.resources.Timeline.Moment)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 step = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {
          set_has_step();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &step_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .buffers.resources.Event event = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_event()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:buffers.resources.Timeline.Moment)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:buffers.resources.Timeline.Moment)
  return false;
#undef DO_
}

void Timeline_Moment::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:buffers.resources.Timeline.Moment)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional int32 step = 1;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->step(), output);
  }

  // optional .buffers.resources.Event event = 2;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, *this->event_, output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:buffers.resources.Timeline.Moment)
}

::google::protobuf::uint8* Timeline_Moment::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:buffers.resources.Timeline.Moment)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional int32 step = 1;
  if (cached_has_bits & 0x00000002u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->step(), target);
  }

  // optional .buffers.resources.Event event = 2;
  if (cached_has_bits & 0x00000001u) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        2, *this->event_, deterministic, target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:buffers.resources.Timeline.Moment)
  return target;
}

size_t Timeline_Moment::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:buffers.resources.Timeline.Moment)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  if (_has_bits_[0 / 32] & 3u) {
    // optional .buffers.resources.Event event = 2;
    if (has_event()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *this->event_);
    }

    // optional int32 step = 1;
    if (has_step()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->step());
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Timeline_Moment::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:buffers.resources.Timeline.Moment)
  GOOGLE_DCHECK_NE(&from, this);
  const Timeline_Moment* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Timeline_Moment>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:buffers.resources.Timeline.Moment)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:buffers.resources.Timeline.Moment)
    MergeFrom(*source);
  }
}

void Timeline_Moment::MergeFrom(const Timeline_Moment& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:buffers.resources.Timeline.Moment)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 3u) {
    if (cached_has_bits & 0x00000001u) {
      mutable_event()->::buffers::resources::Event::MergeFrom(from.event());
    }
    if (cached_has_bits & 0x00000002u) {
      step_ = from.step_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Timeline_Moment::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:buffers.resources.Timeline.Moment)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Timeline_Moment::CopyFrom(const Timeline_Moment& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:buffers.resources.Timeline.Moment)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Timeline_Moment::IsInitialized() const {
  return true;
}

void Timeline_Moment::Swap(Timeline_Moment* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Timeline_Moment::InternalSwap(Timeline_Moment* other) {
  using std::swap;
  swap(event_, other->event_);
  swap(step_, other->step_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Timeline_Moment::GetMetadata() const {
  protobuf_resources_2fTimeline_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_resources_2fTimeline_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void Timeline::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Timeline::kNameFieldNumber;
const int Timeline::kIdFieldNumber;
const int Timeline::kMomentsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Timeline::Timeline()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_resources_2fTimeline_2eproto::InitDefaultsTimeline();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:buffers.resources.Timeline)
}
Timeline::Timeline(const Timeline& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0),
      moments_(from.moments_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_name()) {
    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  id_ = from.id_;
  // @@protoc_insertion_point(copy_constructor:buffers.resources.Timeline)
}

void Timeline::SharedCtor() {
  _cached_size_ = 0;
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
}

Timeline::~Timeline() {
  // @@protoc_insertion_point(destructor:buffers.resources.Timeline)
  SharedDtor();
}

void Timeline::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Timeline::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Timeline::descriptor() {
  ::protobuf_resources_2fTimeline_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_resources_2fTimeline_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Timeline& Timeline::default_instance() {
  ::protobuf_resources_2fTimeline_2eproto::InitDefaultsTimeline();
  return *internal_default_instance();
}

Timeline* Timeline::New(::google::protobuf::Arena* arena) const {
  Timeline* n = new Timeline;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Timeline::Clear() {
// @@protoc_insertion_point(message_clear_start:buffers.resources.Timeline)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  moments_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    GOOGLE_DCHECK(!name_.IsDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited()));
    (*name_.UnsafeRawStringPointer())->clear();
  }
  id_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Timeline::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:buffers.resources.Timeline)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string name = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), static_cast<int>(this->name().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "buffers.resources.Timeline.name");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 id = 2 [(.buffers.gmx) = "GMX_DEPRECATED"];
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          set_has_id();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .buffers.resources.Timeline.Moment moments = 3 [(.buffers.gmx) = "entry"];
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u /* 26 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(input, add_moments()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:buffers.resources.Timeline)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:buffers.resources.Timeline)
  return false;
#undef DO_
}

void Timeline::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:buffers.resources.Timeline)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "buffers.resources.Timeline.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // optional int32 id = 2 [(.buffers.gmx) = "GMX_DEPRECATED"];
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->id(), output);
  }

  // repeated .buffers.resources.Timeline.Moment moments = 3 [(.buffers.gmx) = "entry"];
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->moments_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->moments(static_cast<int>(i)), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:buffers.resources.Timeline)
}

::google::protobuf::uint8* Timeline::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:buffers.resources.Timeline)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "buffers.resources.Timeline.name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // optional int32 id = 2 [(.buffers.gmx) = "GMX_DEPRECATED"];
  if (cached_has_bits & 0x00000002u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->id(), target);
  }

  // repeated .buffers.resources.Timeline.Moment moments = 3 [(.buffers.gmx) = "entry"];
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->moments_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        3, this->moments(static_cast<int>(i)), deterministic, target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:buffers.resources.Timeline)
  return target;
}

size_t Timeline::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:buffers.resources.Timeline)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // repeated .buffers.resources.Timeline.Moment moments = 3 [(.buffers.gmx) = "entry"];
  {
    unsigned int count = static_cast<unsigned int>(this->moments_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->moments(static_cast<int>(i)));
    }
  }

  if (_has_bits_[0 / 32] & 3u) {
    // optional string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // optional int32 id = 2 [(.buffers.gmx) = "GMX_DEPRECATED"];
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->id());
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Timeline::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:buffers.resources.Timeline)
  GOOGLE_DCHECK_NE(&from, this);
  const Timeline* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Timeline>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:buffers.resources.Timeline)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:buffers.resources.Timeline)
    MergeFrom(*source);
  }
}

void Timeline::MergeFrom(const Timeline& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:buffers.resources.Timeline)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  moments_.MergeFrom(from.moments_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 3u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_name();
      name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
    }
    if (cached_has_bits & 0x00000002u) {
      id_ = from.id_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Timeline::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:buffers.resources.Timeline)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Timeline::CopyFrom(const Timeline& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:buffers.resources.Timeline)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Timeline::IsInitialized() const {
  return true;
}

void Timeline::Swap(Timeline* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Timeline::InternalSwap(Timeline* other) {
  using std::swap;
  moments_.InternalSwap(&other->moments_);
  name_.Swap(&other->name_);
  swap(id_, other->id_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Timeline::GetMetadata() const {
  protobuf_resources_2fTimeline_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_resources_2fTimeline_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace resources
}  // namespace buffers

// @@protoc_insertion_point(global_scope)
