#pragma once

#include <cassert>
#include <cstddef>

#include "shaderbinary.h"

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

enum PsslType : uint8_t {
	PSSL_TYPE_FLOAT1 = 0,
	PSSL_TYPE_FLOAT2 = 1,
	PSSL_TYPE_FLOAT3 = 2,
	PSSL_TYPE_FLOAT4 = 3,
	PSSL_TYPE_HALF1 = 4,
	PSSL_TYPE_HALF2 = 5,
	PSSL_TYPE_HALF3 = 6,
	PSSL_TYPE_HALF4 = 7,
	PSSL_TYPE_INT1 = 8,
	PSSL_TYPE_INT2 = 9,
	PSSL_TYPE_INT3 = 10,
	PSSL_TYPE_INT4 = 11,
	PSSL_TYPE_UINT1 = 12,
	PSSL_TYPE_UINT2 = 13,
	PSSL_TYPE_UINT3 = 14,
	PSSL_TYPE_UINT4 = 15,
	PSSL_TYPE_DOUBLE1 = 16,
	PSSL_TYPE_DOUBLE2 = 17,
	PSSL_TYPE_DOUBLE3 = 18,
	PSSL_TYPE_DOUBLE4 = 19,
	PSSL_TYPE_FLOAT1X1 = 20,
	PSSL_TYPE_FLOAT2X1 = 21,
	PSSL_TYPE_FLOAT3X1 = 22,
	PSSL_TYPE_FLOAT4X1 = 23,
	PSSL_TYPE_FLOAT1X2 = 24,
	PSSL_TYPE_FLOAT2X2 = 25,
	PSSL_TYPE_FLOAT3X2 = 26,
	PSSL_TYPE_FLOAT4X2 = 27,
	PSSL_TYPE_FLOAT1X3 = 28,
	PSSL_TYPE_FLOAT2X3 = 29,
	PSSL_TYPE_FLOAT3X3 = 30,
	PSSL_TYPE_FLOAT4X3 = 31,
	PSSL_TYPE_FLOAT1X4 = 32,
	PSSL_TYPE_FLOAT2X4 = 33,
	PSSL_TYPE_FLOAT3X4 = 34,
	PSSL_TYPE_FLOAT4X4 = 35,
	PSSL_TYPE_HALF1X1 = 36,
	PSSL_TYPE_HALF2X1 = 37,
	PSSL_TYPE_HALF3X1 = 38,
	PSSL_TYPE_HALF4X1 = 39,
	PSSL_TYPE_HALF1X2 = 40,
	PSSL_TYPE_HALF2X2 = 41,
	PSSL_TYPE_HALF3X2 = 42,
	PSSL_TYPE_HALF4X2 = 43,
	PSSL_TYPE_HALF1X3 = 44,
	PSSL_TYPE_HALF2X3 = 45,
	PSSL_TYPE_HALF3X3 = 46,
	PSSL_TYPE_HALF4X3 = 47,
	PSSL_TYPE_HALF1X4 = 48,
	PSSL_TYPE_HALF2X4 = 49,
	PSSL_TYPE_HALF3X4 = 50,
	PSSL_TYPE_HALF4X4 = 51,
	PSSL_TYPE_INT1X1 = 52,
	PSSL_TYPE_INT2X1 = 53,
	PSSL_TYPE_INT3X1 = 54,
	PSSL_TYPE_INT4X1 = 55,
	PSSL_TYPE_INT1X2 = 56,
	PSSL_TYPE_INT2X2 = 57,
	PSSL_TYPE_INT3X2 = 58,
	PSSL_TYPE_INT4X2 = 59,
	PSSL_TYPE_INT1X3 = 60,
	PSSL_TYPE_INT2X3 = 61,
	PSSL_TYPE_INT3X3 = 62,
	PSSL_TYPE_INT4X3 = 63,
	PSSL_TYPE_INT1X4 = 64,
	PSSL_TYPE_INT2X4 = 65,
	PSSL_TYPE_INT3X4 = 66,
	PSSL_TYPE_INT4X4 = 67,
	PSSL_TYPE_UINT1X1 = 68,
	PSSL_TYPE_UINT2X1 = 69,
	PSSL_TYPE_UINT3X1 = 70,
	PSSL_TYPE_UINT4X1 = 71,
	PSSL_TYPE_UINT1X2 = 72,
	PSSL_TYPE_UINT2X2 = 73,
	PSSL_TYPE_UINT3X2 = 74,
	PSSL_TYPE_UINT4X2 = 75,
	PSSL_TYPE_UINT1X3 = 76,
	PSSL_TYPE_UINT2X3 = 77,
	PSSL_TYPE_UINT3X3 = 78,
	PSSL_TYPE_UINT4X3 = 79,
	PSSL_TYPE_UINT1X4 = 80,
	PSSL_TYPE_UINT2X4 = 81,
	PSSL_TYPE_UINT3X4 = 82,
	PSSL_TYPE_UINT4X4 = 83,
	PSSL_TYPE_DOUBLE1X1 = 84,
	PSSL_TYPE_DOUBLE2X1 = 85,
	PSSL_TYPE_DOUBLE3X1 = 86,
	PSSL_TYPE_DOUBLE4X1 = 87,
	PSSL_TYPE_DOUBLE1X2 = 88,
	PSSL_TYPE_DOUBLE2X2 = 89,
	PSSL_TYPE_DOUBLE3X2 = 90,
	PSSL_TYPE_DOUBLE4X2 = 91,
	PSSL_TYPE_DOUBLE1X3 = 92,
	PSSL_TYPE_DOUBLE2X3 = 93,
	PSSL_TYPE_DOUBLE3X3 = 94,
	PSSL_TYPE_DOUBLE4X3 = 95,
	PSSL_TYPE_DOUBLE1X4 = 96,
	PSSL_TYPE_DOUBLE2X4 = 97,
	PSSL_TYPE_DOUBLE3X4 = 98,
	PSSL_TYPE_DOUBLE4X4 = 99,
	PSSL_TYPE_POINT = 100,
	PSSL_TYPE_LINE = 101,
	PSSL_TYPE_TRIANGLE = 102,
	PSSL_TYPE_ADJACENTLINE = 103,
	PSSL_TYPE_ADJACENTTRIANGLE = 104,
	PSSL_TYPE_PATCH = 105,
	PSSL_TYPE_STRUCTURE = 106,
	PSSL_TYPE_LONG1 = 107,
	PSSL_TYPE_LONG2 = 108,
	PSSL_TYPE_LONG3 = 109,
	PSSL_TYPE_LONG4 = 110,
	PSSL_TYPE_ULONG1 = 111,
	PSSL_TYPE_ULONG2 = 112,
	PSSL_TYPE_ULONG3 = 113,
	PSSL_TYPE_ULONG4 = 114,
	PSSL_TYPE_LONG1X1 = 115,
	PSSL_TYPE_LONG2X1 = 116,
	PSSL_TYPE_LONG3X1 = 117,
	PSSL_TYPE_LONG4X1 = 118,
	PSSL_TYPE_LONG1X2 = 119,
	PSSL_TYPE_LONG2X2 = 120,
	PSSL_TYPE_LONG3X2 = 121,
	PSSL_TYPE_LONG4X2 = 122,
	PSSL_TYPE_LONG1X3 = 123,
	PSSL_TYPE_LONG2X3 = 124,
	PSSL_TYPE_LONG3X3 = 125,
	PSSL_TYPE_LONG4X3 = 126,
	PSSL_TYPE_LONG1X4 = 127,
	PSSL_TYPE_LONG2X4 = 128,
	PSSL_TYPE_LONG3X4 = 129,
	PSSL_TYPE_LONG4X4 = 130,
	PSSL_TYPE_ULONG1X1 = 131,
	PSSL_TYPE_ULONG2X1 = 132,
	PSSL_TYPE_ULONG3X1 = 133,
	PSSL_TYPE_ULONG4X1 = 134,
	PSSL_TYPE_ULONG1X2 = 135,
	PSSL_TYPE_ULONG2X2 = 136,
	PSSL_TYPE_ULONG3X2 = 137,
	PSSL_TYPE_ULONG4X2 = 138,
	PSSL_TYPE_ULONG1X3 = 139,
	PSSL_TYPE_ULONG2X3 = 140,
	PSSL_TYPE_ULONG3X3 = 141,
	PSSL_TYPE_ULONG4X3 = 142,
	PSSL_TYPE_ULONG1X4 = 143,
	PSSL_TYPE_ULONG2X4 = 144,
	PSSL_TYPE_ULONG3X4 = 145,
	PSSL_TYPE_ULONG4X4 = 146,
	PSSL_TYPE_BOOL1 = 147,
	PSSL_TYPE_BOOL2 = 148,
	PSSL_TYPE_BOOL3 = 149,
	PSSL_TYPE_BOOL4 = 150,
	PSSL_TYPE_TEXTURE = 151,
	PSSL_TYPE_SAMPLERSTATE = 152,
	PSSL_TYPE_BUFFER = 153,
	PSSL_TYPE_USHORT1 = 154,
	PSSL_TYPE_USHORT2 = 155,
	PSSL_TYPE_USHORT3 = 156,
	PSSL_TYPE_USHORT4 = 157,
	PSSL_TYPE_UCHAR1 = 158,
	PSSL_TYPE_UCHAR2 = 159,
	PSSL_TYPE_UCHAR3 = 160,
	PSSL_TYPE_UCHAR4 = 161,
	PSSL_TYPE_SHORT1 = 162,
	PSSL_TYPE_SHORT2 = 163,
	PSSL_TYPE_SHORT3 = 164,
	PSSL_TYPE_SHORT4 = 165,
	PSSL_TYPE_CHAR1 = 166,
	PSSL_TYPE_CHAR2 = 167,
	PSSL_TYPE_CHAR3 = 168,
	PSSL_TYPE_CHAR4 = 169,
	PSSL_TYPE_VOID = 170,
	PSSL_TYPE_TEXTURER128 = 171,
};
const char* psslStrType(PsslType type);

enum PsslSemantic : uint8_t {
	PSSL_SEMANTIC_POSITION = 0,
	PSSL_SEMANTIC_NORMAL = 1,
	PSSL_SEMANTIC_COLOR = 2,
	PSSL_SEMANTIC_BINORMAL = 3,
	PSSL_SEMANTIC_TANGENT = 4,
	PSSL_SEMANTIC_TEXCOORD0 = 5,
	PSSL_SEMANTIC_TEXCOORD1 = 6,
	PSSL_SEMANTIC_TEXCOORD2 = 7,
	PSSL_SEMANTIC_TEXCOORD3 = 8,
	PSSL_SEMANTIC_TEXCOORD4 = 9,
	PSSL_SEMANTIC_TEXCOORD5 = 10,
	PSSL_SEMANTIC_TEXCOORD6 = 11,
	PSSL_SEMANTIC_TEXCOORD7 = 12,
	PSSL_SEMANTIC_TEXCOORD8 = 13,
	PSSL_SEMANTIC_TEXCOORD9 = 14,
	PSSL_SEMANTIC_TEXCOORDEND = 15,
	PSSL_SEMANTIC_IMPLICIT = 16,
	PSSL_SEMANTIC_NONREFERENCABLE = 17,
	PSSL_SEMANTIC_CLIP = 18,
	PSSL_SEMANTIC_FOG = 19,
	PSSL_SEMANTIC_SPOINTSIZE = 20,
	PSSL_SEMANTIC_FRAGCOORD = 21,
	PSSL_SEMANTIC_TARGET0 = 22,
	PSSL_SEMANTIC_TARGET1 = 23,
	PSSL_SEMANTIC_TARGET2 = 24,
	PSSL_SEMANTIC_TARGET3 = 25,
	PSSL_SEMANTIC_TARGET4 = 26,
	PSSL_SEMANTIC_TARGET5 = 27,
	PSSL_SEMANTIC_TARGET6 = 28,
	PSSL_SEMANTIC_TARGET7 = 29,
	PSSL_SEMANTIC_TARGET8 = 30,
	PSSL_SEMANTIC_TARGET9 = 31,
	PSSL_SEMANTIC_TARGET10 = 32,
	PSSL_SEMANTIC_TARGET11 = 33,
	PSSL_SEMANTIC_DEPTH = 34,
	PSSL_SEMANTIC_LASTCG = 35,
	PSSL_SEMANTIC_USERDEFINED = 36,
	PSSL_SEMANTIC_SCLIPDISTANCE = 37,
	PSSL_SEMANTIC_SCULLDISTANCE = 38,
	PSSL_SEMANTIC_SCOVERAGE = 39,
	PSSL_SEMANTIC_SDEPTHOUTPUT = 40,
	PSSL_SEMANTIC_SDISPATCHTHREADID = 41,
	PSSL_SEMANTIC_SDOMAINLOCATION = 42,
	PSSL_SEMANTIC_SGROUPID = 43,
	PSSL_SEMANTIC_SGROUPINDEX = 44,
	PSSL_SEMANTIC_SGROUPTHREADID = 45,
	PSSL_SEMANTIC_SPOSITION = 46,
	PSSL_SEMANTIC_SVERTEXID = 47,
	PSSL_SEMANTIC_SINSTANCEID = 48,
	PSSL_SEMANTIC_SSAMPLEINDEX = 49,
	PSSL_SEMANTIC_SPRIMITIVEID = 50,
	PSSL_SEMANTIC_SGSINSTANCEID = 51,
	PSSL_SEMANTIC_SOUTPUTCONTROLPOINTID = 52,
	PSSL_SEMANTIC_SFRONTFACE = 53,
	PSSL_SEMANTIC_SRENDERTARGETINDEX = 54,
	PSSL_SEMANTIC_SVIEWPORTINDEX = 55,
	PSSL_SEMANTIC_STARGETOUTPUT = 56,
	PSSL_SEMANTIC_SEDGETESSFACTOR = 57,
	PSSL_SEMANTIC_SINSIDETESSFACTOR = 58,
	PSSL_SEMANTIC_SPOINTCOORD = 59,
	PSSL_SEMANTIC_SDEPTHGEOUTPUT = 60,
	PSSL_SEMANTIC_SDEPTHLEOUTPUT = 61,
	PSSL_SEMANTIC_SVERTEXOFFSETID = 62,
	PSSL_SEMANTIC_SINSTANCEOFFSETID = 63,
	PSSL_SEMANTIC_SSTENCILVALUE = 64,
	PSSL_SEMANTIC_SSTENCILOP = 65,
};
const char* psslStrSemantic(PsslSemantic semantic);

enum PsslFragmentInterpType : uint8_t {
	PSSL_FRAG_INTERP_SAMPLE_NONE = 0,
	PSSL_FRAG_INTERP_SAMPLE_LINEAR = 1,
	PSSL_FRAG_INTERP_SAMPLE_POINT = 2,
};
const char* psslStrFragmentInterpType(PsslFragmentInterpType type);

enum PsslBufferType : uint8_t {
	PSSL_BUFFER_DATABUFFER = 0,
	PSSL_BUFFER_TEXTURE1D = 1,
	PSSL_BUFFER_TEXTURE2D = 2,
	PSSL_BUFFER_TEXTURE3D = 3,
	PSSL_BUFFER_TEXTURECUBE = 4,
	PSSL_BUFFER_TEXTURE1DARRAY = 5,
	PSSL_BUFFER_TEXTURE2DARRAY = 6,
	PSSL_BUFFER_TEXTURECUBEARRAY = 7,
	PSSL_BUFFER_MSTEXTURE2D = 8,
	PSSL_BUFFER_MSTEXTURE2DARRAY = 9,
	PSSL_BUFFER_REGULARBUFFER = 10,
	PSSL_BUFFER_BYTEBUFFER = 11,
	PSSL_BUFFER_RWDATABUFFER = 12,
	PSSL_BUFFER_RWTEXTURE1D = 13,
	PSSL_BUFFER_RWTEXTURE2D = 14,
	PSSL_BUFFER_RWTEXTURE3D = 15,
	PSSL_BUFFER_RWTEXTURE1DARRAY = 16,
	PSSL_BUFFER_RWTEXTURE2DARRAY = 17,
	PSSL_BUFFER_RWREGULARBUFFER = 18,
	PSSL_BUFFER_RWBYTEBUFFER = 19,
	PSSL_BUFFER_APPENDBUFFER = 20,
	PSSL_BUFFER_CONSUMEBUFFER = 21,
	PSSL_BUFFER_CONSTANTBUFFER = 22,
	PSSL_BUFFER_TEXTUREBUFFER = 23,
	PSSL_BUFFER_POINTBUFFER = 24,
	PSSL_BUFFER_LINEBUFFER = 25,
	PSSL_BUFFER_TRIANGLEBUFFER = 26,
	PSSL_BUFFER_SRTBUFFER = 27,
	PSSL_BUFFER_DISPATCHDRAWBUFFER = 28,
	PSSL_BUFFER_RWTEXTURECUBE = 29,
	PSSL_BUFFER_RWTEXTURECUBEARRAY = 30,
	PSSL_BUFFER_RWMSTEXTURE2D = 31,
	PSSL_BUFFER_RWMSTEXTURE2DARRAY = 32,
	PSSL_BUFFER_TEXTURE1DR128 = 33,
	PSSL_BUFFER_TEXTURE2DR128 = 34,
	PSSL_BUFFER_MSTEXTURE2DR128 = 35,
	PSSL_BUFFER_RWTEXTURE1DR128 = 36,
	PSSL_BUFFER_RWTEXTURE2DR128 = 37,
	PSSL_BUFFER_RWMSTEXTURE2DR128 = 38,
};
const char* psslStrBufferType(PsslBufferType type);

enum PsslInternalBufferType : uint8_t {
	PSSL_INTERNAL_BUFFER_UAV = 0,
	PSSL_INTERNAL_BUFFER_SRV = 1,
	PSSL_INTERNAL_BUFFER_LDS = 2,
	PSSL_INTERNAL_BUFFER_GDS = 3,
	PSSL_INTERNAL_BUFFER_CBUFFER = 4,
	PSSL_INTERNAL_BUFFER_TEXTURE_SAMPLER = 5,
	PSSL_INTERNAL_BUFFER_INTERNAL = 6,
};
const char* psslStrInternalBufferType(PsslInternalBufferType type);

enum PsslElementType : uint8_t {
	PSSL_ELEMENT_ELEMENT = 0,
	PSSL_ELEMENT_BUFFER = 1,
	PSSL_ELEMENT_SAMPLERSTATE = 2,
	PSSL_ELEMENT_CONSTANTBUFFER = 3,
};
const char* psslStrElementType(PsslElementType type);

enum PsslShaderType : uint8_t {
	PSSL_SHADER_VS = 0x0,
	PSSL_SHADER_FS = 0x1,
	PSSL_SHADER_CS = 0x2,
	PSSL_SHADER_GS = 0x3,
	PSSL_SHADER_HS = 0x4,
	PSSL_SHADER_DS = 0x5,
};
const char* psslStrShaderType(PsslShaderType type);

enum PsslCodeType : uint8_t {
	PSSL_CODE_IL = 0x0,
	PSSL_CODE_ISA = 0x1,
	PSSL_CODE_SCU = 0x2,
};
const char* psslStrCodeType(PsslCodeType type);

enum PsslCompilerType : uint8_t {
	PSSL_COMPILER_UNSPECIFIED = 0,
	PSSL_COMPILER_ORBIS_PSSLC = 1,
	PSSL_COMPILER_ORBIS_ESSLC = 2,
	PSSL_COMPILER_ORBIS_WAVE = 3,
	PSSL_COMPILER_ORBIS_CU_AS = 4,
};
const char* psslStrCompilerType(PsslCompilerType type);

enum PsslVertexVariant : uint8_t {
	PSSL_VSVAR_VERTEX = 0x0,
	PSSL_VSVAR_EXPORT = 0x1,
	PSSL_VSVAR_LOCAL = 0x2,
	PSSL_VSVAR_DISPATCHDRAW = 0x3,
	PSSL_VSVAR_EXPORTONCHIP = 0x4,
};
const char* psslStrVertexVariant(PsslVertexVariant variant);

enum PsslDomainVariant : uint8_t {
	PSSL_DSVAR_VERTEX = 0x0,
	PSSL_DSVAR_EXPORT = 0x1,
	PSSL_DSVAR_VERTEX_HS_ONBUFFER = 0x2,
	PSSL_DSVAR_EXPORT_HS_ONBUFFER = 0x3,
	PSSL_DSVAR_VERTEX_HS_DYNAMIC = 0x4,
	PSSL_DSVAR_EXPORT_HS_DYNAMIC = 0x5,
	PSSL_DSVAR_EXPORT_ONCHIP = 0x6,
};
const char* psslStrDomainVariant(PsslDomainVariant variant);

enum PsslGeometryVariant : uint8_t {
	PSSL_GSVAR_ONBUFFER = 0x0,
	PSSL_GSVAR_ONCHIP = 0x1,
};
const char* psslStrGeometryVariant(PsslGeometryVariant variant);

enum PsslHullVariant : uint8_t {
	PSSL_HSVAR_ONCHIP = 0x0,
	PSSL_HSVAR_ONBUFFER = 0x1,
	PSSL_HSVAR_DYNAMIC = 0x2,
};
const char* psslStrHullVariant(PsslHullVariant variant);

enum PsslGsIoType {
	PSSL_GSIO_TRI = 0x0,
	PSSL_GSIO_LINE = 0x1,
	PSSL_GSIO_POINT = 0x2,
	PSSL_GSIO_ADJTRI = 0x3,
	PSSL_GSIO_ADJLINE = 0x4,
};
const char* psslStrGsIoType(PsslGsIoType type);

enum PsslHsDsPatchType {
	PSSL_HSDSPATCH_ISOLINE = 0x0,
	PSSL_HSDSPATCH_TRI = 0x1,
	PSSL_HSDSPATCH_QUAD = 0x2,
};
const char* psslStrHsDsPatchType(PsslHsDsPatchType type);

enum PsslHsTopologyType {
	PSSL_HSTOPOLOGY_POINT = 0x0,
	PSSL_HSTOPOLOGY_LINE = 0x1,
	PSSL_HSTOPOLOGY_CWTRI = 0x2,
	PSSL_HSTOPOLOGY_CCWTRI = 0x3,
};
const char* psslStrHsTopologyType(PsslHsTopologyType type);

enum PsslHsPartitioningType {
	PSSL_HSPARTITIONING_INTEGER = 0x0,
	PSSL_HSPARTITIONING_POWEROF2 = 0x1,
	PSSL_HSPARTITIONING_ODDFACTORIAL = 0x2,
	PSSL_HSPARTITIONING_EVENFACTORIAL = 0x3,
};
const char* psslStrHsPartitioningType(PsslHsPartitioningType type);

struct PsslPipelineStage {
    PsslVertexVariant vertexvariant;	  // PsslVertexVariant
    PsslDomainVariant domainvariant;	  // PsslDomainVariant
    PsslGeometryVariant geometryvariant;  // PsslGeometryVariant
    PsslHullVariant hullvariant;	  // PsslHullVariant
};
static_assert(sizeof(PsslPipelineStage) == 0x4, "");

union PsslSystemAttributes {
	struct {
		uint16_t numthreads[3];
	} cs;
	struct {
		uint16_t instance;
		uint16_t maxvertexcount;
		uint8_t inputtype;   // PsslGsIoType
		uint8_t outputtype;  // PsslGsIoType
		uint8_t patchsize;
	} gs;
	struct {
		uint8_t patchtype;  // PsslHsDsPatchType
		uint8_t inputcontrolpoints;
	} ds;
	struct {
		uint8_t patchtype;  // PsslHsDsPatchType
		uint8_t inputcontrolpoints;
		uint8_t outputtopologytype;  // PsslHsTopologyType
		uint8_t partitioningtype;    // PsslHsPartitioningType

		uint8_t outputcontrolpoints;
		uint8_t patchsize;
		uint8_t _unused[2];

		float maxtessfactor;
	} hs;
};
static_assert(sizeof(PsslSystemAttributes) == 0xC, "");

struct PsslBinaryHeader {
	uint8_t vermajor;
	uint8_t verminor;
	uint16_t compiler_revision;

	uint32_t association_hash0;
	uint32_t association_hash1;

    PsslShaderType shadertype;  // PsslShaderType
    PsslCodeType codetype;    // PsslCodeType
	uint8_t uses_srt;

	// first 4 bits are a PsslCompilerType
	// last 4 bits are unused
	uint8_t compilertype;

	uint32_t codesize;

	PsslPipelineStage stageinfo;
	PsslSystemAttributes sysattributeinfo;
};
static_assert(sizeof(PsslBinaryHeader) == 0x24, "");

struct PsslBinaryParamInfo {
	uint32_t numbufferresources;
	uint32_t numconstants;
	uint32_t numvariables;
	uint32_t numsamplerresources;

	uint8_t numinputattributes;
	uint8_t numoutputattributes;
	uint8_t numstreamout;
	uint8_t _unused;

	uint32_t valuetablesize;
	uint32_t stringtablesize;
};
static_assert(sizeof(PsslBinaryParamInfo) == 0x1c, "");

struct PsslBinaryAttribute {
    PsslType type;	   // PsslType
    PsslSemantic semantic;  // PsslSemantic
	uint8_t semanticindex;
	uint8_t resourceindex;

    PsslFragmentInterpType interptype;  // PsslFragmentInterpType
	uint8_t streamnumber;
	uint16_t _unused;

	uint32_t nameoff;
	uint32_t semanticnameoff;
};
static_assert(sizeof(PsslBinaryAttribute) == 0x10, "");

struct PsslBinaryBuffer {
    // References a resource by its index.
	// SRV is for textures (t0, t1, ...), UAV is for uniforms (u0, u1, ...),
	uint32_t resourceidx;
	uint32_t stridesize;
    PsslBufferType buffertype;    // PsslBufferType
    PsslInternalBufferType internaltype;  // PsslInternalBufferType
    PsslType type;	       // PsslType
	uint8_t _unused;
	// used by constant buffer
	uint32_t numelems;
	uint32_t elementoff;
	uint32_t nameoff;
};
static_assert(sizeof(PsslBinaryBuffer) == 0x18, "");

struct PsslBinaryConstant {
	uint32_t elementoffset;
	uint32_t registeridx;
	uint8_t constantbufidx;
    PsslType elementtype;  // PsslType
    PsslBufferType buffertype;   // PsslBufferType
	uint8_t _unused;

	union {
		float defaultf32[4];
		uint32_t defaultu32[4];
	};

	uint32_t nameoff;
};
static_assert(sizeof(PsslBinaryConstant) == 0x20, "");

struct PsslBinaryElement {
	uint8_t type;  // PsslType

	uint8_t isused : 1;
	uint8_t isrowmajor : 1;
	uint8_t _unused : 6;

	uint8_t ispointer : 1;
	uint8_t _unused2 : 7;

    PsslElementType elementtype;  // PsslElementType
	uint32_t byteoffset;
	uint32_t size;
	uint32_t arraysize;

	union {
		uint32_t defaultvalueoff;
		uint32_t resourceidx;
	};

	// for structure
	uint32_t numelements;
	uint32_t elementoffset;

	uint32_t nameoff;
	uint32_t typenameoff;
};
static_assert(sizeof(PsslBinaryElement) == 0x24, "");

struct PsslBinarySamplerState {
	uint64_t _unused;
	uint32_t resourceidx;
	uint32_t nameoff;
};
static_assert(sizeof(PsslBinarySamplerState) == 0x10, "");

static inline PsslCompilerType psslSbCompilerType(const PsslBinaryHeader* hdr) {
	assert(hdr);

	const uint8_t val = hdr->compilertype & 0xf;
	if (val > PSSL_COMPILER_ORBIS_CU_AS) {
		assert(0);
	}
    return (PsslCompilerType)val;
}

static inline const GnmShaderFileHeader* psslSbGnmShader(
    const PsslBinaryHeader* hdr
) {
	if (!hdr) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)hdr + sizeof(PsslBinaryHeader);
	return (const GnmShaderFileHeader*)ptr;
}

static inline const GnmShaderBinaryInfo* psslSbGnmShaderBinaryInfo(const PsslBinaryHeader* hdr) {
	if (!hdr) {
		return NULL;
	}

	const GnmShaderFileHeader* sfhdr = psslSbGnmShader(hdr);
	const GnmShaderCommonData* common = gnmShfCommonData(sfhdr);

	const uint8_t* ptr = (const uint8_t*)hdr + sizeof(PsslBinaryHeader) +
			     hdr->codesize - sizeof(GnmShaderBinaryInfo) - common->embeddedconstantbufferdqwords * 16;
	return (const GnmShaderBinaryInfo*)ptr;
}

static inline const PsslBinaryParamInfo* psslSbParameterInfo(
    const PsslBinaryHeader* hdr
) {
	if (!hdr) {
		return NULL;
	}
	const uint8_t* ptr =
	    (const uint8_t*)hdr + sizeof(PsslBinaryHeader) + hdr->codesize;
	return (const PsslBinaryParamInfo*)ptr;
}

static inline const PsslBinaryBuffer* psslParamBuffer(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numbufferresources < idx) {
		return NULL;
	}
	const void* ptr = (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
			  sizeof(PsslBinaryBuffer) * idx;
    return (const PsslBinaryBuffer*)ptr;
}
static inline const PsslBinaryConstant* psslParamConstant(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numconstants < idx) {
		return NULL;
	}
	const void* ptr = (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
			  sizeof(PsslBinaryBuffer) * info->numbufferresources +
			  sizeof(PsslBinaryConstant) * idx;
    return (const PsslBinaryConstant*)ptr;
}
static inline const PsslBinaryElement* psslParamVariable(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numvariables < idx) {
		return NULL;
	}
	const void* ptr = (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
			  sizeof(PsslBinaryBuffer) * info->numbufferresources +
			  sizeof(PsslBinaryConstant) * info->numconstants +
			  sizeof(PsslBinaryElement) * idx;
    return (const PsslBinaryElement*)ptr;
}
static inline const PsslBinarySamplerState* psslParamSampler(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numsamplerresources < idx) {
		return NULL;
	}
	const void* ptr = (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
			  sizeof(PsslBinaryBuffer) * info->numbufferresources +
			  sizeof(PsslBinaryConstant) * info->numconstants +
			  sizeof(PsslBinaryElement) * info->numvariables +
			  sizeof(PsslBinarySamplerState) * idx;
    return (const PsslBinarySamplerState*)ptr;
}
static inline const PsslBinaryAttribute* psslParamInputAttr(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numinputattributes < idx) {
		return NULL;
	}
	const void* ptr =
	    (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
	    sizeof(PsslBinaryBuffer) * info->numbufferresources +
	    sizeof(PsslBinaryConstant) * info->numconstants +
	    sizeof(PsslBinaryElement) * info->numvariables +
	    sizeof(PsslBinarySamplerState) * info->numsamplerresources +
	    sizeof(PsslBinaryAttribute) * idx;
    return (const PsslBinaryAttribute*)ptr;
}
static inline const PsslBinaryAttribute* psslParamOutputAttr(
    const PsslBinaryParamInfo* info, uint32_t idx
) {
	if (!info || info->numoutputattributes < idx) {
		return NULL;
	}
	const void* ptr =
	    (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
	    sizeof(PsslBinaryBuffer) * info->numbufferresources +
	    sizeof(PsslBinaryConstant) * info->numconstants +
	    sizeof(PsslBinaryElement) * info->numvariables +
	    sizeof(PsslBinarySamplerState) * info->numsamplerresources +
	    sizeof(PsslBinaryAttribute) * info->numinputattributes +
	    sizeof(PsslBinaryAttribute) * idx;
    return (const PsslBinaryAttribute*)ptr;
}
static inline const char* psslParamStringTable(const PsslBinaryParamInfo* info
) {
	if (!info) {
		return NULL;
	}
	const void* ptr =
	    (const uint8_t*)info + sizeof(PsslBinaryParamInfo) +
	    sizeof(PsslBinaryBuffer) * info->numbufferresources +
	    sizeof(PsslBinaryConstant) * info->numconstants +
	    sizeof(PsslBinaryElement) * info->numvariables +
	    sizeof(PsslBinarySamplerState) * info->numsamplerresources +
	    sizeof(PsslBinaryAttribute) * info->numinputattributes +
	    sizeof(PsslBinaryAttribute) * info->numoutputattributes +
	    info->valuetablesize;
    return (const char*)ptr;
}

static inline const char* psslAttrName(const PsslBinaryAttribute* attr) {
	if (!attr) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)&attr->nameoff + attr->nameoff;
	return (const char*)ptr;
}
static inline const char* psslAttrSemanticName(const PsslBinaryAttribute* attr
) {
	if (!attr) {
		return NULL;
	}
	const uint8_t* ptr =
	    (const uint8_t*)&attr->semanticnameoff + attr->semanticnameoff;
	return (const char*)ptr;
}

static inline const char* psslBufferName(const PsslBinaryBuffer* buf) {
	if (!buf) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)&buf->nameoff + buf->nameoff;
	return (const char*)ptr;
}

static inline const char* psslConstantName(const PsslBinaryConstant* cnst) {
	if (!cnst) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)&cnst->nameoff + cnst->nameoff;
	return (const char*)ptr;
}

static inline const char* psslElementName(const PsslBinaryElement* elem) {
	if (!elem) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)&elem->nameoff + elem->nameoff;
	return (const char*)ptr;
}
static inline const char* psslElementTypeName(const PsslBinaryElement* elem) {
	if (!elem) {
		return NULL;
	}
	const uint8_t* ptr =
	    (const uint8_t*)&elem->typenameoff + elem->typenameoff;
	return (const char*)ptr;
}

static inline const char* psslSamplerStateName(
    const PsslBinarySamplerState* state
) {
	if (!state) {
		return NULL;
	}
	const uint8_t* ptr = (const uint8_t*)&state->nameoff + state->nameoff;
	return (const char*)ptr;
}
