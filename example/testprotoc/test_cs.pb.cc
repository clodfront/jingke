// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test_cs.proto

#include "test_cs.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace CSP {
class CsFightReqDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<CsFightReq> _instance;
} _CsFightReq_default_instance_;
class CsFightRspDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<CsFightRsp> _instance;
} _CsFightRsp_default_instance_;
class CsRankTop3ReqDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<CsRankTop3Req> _instance;
} _CsRankTop3Req_default_instance_;
class CsRankTop3RspDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<CsRankTop3Rsp> _instance;
} _CsRankTop3Rsp_default_instance_;
}  // namespace CSP
static void InitDefaultsscc_info_CsFightReq_test_5fcs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CSP::_CsFightReq_default_instance_;
    new (ptr) ::CSP::CsFightReq();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::CSP::CsFightReq::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_CsFightReq_test_5fcs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_CsFightReq_test_5fcs_2eproto}, {}};

static void InitDefaultsscc_info_CsFightRsp_test_5fcs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CSP::_CsFightRsp_default_instance_;
    new (ptr) ::CSP::CsFightRsp();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::CSP::CsFightRsp::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_CsFightRsp_test_5fcs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_CsFightRsp_test_5fcs_2eproto}, {}};

static void InitDefaultsscc_info_CsRankTop3Req_test_5fcs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CSP::_CsRankTop3Req_default_instance_;
    new (ptr) ::CSP::CsRankTop3Req();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::CSP::CsRankTop3Req::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_CsRankTop3Req_test_5fcs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_CsRankTop3Req_test_5fcs_2eproto}, {}};

static void InitDefaultsscc_info_CsRankTop3Rsp_test_5fcs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CSP::_CsRankTop3Rsp_default_instance_;
    new (ptr) ::CSP::CsRankTop3Rsp();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::CSP::CsRankTop3Rsp::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_CsRankTop3Rsp_test_5fcs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_CsRankTop3Rsp_test_5fcs_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_test_5fcs_2eproto[4];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_test_5fcs_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_test_5fcs_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_test_5fcs_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightReq, fighterid_a_),
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightReq, fighterid_b_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightRsp, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightRsp, errcode_),
  PROTOBUF_FIELD_OFFSET(::CSP::CsFightRsp, winnerid_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Req, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Req, order_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Rsp, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Rsp, errcode_),
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Rsp, validnum_),
  PROTOBUF_FIELD_OFFSET(::CSP::CsRankTop3Rsp, ids_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::CSP::CsFightReq)},
  { 7, -1, sizeof(::CSP::CsFightRsp)},
  { 14, -1, sizeof(::CSP::CsRankTop3Req)},
  { 20, -1, sizeof(::CSP::CsRankTop3Rsp)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CSP::_CsFightReq_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CSP::_CsFightRsp_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CSP::_CsRankTop3Req_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CSP::_CsRankTop3Rsp_default_instance_),
};

const char descriptor_table_protodef_test_5fcs_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rtest_cs.proto\022\003CSP\"6\n\nCsFightReq\022\023\n\013fi"
  "ghterid_a\030\001 \001(\r\022\023\n\013fighterid_b\030\002 \001(\r\"/\n\n"
  "CsFightRsp\022\017\n\007errcode\030\001 \001(\005\022\020\n\010winnerid\030"
  "\002 \001(\r\"\036\n\rCsRankTop3Req\022\r\n\005order\030\001 \001(\005\"\?\n"
  "\rCsRankTop3Rsp\022\017\n\007errcode\030\001 \001(\005\022\020\n\010valid"
  "num\030\002 \001(\r\022\013\n\003ids\030\003 \003(\r*\211\001\n\tEnumCsCmd\022\020\n\014"
  "cs_cmd_begin\020\000\022\024\n\020cs_cmd_fight_req\020\001\022\024\n\020"
  "cs_cmd_fight_rsp\020\002\022\036\n\032cs_cmd_query_rank_"
  "top3_req\020\003\022\036\n\032cs_cmd_query_rank_top3_rsp"
  "\020\004b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_test_5fcs_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_test_5fcs_2eproto_sccs[4] = {
  &scc_info_CsFightReq_test_5fcs_2eproto.base,
  &scc_info_CsFightRsp_test_5fcs_2eproto.base,
  &scc_info_CsRankTop3Req_test_5fcs_2eproto.base,
  &scc_info_CsRankTop3Rsp_test_5fcs_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_test_5fcs_2eproto_once;
static bool descriptor_table_test_5fcs_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_test_5fcs_2eproto = {
  &descriptor_table_test_5fcs_2eproto_initialized, descriptor_table_protodef_test_5fcs_2eproto, "test_cs.proto", 370,
  &descriptor_table_test_5fcs_2eproto_once, descriptor_table_test_5fcs_2eproto_sccs, descriptor_table_test_5fcs_2eproto_deps, 4, 0,
  schemas, file_default_instances, TableStruct_test_5fcs_2eproto::offsets,
  file_level_metadata_test_5fcs_2eproto, 4, file_level_enum_descriptors_test_5fcs_2eproto, file_level_service_descriptors_test_5fcs_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_test_5fcs_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_test_5fcs_2eproto), true);
namespace CSP {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* EnumCsCmd_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_test_5fcs_2eproto);
  return file_level_enum_descriptors_test_5fcs_2eproto[0];
}
bool EnumCsCmd_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void CsFightReq::InitAsDefaultInstance() {
}
class CsFightReq::_Internal {
 public:
};

CsFightReq::CsFightReq()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:CSP.CsFightReq)
}
CsFightReq::CsFightReq(const CsFightReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&fighterid_a_, &from.fighterid_a_,
    static_cast<size_t>(reinterpret_cast<char*>(&fighterid_b_) -
    reinterpret_cast<char*>(&fighterid_a_)) + sizeof(fighterid_b_));
  // @@protoc_insertion_point(copy_constructor:CSP.CsFightReq)
}

void CsFightReq::SharedCtor() {
  ::memset(&fighterid_a_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&fighterid_b_) -
      reinterpret_cast<char*>(&fighterid_a_)) + sizeof(fighterid_b_));
}

CsFightReq::~CsFightReq() {
  // @@protoc_insertion_point(destructor:CSP.CsFightReq)
  SharedDtor();
}

void CsFightReq::SharedDtor() {
}

void CsFightReq::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const CsFightReq& CsFightReq::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_CsFightReq_test_5fcs_2eproto.base);
  return *internal_default_instance();
}


void CsFightReq::Clear() {
// @@protoc_insertion_point(message_clear_start:CSP.CsFightReq)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&fighterid_a_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&fighterid_b_) -
      reinterpret_cast<char*>(&fighterid_a_)) + sizeof(fighterid_b_));
  _internal_metadata_.Clear();
}

const char* CsFightReq::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint32 fighterid_a = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          fighterid_a_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 fighterid_b = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          fighterid_b_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* CsFightReq::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CSP.CsFightReq)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 fighterid_a = 1;
  if (this->fighterid_a() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_fighterid_a(), target);
  }

  // uint32 fighterid_b = 2;
  if (this->fighterid_b() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_fighterid_b(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CSP.CsFightReq)
  return target;
}

size_t CsFightReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CSP.CsFightReq)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint32 fighterid_a = 1;
  if (this->fighterid_a() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_fighterid_a());
  }

  // uint32 fighterid_b = 2;
  if (this->fighterid_b() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_fighterid_b());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void CsFightReq::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CSP.CsFightReq)
  GOOGLE_DCHECK_NE(&from, this);
  const CsFightReq* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<CsFightReq>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CSP.CsFightReq)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CSP.CsFightReq)
    MergeFrom(*source);
  }
}

void CsFightReq::MergeFrom(const CsFightReq& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CSP.CsFightReq)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.fighterid_a() != 0) {
    _internal_set_fighterid_a(from._internal_fighterid_a());
  }
  if (from.fighterid_b() != 0) {
    _internal_set_fighterid_b(from._internal_fighterid_b());
  }
}

void CsFightReq::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CSP.CsFightReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CsFightReq::CopyFrom(const CsFightReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CSP.CsFightReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CsFightReq::IsInitialized() const {
  return true;
}

void CsFightReq::InternalSwap(CsFightReq* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(fighterid_a_, other->fighterid_a_);
  swap(fighterid_b_, other->fighterid_b_);
}

::PROTOBUF_NAMESPACE_ID::Metadata CsFightReq::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void CsFightRsp::InitAsDefaultInstance() {
}
class CsFightRsp::_Internal {
 public:
};

CsFightRsp::CsFightRsp()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:CSP.CsFightRsp)
}
CsFightRsp::CsFightRsp(const CsFightRsp& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&errcode_, &from.errcode_,
    static_cast<size_t>(reinterpret_cast<char*>(&winnerid_) -
    reinterpret_cast<char*>(&errcode_)) + sizeof(winnerid_));
  // @@protoc_insertion_point(copy_constructor:CSP.CsFightRsp)
}

void CsFightRsp::SharedCtor() {
  ::memset(&errcode_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&winnerid_) -
      reinterpret_cast<char*>(&errcode_)) + sizeof(winnerid_));
}

CsFightRsp::~CsFightRsp() {
  // @@protoc_insertion_point(destructor:CSP.CsFightRsp)
  SharedDtor();
}

void CsFightRsp::SharedDtor() {
}

void CsFightRsp::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const CsFightRsp& CsFightRsp::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_CsFightRsp_test_5fcs_2eproto.base);
  return *internal_default_instance();
}


void CsFightRsp::Clear() {
// @@protoc_insertion_point(message_clear_start:CSP.CsFightRsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&errcode_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&winnerid_) -
      reinterpret_cast<char*>(&errcode_)) + sizeof(winnerid_));
  _internal_metadata_.Clear();
}

const char* CsFightRsp::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 errcode = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          errcode_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 winnerid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          winnerid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* CsFightRsp::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CSP.CsFightRsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 errcode = 1;
  if (this->errcode() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_errcode(), target);
  }

  // uint32 winnerid = 2;
  if (this->winnerid() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_winnerid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CSP.CsFightRsp)
  return target;
}

size_t CsFightRsp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CSP.CsFightRsp)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 errcode = 1;
  if (this->errcode() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_errcode());
  }

  // uint32 winnerid = 2;
  if (this->winnerid() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_winnerid());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void CsFightRsp::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CSP.CsFightRsp)
  GOOGLE_DCHECK_NE(&from, this);
  const CsFightRsp* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<CsFightRsp>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CSP.CsFightRsp)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CSP.CsFightRsp)
    MergeFrom(*source);
  }
}

void CsFightRsp::MergeFrom(const CsFightRsp& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CSP.CsFightRsp)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.errcode() != 0) {
    _internal_set_errcode(from._internal_errcode());
  }
  if (from.winnerid() != 0) {
    _internal_set_winnerid(from._internal_winnerid());
  }
}

void CsFightRsp::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CSP.CsFightRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CsFightRsp::CopyFrom(const CsFightRsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CSP.CsFightRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CsFightRsp::IsInitialized() const {
  return true;
}

void CsFightRsp::InternalSwap(CsFightRsp* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(errcode_, other->errcode_);
  swap(winnerid_, other->winnerid_);
}

::PROTOBUF_NAMESPACE_ID::Metadata CsFightRsp::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void CsRankTop3Req::InitAsDefaultInstance() {
}
class CsRankTop3Req::_Internal {
 public:
};

CsRankTop3Req::CsRankTop3Req()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:CSP.CsRankTop3Req)
}
CsRankTop3Req::CsRankTop3Req(const CsRankTop3Req& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  order_ = from.order_;
  // @@protoc_insertion_point(copy_constructor:CSP.CsRankTop3Req)
}

void CsRankTop3Req::SharedCtor() {
  order_ = 0;
}

CsRankTop3Req::~CsRankTop3Req() {
  // @@protoc_insertion_point(destructor:CSP.CsRankTop3Req)
  SharedDtor();
}

void CsRankTop3Req::SharedDtor() {
}

void CsRankTop3Req::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const CsRankTop3Req& CsRankTop3Req::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_CsRankTop3Req_test_5fcs_2eproto.base);
  return *internal_default_instance();
}


void CsRankTop3Req::Clear() {
// @@protoc_insertion_point(message_clear_start:CSP.CsRankTop3Req)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  order_ = 0;
  _internal_metadata_.Clear();
}

const char* CsRankTop3Req::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 order = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          order_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* CsRankTop3Req::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CSP.CsRankTop3Req)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 order = 1;
  if (this->order() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_order(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CSP.CsRankTop3Req)
  return target;
}

size_t CsRankTop3Req::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CSP.CsRankTop3Req)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 order = 1;
  if (this->order() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_order());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void CsRankTop3Req::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CSP.CsRankTop3Req)
  GOOGLE_DCHECK_NE(&from, this);
  const CsRankTop3Req* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<CsRankTop3Req>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CSP.CsRankTop3Req)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CSP.CsRankTop3Req)
    MergeFrom(*source);
  }
}

void CsRankTop3Req::MergeFrom(const CsRankTop3Req& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CSP.CsRankTop3Req)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.order() != 0) {
    _internal_set_order(from._internal_order());
  }
}

void CsRankTop3Req::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CSP.CsRankTop3Req)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CsRankTop3Req::CopyFrom(const CsRankTop3Req& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CSP.CsRankTop3Req)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CsRankTop3Req::IsInitialized() const {
  return true;
}

void CsRankTop3Req::InternalSwap(CsRankTop3Req* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(order_, other->order_);
}

::PROTOBUF_NAMESPACE_ID::Metadata CsRankTop3Req::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void CsRankTop3Rsp::InitAsDefaultInstance() {
}
class CsRankTop3Rsp::_Internal {
 public:
};

CsRankTop3Rsp::CsRankTop3Rsp()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:CSP.CsRankTop3Rsp)
}
CsRankTop3Rsp::CsRankTop3Rsp(const CsRankTop3Rsp& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      ids_(from.ids_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&errcode_, &from.errcode_,
    static_cast<size_t>(reinterpret_cast<char*>(&validnum_) -
    reinterpret_cast<char*>(&errcode_)) + sizeof(validnum_));
  // @@protoc_insertion_point(copy_constructor:CSP.CsRankTop3Rsp)
}

void CsRankTop3Rsp::SharedCtor() {
  ::memset(&errcode_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&validnum_) -
      reinterpret_cast<char*>(&errcode_)) + sizeof(validnum_));
}

CsRankTop3Rsp::~CsRankTop3Rsp() {
  // @@protoc_insertion_point(destructor:CSP.CsRankTop3Rsp)
  SharedDtor();
}

void CsRankTop3Rsp::SharedDtor() {
}

void CsRankTop3Rsp::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const CsRankTop3Rsp& CsRankTop3Rsp::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_CsRankTop3Rsp_test_5fcs_2eproto.base);
  return *internal_default_instance();
}


void CsRankTop3Rsp::Clear() {
// @@protoc_insertion_point(message_clear_start:CSP.CsRankTop3Rsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ids_.Clear();
  ::memset(&errcode_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&validnum_) -
      reinterpret_cast<char*>(&errcode_)) + sizeof(validnum_));
  _internal_metadata_.Clear();
}

const char* CsRankTop3Rsp::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 errcode = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          errcode_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 validnum = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          validnum_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated uint32 ids = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedUInt32Parser(_internal_mutable_ids(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24) {
          _internal_add_ids(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* CsRankTop3Rsp::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CSP.CsRankTop3Rsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 errcode = 1;
  if (this->errcode() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_errcode(), target);
  }

  // uint32 validnum = 2;
  if (this->validnum() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_validnum(), target);
  }

  // repeated uint32 ids = 3;
  {
    int byte_size = _ids_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteUInt32Packed(
          3, _internal_ids(), byte_size, target);
    }
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CSP.CsRankTop3Rsp)
  return target;
}

size_t CsRankTop3Rsp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CSP.CsRankTop3Rsp)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated uint32 ids = 3;
  {
    size_t data_size = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      UInt32Size(this->ids_);
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _ids_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // int32 errcode = 1;
  if (this->errcode() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_errcode());
  }

  // uint32 validnum = 2;
  if (this->validnum() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_validnum());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void CsRankTop3Rsp::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CSP.CsRankTop3Rsp)
  GOOGLE_DCHECK_NE(&from, this);
  const CsRankTop3Rsp* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<CsRankTop3Rsp>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CSP.CsRankTop3Rsp)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CSP.CsRankTop3Rsp)
    MergeFrom(*source);
  }
}

void CsRankTop3Rsp::MergeFrom(const CsRankTop3Rsp& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CSP.CsRankTop3Rsp)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  ids_.MergeFrom(from.ids_);
  if (from.errcode() != 0) {
    _internal_set_errcode(from._internal_errcode());
  }
  if (from.validnum() != 0) {
    _internal_set_validnum(from._internal_validnum());
  }
}

void CsRankTop3Rsp::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CSP.CsRankTop3Rsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CsRankTop3Rsp::CopyFrom(const CsRankTop3Rsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CSP.CsRankTop3Rsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CsRankTop3Rsp::IsInitialized() const {
  return true;
}

void CsRankTop3Rsp::InternalSwap(CsRankTop3Rsp* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  ids_.InternalSwap(&other->ids_);
  swap(errcode_, other->errcode_);
  swap(validnum_, other->validnum_);
}

::PROTOBUF_NAMESPACE_ID::Metadata CsRankTop3Rsp::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace CSP
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::CSP::CsFightReq* Arena::CreateMaybeMessage< ::CSP::CsFightReq >(Arena* arena) {
  return Arena::CreateInternal< ::CSP::CsFightReq >(arena);
}
template<> PROTOBUF_NOINLINE ::CSP::CsFightRsp* Arena::CreateMaybeMessage< ::CSP::CsFightRsp >(Arena* arena) {
  return Arena::CreateInternal< ::CSP::CsFightRsp >(arena);
}
template<> PROTOBUF_NOINLINE ::CSP::CsRankTop3Req* Arena::CreateMaybeMessage< ::CSP::CsRankTop3Req >(Arena* arena) {
  return Arena::CreateInternal< ::CSP::CsRankTop3Req >(arena);
}
template<> PROTOBUF_NOINLINE ::CSP::CsRankTop3Rsp* Arena::CreateMaybeMessage< ::CSP::CsRankTop3Rsp >(Arena* arena) {
  return Arena::CreateInternal< ::CSP::CsRankTop3Rsp >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
