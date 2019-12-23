// Code generated by protoc-gen-go. DO NOT EDIT.
// source: api/proto/service.proto

package service

import (
	context "context"
	fmt "fmt"
	proto "github.com/golang/protobuf/proto"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
	math "math"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion3 // please upgrade the proto package

type StoreType int32

const (
	StoreType_BTreeDisk     StoreType = 0
	StoreType_BPlusTreeDisk StoreType = 1
)

var StoreType_name = map[int32]string{
	0: "BTreeDisk",
	1: "BPlusTreeDisk",
}

var StoreType_value = map[string]int32{
	"BTreeDisk":     0,
	"BPlusTreeDisk": 1,
}

func (x StoreType) String() string {
	return proto.EnumName(StoreType_name, int32(x))
}

func (StoreType) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{0}
}

type Status struct {
	//code = 1 means success
	Code                 int32    `protobuf:"varint,1,opt,name=code,proto3" json:"code,omitempty"`
	Error                string   `protobuf:"bytes,2,opt,name=error,proto3" json:"error,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *Status) Reset()         { *m = Status{} }
func (m *Status) String() string { return proto.CompactTextString(m) }
func (*Status) ProtoMessage()    {}
func (*Status) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{0}
}

func (m *Status) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_Status.Unmarshal(m, b)
}
func (m *Status) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_Status.Marshal(b, m, deterministic)
}
func (m *Status) XXX_Merge(src proto.Message) {
	xxx_messageInfo_Status.Merge(m, src)
}
func (m *Status) XXX_Size() int {
	return xxx_messageInfo_Status.Size(m)
}
func (m *Status) XXX_DiscardUnknown() {
	xxx_messageInfo_Status.DiscardUnknown(m)
}

var xxx_messageInfo_Status proto.InternalMessageInfo

func (m *Status) GetCode() int32 {
	if m != nil {
		return m.Code
	}
	return 0
}

func (m *Status) GetError() string {
	if m != nil {
		return m.Error
	}
	return ""
}

type MessageResponse struct {
	Status               *Status  `protobuf:"bytes,1,opt,name=status,proto3" json:"status,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *MessageResponse) Reset()         { *m = MessageResponse{} }
func (m *MessageResponse) String() string { return proto.CompactTextString(m) }
func (*MessageResponse) ProtoMessage()    {}
func (*MessageResponse) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{1}
}

func (m *MessageResponse) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_MessageResponse.Unmarshal(m, b)
}
func (m *MessageResponse) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_MessageResponse.Marshal(b, m, deterministic)
}
func (m *MessageResponse) XXX_Merge(src proto.Message) {
	xxx_messageInfo_MessageResponse.Merge(m, src)
}
func (m *MessageResponse) XXX_Size() int {
	return xxx_messageInfo_MessageResponse.Size(m)
}
func (m *MessageResponse) XXX_DiscardUnknown() {
	xxx_messageInfo_MessageResponse.DiscardUnknown(m)
}

var xxx_messageInfo_MessageResponse proto.InternalMessageInfo

func (m *MessageResponse) GetStatus() *Status {
	if m != nil {
		return m.Status
	}
	return nil
}

type ConnectionRequest struct {
	Type                 StoreType `protobuf:"varint,1,opt,name=type,proto3,enum=service.StoreType" json:"type,omitempty"`
	XXX_NoUnkeyedLiteral struct{}  `json:"-"`
	XXX_unrecognized     []byte    `json:"-"`
	XXX_sizecache        int32     `json:"-"`
}

func (m *ConnectionRequest) Reset()         { *m = ConnectionRequest{} }
func (m *ConnectionRequest) String() string { return proto.CompactTextString(m) }
func (*ConnectionRequest) ProtoMessage()    {}
func (*ConnectionRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{2}
}

func (m *ConnectionRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_ConnectionRequest.Unmarshal(m, b)
}
func (m *ConnectionRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_ConnectionRequest.Marshal(b, m, deterministic)
}
func (m *ConnectionRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_ConnectionRequest.Merge(m, src)
}
func (m *ConnectionRequest) XXX_Size() int {
	return xxx_messageInfo_ConnectionRequest.Size(m)
}
func (m *ConnectionRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_ConnectionRequest.DiscardUnknown(m)
}

var xxx_messageInfo_ConnectionRequest proto.InternalMessageInfo

func (m *ConnectionRequest) GetType() StoreType {
	if m != nil {
		return m.Type
	}
	return StoreType_BTreeDisk
}

type DisconnectRequest struct {
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *DisconnectRequest) Reset()         { *m = DisconnectRequest{} }
func (m *DisconnectRequest) String() string { return proto.CompactTextString(m) }
func (*DisconnectRequest) ProtoMessage()    {}
func (*DisconnectRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{3}
}

func (m *DisconnectRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_DisconnectRequest.Unmarshal(m, b)
}
func (m *DisconnectRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_DisconnectRequest.Marshal(b, m, deterministic)
}
func (m *DisconnectRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_DisconnectRequest.Merge(m, src)
}
func (m *DisconnectRequest) XXX_Size() int {
	return xxx_messageInfo_DisconnectRequest.Size(m)
}
func (m *DisconnectRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_DisconnectRequest.DiscardUnknown(m)
}

var xxx_messageInfo_DisconnectRequest proto.InternalMessageInfo

type GetRequest struct {
	Key                  string   `protobuf:"bytes,1,opt,name=key,proto3" json:"key,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *GetRequest) Reset()         { *m = GetRequest{} }
func (m *GetRequest) String() string { return proto.CompactTextString(m) }
func (*GetRequest) ProtoMessage()    {}
func (*GetRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{4}
}

func (m *GetRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_GetRequest.Unmarshal(m, b)
}
func (m *GetRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_GetRequest.Marshal(b, m, deterministic)
}
func (m *GetRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_GetRequest.Merge(m, src)
}
func (m *GetRequest) XXX_Size() int {
	return xxx_messageInfo_GetRequest.Size(m)
}
func (m *GetRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_GetRequest.DiscardUnknown(m)
}

var xxx_messageInfo_GetRequest proto.InternalMessageInfo

func (m *GetRequest) GetKey() string {
	if m != nil {
		return m.Key
	}
	return ""
}

type GetResponse struct {
	Status               *Status  `protobuf:"bytes,1,opt,name=status,proto3" json:"status,omitempty"`
	Value                string   `protobuf:"bytes,2,opt,name=value,proto3" json:"value,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *GetResponse) Reset()         { *m = GetResponse{} }
func (m *GetResponse) String() string { return proto.CompactTextString(m) }
func (*GetResponse) ProtoMessage()    {}
func (*GetResponse) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{5}
}

func (m *GetResponse) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_GetResponse.Unmarshal(m, b)
}
func (m *GetResponse) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_GetResponse.Marshal(b, m, deterministic)
}
func (m *GetResponse) XXX_Merge(src proto.Message) {
	xxx_messageInfo_GetResponse.Merge(m, src)
}
func (m *GetResponse) XXX_Size() int {
	return xxx_messageInfo_GetResponse.Size(m)
}
func (m *GetResponse) XXX_DiscardUnknown() {
	xxx_messageInfo_GetResponse.DiscardUnknown(m)
}

var xxx_messageInfo_GetResponse proto.InternalMessageInfo

func (m *GetResponse) GetStatus() *Status {
	if m != nil {
		return m.Status
	}
	return nil
}

func (m *GetResponse) GetValue() string {
	if m != nil {
		return m.Value
	}
	return ""
}

type SetRequest struct {
	Key                  string   `protobuf:"bytes,1,opt,name=key,proto3" json:"key,omitempty"`
	Value                string   `protobuf:"bytes,2,opt,name=value,proto3" json:"value,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *SetRequest) Reset()         { *m = SetRequest{} }
func (m *SetRequest) String() string { return proto.CompactTextString(m) }
func (*SetRequest) ProtoMessage()    {}
func (*SetRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{6}
}

func (m *SetRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_SetRequest.Unmarshal(m, b)
}
func (m *SetRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_SetRequest.Marshal(b, m, deterministic)
}
func (m *SetRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_SetRequest.Merge(m, src)
}
func (m *SetRequest) XXX_Size() int {
	return xxx_messageInfo_SetRequest.Size(m)
}
func (m *SetRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_SetRequest.DiscardUnknown(m)
}

var xxx_messageInfo_SetRequest proto.InternalMessageInfo

func (m *SetRequest) GetKey() string {
	if m != nil {
		return m.Key
	}
	return ""
}

func (m *SetRequest) GetValue() string {
	if m != nil {
		return m.Value
	}
	return ""
}

type RemoveRequest struct {
	Key                  string   `protobuf:"bytes,1,opt,name=key,proto3" json:"key,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *RemoveRequest) Reset()         { *m = RemoveRequest{} }
func (m *RemoveRequest) String() string { return proto.CompactTextString(m) }
func (*RemoveRequest) ProtoMessage()    {}
func (*RemoveRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{7}
}

func (m *RemoveRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_RemoveRequest.Unmarshal(m, b)
}
func (m *RemoveRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_RemoveRequest.Marshal(b, m, deterministic)
}
func (m *RemoveRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_RemoveRequest.Merge(m, src)
}
func (m *RemoveRequest) XXX_Size() int {
	return xxx_messageInfo_RemoveRequest.Size(m)
}
func (m *RemoveRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_RemoveRequest.DiscardUnknown(m)
}

var xxx_messageInfo_RemoveRequest proto.InternalMessageInfo

func (m *RemoveRequest) GetKey() string {
	if m != nil {
		return m.Key
	}
	return ""
}

type RemoveResponse struct {
	Status               *Status  `protobuf:"bytes,1,opt,name=status,proto3" json:"status,omitempty"`
	Check                bool     `protobuf:"varint,2,opt,name=check,proto3" json:"check,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *RemoveResponse) Reset()         { *m = RemoveResponse{} }
func (m *RemoveResponse) String() string { return proto.CompactTextString(m) }
func (*RemoveResponse) ProtoMessage()    {}
func (*RemoveResponse) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{8}
}

func (m *RemoveResponse) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_RemoveResponse.Unmarshal(m, b)
}
func (m *RemoveResponse) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_RemoveResponse.Marshal(b, m, deterministic)
}
func (m *RemoveResponse) XXX_Merge(src proto.Message) {
	xxx_messageInfo_RemoveResponse.Merge(m, src)
}
func (m *RemoveResponse) XXX_Size() int {
	return xxx_messageInfo_RemoveResponse.Size(m)
}
func (m *RemoveResponse) XXX_DiscardUnknown() {
	xxx_messageInfo_RemoveResponse.DiscardUnknown(m)
}

var xxx_messageInfo_RemoveResponse proto.InternalMessageInfo

func (m *RemoveResponse) GetStatus() *Status {
	if m != nil {
		return m.Status
	}
	return nil
}

func (m *RemoveResponse) GetCheck() bool {
	if m != nil {
		return m.Check
	}
	return false
}

type ExistRequest struct {
	Key                  string   `protobuf:"bytes,1,opt,name=key,proto3" json:"key,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *ExistRequest) Reset()         { *m = ExistRequest{} }
func (m *ExistRequest) String() string { return proto.CompactTextString(m) }
func (*ExistRequest) ProtoMessage()    {}
func (*ExistRequest) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{9}
}

func (m *ExistRequest) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_ExistRequest.Unmarshal(m, b)
}
func (m *ExistRequest) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_ExistRequest.Marshal(b, m, deterministic)
}
func (m *ExistRequest) XXX_Merge(src proto.Message) {
	xxx_messageInfo_ExistRequest.Merge(m, src)
}
func (m *ExistRequest) XXX_Size() int {
	return xxx_messageInfo_ExistRequest.Size(m)
}
func (m *ExistRequest) XXX_DiscardUnknown() {
	xxx_messageInfo_ExistRequest.DiscardUnknown(m)
}

var xxx_messageInfo_ExistRequest proto.InternalMessageInfo

func (m *ExistRequest) GetKey() string {
	if m != nil {
		return m.Key
	}
	return ""
}

type ExistResponse struct {
	Status               *Status  `protobuf:"bytes,1,opt,name=status,proto3" json:"status,omitempty"`
	Check                bool     `protobuf:"varint,2,opt,name=check,proto3" json:"check,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *ExistResponse) Reset()         { *m = ExistResponse{} }
func (m *ExistResponse) String() string { return proto.CompactTextString(m) }
func (*ExistResponse) ProtoMessage()    {}
func (*ExistResponse) Descriptor() ([]byte, []int) {
	return fileDescriptor_12a4512b96b720f2, []int{10}
}

func (m *ExistResponse) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_ExistResponse.Unmarshal(m, b)
}
func (m *ExistResponse) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_ExistResponse.Marshal(b, m, deterministic)
}
func (m *ExistResponse) XXX_Merge(src proto.Message) {
	xxx_messageInfo_ExistResponse.Merge(m, src)
}
func (m *ExistResponse) XXX_Size() int {
	return xxx_messageInfo_ExistResponse.Size(m)
}
func (m *ExistResponse) XXX_DiscardUnknown() {
	xxx_messageInfo_ExistResponse.DiscardUnknown(m)
}

var xxx_messageInfo_ExistResponse proto.InternalMessageInfo

func (m *ExistResponse) GetStatus() *Status {
	if m != nil {
		return m.Status
	}
	return nil
}

func (m *ExistResponse) GetCheck() bool {
	if m != nil {
		return m.Check
	}
	return false
}

func init() {
	proto.RegisterEnum("service.StoreType", StoreType_name, StoreType_value)
	proto.RegisterType((*Status)(nil), "service.Status")
	proto.RegisterType((*MessageResponse)(nil), "service.MessageResponse")
	proto.RegisterType((*ConnectionRequest)(nil), "service.ConnectionRequest")
	proto.RegisterType((*DisconnectRequest)(nil), "service.DisconnectRequest")
	proto.RegisterType((*GetRequest)(nil), "service.GetRequest")
	proto.RegisterType((*GetResponse)(nil), "service.GetResponse")
	proto.RegisterType((*SetRequest)(nil), "service.SetRequest")
	proto.RegisterType((*RemoveRequest)(nil), "service.RemoveRequest")
	proto.RegisterType((*RemoveResponse)(nil), "service.RemoveResponse")
	proto.RegisterType((*ExistRequest)(nil), "service.ExistRequest")
	proto.RegisterType((*ExistResponse)(nil), "service.ExistResponse")
}

func init() { proto.RegisterFile("api/proto/service.proto", fileDescriptor_12a4512b96b720f2) }

var fileDescriptor_12a4512b96b720f2 = []byte{
	// 420 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xac, 0x54, 0x4d, 0x6f, 0xd3, 0x40,
	0x10, 0xad, 0x71, 0xe3, 0xe2, 0x29, 0x6e, 0x9b, 0x69, 0x68, 0x22, 0x1f, 0x50, 0xd8, 0x03, 0x44,
	0x48, 0x24, 0x92, 0x41, 0x08, 0x91, 0x13, 0x21, 0x28, 0x07, 0x3e, 0xb5, 0x8e, 0xb8, 0x1b, 0x33,
	0x02, 0x2b, 0xc1, 0x6b, 0xbc, 0x76, 0x84, 0xaf, 0xfc, 0x72, 0x94, 0xb5, 0xbd, 0xf9, 0x52, 0x2c,
	0x81, 0x7a, 0xdb, 0x37, 0x33, 0x6f, 0xde, 0xec, 0xce, 0xd3, 0x42, 0x37, 0x48, 0xa2, 0x51, 0x92,
	0x8a, 0x4c, 0x8c, 0x24, 0xa5, 0xab, 0x28, 0xa4, 0xa1, 0x42, 0x78, 0x56, 0x41, 0xe6, 0x81, 0xe5,
	0x67, 0x41, 0x96, 0x4b, 0x44, 0x38, 0x0d, 0xc5, 0x37, 0xea, 0x19, 0x7d, 0x63, 0xd0, 0xe2, 0xea,
	0x8c, 0x1d, 0x68, 0x51, 0x9a, 0x8a, 0xb4, 0x77, 0xa7, 0x6f, 0x0c, 0x6c, 0x5e, 0x02, 0xf6, 0x0a,
	0x2e, 0x3f, 0x90, 0x94, 0xc1, 0x77, 0xe2, 0x24, 0x13, 0x11, 0x4b, 0xc2, 0xc7, 0x60, 0x49, 0xd5,
	0x46, 0xd1, 0xcf, 0xbd, 0xcb, 0x61, 0xad, 0x57, 0x76, 0xe7, 0x55, 0x9a, 0x8d, 0xa1, 0xfd, 0x46,
	0xc4, 0x31, 0x85, 0x59, 0x24, 0x62, 0x4e, 0xbf, 0x72, 0x92, 0x19, 0x3e, 0x82, 0xd3, 0xac, 0x48,
	0x4a, 0xe9, 0x0b, 0x0f, 0xb7, 0xb8, 0x22, 0xa5, 0x79, 0x91, 0x10, 0x57, 0x79, 0x76, 0x0d, 0xed,
	0x69, 0x24, 0xc3, 0x92, 0x5f, 0x91, 0xd9, 0x03, 0x80, 0x19, 0xd5, 0x08, 0xaf, 0xc0, 0x5c, 0x50,
	0xa1, 0x3a, 0xd9, 0x7c, 0x7d, 0x64, 0xef, 0xe1, 0x5c, 0xe5, 0xff, 0x71, 0xd2, 0xf5, 0xdd, 0x57,
	0xc1, 0x32, 0xa7, 0xfa, 0xee, 0x0a, 0xb0, 0xe7, 0x00, 0x7e, 0x83, 0xda, 0x11, 0xd6, 0x43, 0x70,
	0x38, 0xfd, 0x14, 0x2b, 0x3a, 0x3e, 0xe6, 0x27, 0xb8, 0xa8, 0x4b, 0xfe, 0x63, 0xd2, 0xf0, 0x07,
	0x85, 0x0b, 0xa5, 0x79, 0x97, 0x97, 0x80, 0xf5, 0xe1, 0xde, 0xdb, 0xdf, 0x91, 0x6c, 0x78, 0x99,
	0x8f, 0xe0, 0x54, 0x15, 0xb7, 0xa2, 0xf8, 0xe4, 0x29, 0xd8, 0x7a, 0x63, 0xe8, 0x80, 0x3d, 0x99,
	0xa7, 0x44, 0xd3, 0x48, 0x2e, 0xae, 0x4e, 0xb0, 0x0d, 0xce, 0xe4, 0xf3, 0x32, 0x97, 0x3a, 0x64,
	0x78, 0x7f, 0x4c, 0xe8, 0xbc, 0xa3, 0xe2, 0xcb, 0xfa, 0x85, 0x14, 0xcf, 0x2f, 0xc5, 0xf0, 0x35,
	0x9c, 0x55, 0x1e, 0x41, 0x57, 0x4f, 0x70, 0xe0, 0x1a, 0xb7, 0xa7, 0x73, 0x7b, 0x6e, 0x64, 0x27,
	0x38, 0x05, 0xd8, 0x38, 0x65, 0xab, 0xcb, 0x81, 0x7d, 0x1a, 0xbb, 0x78, 0x60, 0xce, 0x28, 0xc3,
	0x6b, 0x5d, 0xb2, 0x31, 0x9a, 0xdb, 0xd9, 0x0d, 0x6a, 0xce, 0x0b, 0x30, 0xfd, 0x1d, 0xce, 0xc6,
	0x2e, 0x8d, 0x5a, 0x63, 0xb0, 0xca, 0xfd, 0xe3, 0x8d, 0xae, 0xda, 0xf1, 0x8c, 0xdb, 0x3d, 0x88,
	0x6b, 0xf2, 0x4b, 0x68, 0xa9, 0x4d, 0xe2, 0x7d, 0x5d, 0xb3, 0xbd, 0x7b, 0xf7, 0x66, 0x3f, 0x5c,
	0x33, 0xbf, 0x5a, 0xea, 0x3f, 0x78, 0xf6, 0x37, 0x00, 0x00, 0xff, 0xff, 0xd5, 0x72, 0xdc, 0x71,
	0x2a, 0x04, 0x00, 0x00,
}

// Reference imports to suppress errors if they are not otherwise used.
var _ context.Context
var _ grpc.ClientConn

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
const _ = grpc.SupportPackageIsVersion4

// KeyValueStoreServiceClient is the client API for KeyValueStoreService service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://godoc.org/google.golang.org/grpc#ClientConn.NewStream.
type KeyValueStoreServiceClient interface {
	Connect(ctx context.Context, in *ConnectionRequest, opts ...grpc.CallOption) (*MessageResponse, error)
	Disconnect(ctx context.Context, in *DisconnectRequest, opts ...grpc.CallOption) (*MessageResponse, error)
	Get(ctx context.Context, in *GetRequest, opts ...grpc.CallOption) (*GetResponse, error)
	Set(ctx context.Context, in *SetRequest, opts ...grpc.CallOption) (*MessageResponse, error)
	Remove(ctx context.Context, in *RemoveRequest, opts ...grpc.CallOption) (*RemoveResponse, error)
	Exist(ctx context.Context, in *ExistRequest, opts ...grpc.CallOption) (*ExistResponse, error)
}

type keyValueStoreServiceClient struct {
	cc *grpc.ClientConn
}

func NewKeyValueStoreServiceClient(cc *grpc.ClientConn) KeyValueStoreServiceClient {
	return &keyValueStoreServiceClient{cc}
}

func (c *keyValueStoreServiceClient) Connect(ctx context.Context, in *ConnectionRequest, opts ...grpc.CallOption) (*MessageResponse, error) {
	out := new(MessageResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Connect", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *keyValueStoreServiceClient) Disconnect(ctx context.Context, in *DisconnectRequest, opts ...grpc.CallOption) (*MessageResponse, error) {
	out := new(MessageResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Disconnect", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *keyValueStoreServiceClient) Get(ctx context.Context, in *GetRequest, opts ...grpc.CallOption) (*GetResponse, error) {
	out := new(GetResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Get", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *keyValueStoreServiceClient) Set(ctx context.Context, in *SetRequest, opts ...grpc.CallOption) (*MessageResponse, error) {
	out := new(MessageResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Set", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *keyValueStoreServiceClient) Remove(ctx context.Context, in *RemoveRequest, opts ...grpc.CallOption) (*RemoveResponse, error) {
	out := new(RemoveResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Remove", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *keyValueStoreServiceClient) Exist(ctx context.Context, in *ExistRequest, opts ...grpc.CallOption) (*ExistResponse, error) {
	out := new(ExistResponse)
	err := c.cc.Invoke(ctx, "/service.KeyValueStoreService/Exist", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// KeyValueStoreServiceServer is the server API for KeyValueStoreService service.
type KeyValueStoreServiceServer interface {
	Connect(context.Context, *ConnectionRequest) (*MessageResponse, error)
	Disconnect(context.Context, *DisconnectRequest) (*MessageResponse, error)
	Get(context.Context, *GetRequest) (*GetResponse, error)
	Set(context.Context, *SetRequest) (*MessageResponse, error)
	Remove(context.Context, *RemoveRequest) (*RemoveResponse, error)
	Exist(context.Context, *ExistRequest) (*ExistResponse, error)
}

// UnimplementedKeyValueStoreServiceServer can be embedded to have forward compatible implementations.
type UnimplementedKeyValueStoreServiceServer struct {
}

func (*UnimplementedKeyValueStoreServiceServer) Connect(ctx context.Context, req *ConnectionRequest) (*MessageResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Connect not implemented")
}
func (*UnimplementedKeyValueStoreServiceServer) Disconnect(ctx context.Context, req *DisconnectRequest) (*MessageResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Disconnect not implemented")
}
func (*UnimplementedKeyValueStoreServiceServer) Get(ctx context.Context, req *GetRequest) (*GetResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Get not implemented")
}
func (*UnimplementedKeyValueStoreServiceServer) Set(ctx context.Context, req *SetRequest) (*MessageResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Set not implemented")
}
func (*UnimplementedKeyValueStoreServiceServer) Remove(ctx context.Context, req *RemoveRequest) (*RemoveResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Remove not implemented")
}
func (*UnimplementedKeyValueStoreServiceServer) Exist(ctx context.Context, req *ExistRequest) (*ExistResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Exist not implemented")
}

func RegisterKeyValueStoreServiceServer(s *grpc.Server, srv KeyValueStoreServiceServer) {
	s.RegisterService(&_KeyValueStoreService_serviceDesc, srv)
}

func _KeyValueStoreService_Connect_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(ConnectionRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Connect(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Connect",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Connect(ctx, req.(*ConnectionRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _KeyValueStoreService_Disconnect_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(DisconnectRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Disconnect(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Disconnect",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Disconnect(ctx, req.(*DisconnectRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _KeyValueStoreService_Get_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(GetRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Get(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Get",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Get(ctx, req.(*GetRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _KeyValueStoreService_Set_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(SetRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Set(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Set",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Set(ctx, req.(*SetRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _KeyValueStoreService_Remove_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(RemoveRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Remove(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Remove",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Remove(ctx, req.(*RemoveRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _KeyValueStoreService_Exist_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(ExistRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(KeyValueStoreServiceServer).Exist(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/service.KeyValueStoreService/Exist",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(KeyValueStoreServiceServer).Exist(ctx, req.(*ExistRequest))
	}
	return interceptor(ctx, in, info, handler)
}

var _KeyValueStoreService_serviceDesc = grpc.ServiceDesc{
	ServiceName: "service.KeyValueStoreService",
	HandlerType: (*KeyValueStoreServiceServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "Connect",
			Handler:    _KeyValueStoreService_Connect_Handler,
		},
		{
			MethodName: "Disconnect",
			Handler:    _KeyValueStoreService_Disconnect_Handler,
		},
		{
			MethodName: "Get",
			Handler:    _KeyValueStoreService_Get_Handler,
		},
		{
			MethodName: "Set",
			Handler:    _KeyValueStoreService_Set_Handler,
		},
		{
			MethodName: "Remove",
			Handler:    _KeyValueStoreService_Remove_Handler,
		},
		{
			MethodName: "Exist",
			Handler:    _KeyValueStoreService_Exist_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "api/proto/service.proto",
}
