#include "types.h"

const char* psslStrType(PsslType type) {
	switch (type) {
	case PSSL_TYPE_FLOAT1:
		return "float1";
	case PSSL_TYPE_FLOAT2:
		return "float2";
	case PSSL_TYPE_FLOAT3:
		return "float3";
	case PSSL_TYPE_FLOAT4:
		return "float4";
	case PSSL_TYPE_HALF1:
		return "half1";
	case PSSL_TYPE_HALF2:
		return "half2";
	case PSSL_TYPE_HALF3:
		return "half3";
	case PSSL_TYPE_HALF4:
		return "half4";
	case PSSL_TYPE_INT1:
		return "int1";
	case PSSL_TYPE_INT2:
		return "int2";
	case PSSL_TYPE_INT3:
		return "int3";
	case PSSL_TYPE_INT4:
		return "int4";
	case PSSL_TYPE_UINT1:
		return "uint1";
	case PSSL_TYPE_UINT2:
		return "uint2";
	case PSSL_TYPE_UINT3:
		return "uint3";
	case PSSL_TYPE_UINT4:
		return "uint4";
	case PSSL_TYPE_DOUBLE1:
		return "double1";
	case PSSL_TYPE_DOUBLE2:
		return "double2";
	case PSSL_TYPE_DOUBLE3:
		return "double3";
	case PSSL_TYPE_DOUBLE4:
		return "double4";
	case PSSL_TYPE_FLOAT1X1:
		return "float1x1";
	case PSSL_TYPE_FLOAT2X1:
		return "float2x1";
	case PSSL_TYPE_FLOAT3X1:
		return "float3x1";
	case PSSL_TYPE_FLOAT4X1:
		return "float4x1";
	case PSSL_TYPE_FLOAT1X2:
		return "float1x2";
	case PSSL_TYPE_FLOAT2X2:
		return "float2x2";
	case PSSL_TYPE_FLOAT3X2:
		return "float3x2";
	case PSSL_TYPE_FLOAT4X2:
		return "float4x2";
	case PSSL_TYPE_FLOAT1X3:
		return "float1x3";
	case PSSL_TYPE_FLOAT2X3:
		return "float2x3";
	case PSSL_TYPE_FLOAT3X3:
		return "float3x3";
	case PSSL_TYPE_FLOAT4X3:
		return "float4x3";
	case PSSL_TYPE_FLOAT1X4:
		return "float1x4";
	case PSSL_TYPE_FLOAT2X4:
		return "float2x4";
	case PSSL_TYPE_FLOAT3X4:
		return "float3x4";
	case PSSL_TYPE_FLOAT4X4:
		return "float4x4";
	case PSSL_TYPE_HALF1X1:
		return "half1x1";
	case PSSL_TYPE_HALF2X1:
		return "half2x1";
	case PSSL_TYPE_HALF3X1:
		return "half3x1";
	case PSSL_TYPE_HALF4X1:
		return "half4x1";
	case PSSL_TYPE_HALF1X2:
		return "half1x2";
	case PSSL_TYPE_HALF2X2:
		return "half2x2";
	case PSSL_TYPE_HALF3X2:
		return "half3x2";
	case PSSL_TYPE_HALF4X2:
		return "half4x2";
	case PSSL_TYPE_HALF1X3:
		return "half1x3";
	case PSSL_TYPE_HALF2X3:
		return "half2x3";
	case PSSL_TYPE_HALF3X3:
		return "half3x3";
	case PSSL_TYPE_HALF4X3:
		return "half4x3";
	case PSSL_TYPE_HALF1X4:
		return "half1x4";
	case PSSL_TYPE_HALF2X4:
		return "half2x4";
	case PSSL_TYPE_HALF3X4:
		return "half3x4";
	case PSSL_TYPE_HALF4X4:
		return "half4x4";
	case PSSL_TYPE_INT1X1:
		return "int1x1";
	case PSSL_TYPE_INT2X1:
		return "int2x1";
	case PSSL_TYPE_INT3X1:
		return "int3x1";
	case PSSL_TYPE_INT4X1:
		return "int4x1";
	case PSSL_TYPE_INT1X2:
		return "int1x2";
	case PSSL_TYPE_INT2X2:
		return "int2x2";
	case PSSL_TYPE_INT3X2:
		return "int3x2";
	case PSSL_TYPE_INT4X2:
		return "int4x2";
	case PSSL_TYPE_INT1X3:
		return "int1x3";
	case PSSL_TYPE_INT2X3:
		return "int2x3";
	case PSSL_TYPE_INT3X3:
		return "int3x3";
	case PSSL_TYPE_INT4X3:
		return "int4x3";
	case PSSL_TYPE_INT1X4:
		return "int1x4";
	case PSSL_TYPE_INT2X4:
		return "int2x4";
	case PSSL_TYPE_INT3X4:
		return "int3x4";
	case PSSL_TYPE_INT4X4:
		return "int4x4";
	case PSSL_TYPE_UINT1X1:
		return "uint1x1";
	case PSSL_TYPE_UINT2X1:
		return "uint2x1";
	case PSSL_TYPE_UINT3X1:
		return "uint3x1";
	case PSSL_TYPE_UINT4X1:
		return "uint4x1";
	case PSSL_TYPE_UINT1X2:
		return "uint1x2";
	case PSSL_TYPE_UINT2X2:
		return "uint2x2";
	case PSSL_TYPE_UINT3X2:
		return "uint3x2";
	case PSSL_TYPE_UINT4X2:
		return "uint4x2";
	case PSSL_TYPE_UINT1X3:
		return "uint1x3";
	case PSSL_TYPE_UINT2X3:
		return "uint2x3";
	case PSSL_TYPE_UINT3X3:
		return "uint3x3";
	case PSSL_TYPE_UINT4X3:
		return "uint4x3";
	case PSSL_TYPE_UINT1X4:
		return "uint1x4";
	case PSSL_TYPE_UINT2X4:
		return "uint2x4";
	case PSSL_TYPE_UINT3X4:
		return "uint3x4";
	case PSSL_TYPE_UINT4X4:
		return "uint4x4";
	case PSSL_TYPE_DOUBLE1X1:
		return "double1x1";
	case PSSL_TYPE_DOUBLE2X1:
		return "double2x1";
	case PSSL_TYPE_DOUBLE3X1:
		return "double3x1";
	case PSSL_TYPE_DOUBLE4X1:
		return "double4x1";
	case PSSL_TYPE_DOUBLE1X2:
		return "double1x2";
	case PSSL_TYPE_DOUBLE2X2:
		return "double2x2";
	case PSSL_TYPE_DOUBLE3X2:
		return "double3x2";
	case PSSL_TYPE_DOUBLE4X2:
		return "double4x2";
	case PSSL_TYPE_DOUBLE1X3:
		return "double1x3";
	case PSSL_TYPE_DOUBLE2X3:
		return "double2x3";
	case PSSL_TYPE_DOUBLE3X3:
		return "double3x3";
	case PSSL_TYPE_DOUBLE4X3:
		return "double4x3";
	case PSSL_TYPE_DOUBLE1X4:
		return "double1x4";
	case PSSL_TYPE_DOUBLE2X4:
		return "double2x4";
	case PSSL_TYPE_DOUBLE3X4:
		return "double3x4";
	case PSSL_TYPE_DOUBLE4X4:
		return "double4x4";
	case PSSL_TYPE_POINT:
		return "point";
	case PSSL_TYPE_LINE:
		return "line";
	case PSSL_TYPE_TRIANGLE:
		return "triangle";
	case PSSL_TYPE_ADJACENTLINE:
		return "adjacentline";
	case PSSL_TYPE_ADJACENTTRIANGLE:
		return "adjacenttriangle";
	case PSSL_TYPE_PATCH:
		return "patch";
	case PSSL_TYPE_STRUCTURE:
		return "structure";
	case PSSL_TYPE_LONG1:
		return "long1";
	case PSSL_TYPE_LONG2:
		return "long2";
	case PSSL_TYPE_LONG3:
		return "long3";
	case PSSL_TYPE_LONG4:
		return "long4";
	case PSSL_TYPE_ULONG1:
		return "ulong1";
	case PSSL_TYPE_ULONG2:
		return "ulong2";
	case PSSL_TYPE_ULONG3:
		return "ulong3";
	case PSSL_TYPE_ULONG4:
		return "ulong4";
	case PSSL_TYPE_LONG1X1:
		return "long1x1";
	case PSSL_TYPE_LONG2X1:
		return "long2x1";
	case PSSL_TYPE_LONG3X1:
		return "long3x1";
	case PSSL_TYPE_LONG4X1:
		return "long4x1";
	case PSSL_TYPE_LONG1X2:
		return "long1x2";
	case PSSL_TYPE_LONG2X2:
		return "long2x2";
	case PSSL_TYPE_LONG3X2:
		return "long3x2";
	case PSSL_TYPE_LONG4X2:
		return "long4x2";
	case PSSL_TYPE_LONG1X3:
		return "long1x3";
	case PSSL_TYPE_LONG2X3:
		return "long2x3";
	case PSSL_TYPE_LONG3X3:
		return "long3x3";
	case PSSL_TYPE_LONG4X3:
		return "long4x3";
	case PSSL_TYPE_LONG1X4:
		return "long1x4";
	case PSSL_TYPE_LONG2X4:
		return "long2x4";
	case PSSL_TYPE_LONG3X4:
		return "long3x4";
	case PSSL_TYPE_LONG4X4:
		return "long4x4";
	case PSSL_TYPE_ULONG1X1:
		return "ulong1x1";
	case PSSL_TYPE_ULONG2X1:
		return "ulong2x1";
	case PSSL_TYPE_ULONG3X1:
		return "ulong3x1";
	case PSSL_TYPE_ULONG4X1:
		return "ulong4x1";
	case PSSL_TYPE_ULONG1X2:
		return "ulong1x2";
	case PSSL_TYPE_ULONG2X2:
		return "ulong2x2";
	case PSSL_TYPE_ULONG3X2:
		return "ulong3x2";
	case PSSL_TYPE_ULONG4X2:
		return "ulong4x2";
	case PSSL_TYPE_ULONG1X3:
		return "ulong1x3";
	case PSSL_TYPE_ULONG2X3:
		return "ulong2x3";
	case PSSL_TYPE_ULONG3X3:
		return "ulong3x3";
	case PSSL_TYPE_ULONG4X3:
		return "ulong4x3";
	case PSSL_TYPE_ULONG1X4:
		return "ulong1x4";
	case PSSL_TYPE_ULONG2X4:
		return "ulong2x4";
	case PSSL_TYPE_ULONG3X4:
		return "ulong3x4";
	case PSSL_TYPE_ULONG4X4:
		return "ulong4x4";
	case PSSL_TYPE_BOOL1:
		return "bool1";
	case PSSL_TYPE_BOOL2:
		return "bool2";
	case PSSL_TYPE_BOOL3:
		return "bool3";
	case PSSL_TYPE_BOOL4:
		return "bool4";
	case PSSL_TYPE_TEXTURE:
		return "texture";
	case PSSL_TYPE_SAMPLERSTATE:
		return "samplerstate";
	case PSSL_TYPE_BUFFER:
		return "buffer";
	case PSSL_TYPE_USHORT1:
		return "ushort1";
	case PSSL_TYPE_USHORT2:
		return "ushort2";
	case PSSL_TYPE_USHORT3:
		return "ushort3";
	case PSSL_TYPE_USHORT4:
		return "ushort4";
	case PSSL_TYPE_UCHAR1:
		return "uchar1";
	case PSSL_TYPE_UCHAR2:
		return "uchar2";
	case PSSL_TYPE_UCHAR3:
		return "uchar3";
	case PSSL_TYPE_UCHAR4:
		return "uchar4";
	case PSSL_TYPE_SHORT1:
		return "short1";
	case PSSL_TYPE_SHORT2:
		return "short2";
	case PSSL_TYPE_SHORT3:
		return "short3";
	case PSSL_TYPE_SHORT4:
		return "short4";
	case PSSL_TYPE_CHAR1:
		return "char1";
	case PSSL_TYPE_CHAR2:
		return "char2";
	case PSSL_TYPE_CHAR3:
		return "char3";
	case PSSL_TYPE_CHAR4:
		return "char4";
	case PSSL_TYPE_VOID:
		return "void";
	case PSSL_TYPE_TEXTURER128:
		return "texturer128";
	default:
		return "Invalid type";
	}
}

const char* psslStrSemantic(PsslSemantic semantic) {
	switch (semantic) {
	case PSSL_SEMANTIC_POSITION:
		return "position";
	case PSSL_SEMANTIC_NORMAL:
		return "normal";
	case PSSL_SEMANTIC_COLOR:
		return "color";
	case PSSL_SEMANTIC_BINORMAL:
		return "binormal";
	case PSSL_SEMANTIC_TANGENT:
		return "tangent";
	case PSSL_SEMANTIC_TEXCOORD0:
		return "texcoord0";
	case PSSL_SEMANTIC_TEXCOORD1:
		return "texcoord1";
	case PSSL_SEMANTIC_TEXCOORD2:
		return "texcoord2";
	case PSSL_SEMANTIC_TEXCOORD3:
		return "texcoord3";
	case PSSL_SEMANTIC_TEXCOORD4:
		return "texcoord4";
	case PSSL_SEMANTIC_TEXCOORD5:
		return "texcoord5";
	case PSSL_SEMANTIC_TEXCOORD6:
		return "texcoord6";
	case PSSL_SEMANTIC_TEXCOORD7:
		return "texcoord7";
	case PSSL_SEMANTIC_TEXCOORD8:
		return "texcoord8";
	case PSSL_SEMANTIC_TEXCOORD9:
		return "texcoord9";
	case PSSL_SEMANTIC_TEXCOORDEND:
		return "texcoordend";
	case PSSL_SEMANTIC_IMPLICIT:
		return "implicit";
	case PSSL_SEMANTIC_NONREFERENCABLE:
		return "nonreferencable";
	case PSSL_SEMANTIC_CLIP:
		return "clip";
	case PSSL_SEMANTIC_FOG:
		return "fog";
	case PSSL_SEMANTIC_SPOINTSIZE:
		return "spointsize";
	case PSSL_SEMANTIC_FRAGCOORD:
		return "fragcoord";
	case PSSL_SEMANTIC_TARGET0:
		return "target0";
	case PSSL_SEMANTIC_TARGET1:
		return "target1";
	case PSSL_SEMANTIC_TARGET2:
		return "target2";
	case PSSL_SEMANTIC_TARGET3:
		return "target3";
	case PSSL_SEMANTIC_TARGET4:
		return "target4";
	case PSSL_SEMANTIC_TARGET5:
		return "target5";
	case PSSL_SEMANTIC_TARGET6:
		return "target6";
	case PSSL_SEMANTIC_TARGET7:
		return "target7";
	case PSSL_SEMANTIC_TARGET8:
		return "target8";
	case PSSL_SEMANTIC_TARGET9:
		return "target9";
	case PSSL_SEMANTIC_TARGET10:
		return "target10";
	case PSSL_SEMANTIC_TARGET11:
		return "target11";
	case PSSL_SEMANTIC_DEPTH:
		return "depth";
	case PSSL_SEMANTIC_LASTCG:
		return "lastcg";
	case PSSL_SEMANTIC_USERDEFINED:
		return "userdefined";
	case PSSL_SEMANTIC_SCLIPDISTANCE:
		return "sclipdistance";
	case PSSL_SEMANTIC_SCULLDISTANCE:
		return "sculldistance";
	case PSSL_SEMANTIC_SCOVERAGE:
		return "scoverage";
	case PSSL_SEMANTIC_SDEPTHOUTPUT:
		return "sdepthoutput";
	case PSSL_SEMANTIC_SDISPATCHTHREADID:
		return "sdispatchthreadid";
	case PSSL_SEMANTIC_SDOMAINLOCATION:
		return "sdomainlocation";
	case PSSL_SEMANTIC_SGROUPID:
		return "sgroupid";
	case PSSL_SEMANTIC_SGROUPINDEX:
		return "sgroupindex";
	case PSSL_SEMANTIC_SGROUPTHREADID:
		return "sgroupthreadid";
	case PSSL_SEMANTIC_SPOSITION:
		return "sposition";
	case PSSL_SEMANTIC_SVERTEXID:
		return "svertexid";
	case PSSL_SEMANTIC_SINSTANCEID:
		return "sinstanceid";
	case PSSL_SEMANTIC_SSAMPLEINDEX:
		return "ssampleindex";
	case PSSL_SEMANTIC_SPRIMITIVEID:
		return "sprimitiveid";
	case PSSL_SEMANTIC_SGSINSTANCEID:
		return "sgsinstanceid";
	case PSSL_SEMANTIC_SOUTPUTCONTROLPOINTID:
		return "soutputcontrolpointid";
	case PSSL_SEMANTIC_SFRONTFACE:
		return "sfrontface";
	case PSSL_SEMANTIC_SRENDERTARGETINDEX:
		return "srendertargetindex";
	case PSSL_SEMANTIC_SVIEWPORTINDEX:
		return "sviewportindex";
	case PSSL_SEMANTIC_STARGETOUTPUT:
		return "stargetoutput";
	case PSSL_SEMANTIC_SEDGETESSFACTOR:
		return "sedgetessfactor";
	case PSSL_SEMANTIC_SINSIDETESSFACTOR:
		return "sinsidetessfactor";
	case PSSL_SEMANTIC_SPOINTCOORD:
		return "spointcoord";
	case PSSL_SEMANTIC_SDEPTHGEOUTPUT:
		return "sdepthgeoutput";
	case PSSL_SEMANTIC_SDEPTHLEOUTPUT:
		return "sdepthleoutput";
	case PSSL_SEMANTIC_SVERTEXOFFSETID:
		return "svertexoffsetid";
	case PSSL_SEMANTIC_SINSTANCEOFFSETID:
		return "sinstanceoffsetid";
	case PSSL_SEMANTIC_SSTENCILVALUE:
		return "sstencilvalue";
	case PSSL_SEMANTIC_SSTENCILOP:
		return "sstencilop";
	default:
		return "Invalid semantic";
	}
}

const char* psslStrFragmentInterpType(PsslFragmentInterpType type) {
	switch (type) {
	case PSSL_FRAG_INTERP_SAMPLE_NONE:
		return "Sample None";
	case PSSL_FRAG_INTERP_SAMPLE_LINEAR:
		return "Sample Linear";
	case PSSL_FRAG_INTERP_SAMPLE_POINT:
		return "Sample Point";
	default:
		return "Invalid type";
	}
}

const char* psslStrBufferType(PsslBufferType type) {
	switch (type) {
	case PSSL_BUFFER_DATABUFFER:
		return "databuffer";
	case PSSL_BUFFER_TEXTURE1D:
		return "texture1d";
	case PSSL_BUFFER_TEXTURE2D:
		return "texture2d";
	case PSSL_BUFFER_TEXTURE3D:
		return "texture3d";
	case PSSL_BUFFER_TEXTURECUBE:
		return "texturecube";
	case PSSL_BUFFER_TEXTURE1DARRAY:
		return "texture1darray";
	case PSSL_BUFFER_TEXTURE2DARRAY:
		return "texture2darray";
	case PSSL_BUFFER_TEXTURECUBEARRAY:
		return "texturecubearray";
	case PSSL_BUFFER_MSTEXTURE2D:
		return "mstexture2d";
	case PSSL_BUFFER_MSTEXTURE2DARRAY:
		return "mstexture2darray";
	case PSSL_BUFFER_REGULARBUFFER:
		return "regularbuffer";
	case PSSL_BUFFER_BYTEBUFFER:
		return "bytebuffer";
	case PSSL_BUFFER_RWDATABUFFER:
		return "rwdatabuffer";
	case PSSL_BUFFER_RWTEXTURE1D:
		return "rwtexture1d";
	case PSSL_BUFFER_RWTEXTURE2D:
		return "rwtexture2d";
	case PSSL_BUFFER_RWTEXTURE3D:
		return "rwtexture3d";
	case PSSL_BUFFER_RWTEXTURE1DARRAY:
		return "rwtexture1darray";
	case PSSL_BUFFER_RWTEXTURE2DARRAY:
		return "rwtexture2darray";
	case PSSL_BUFFER_RWREGULARBUFFER:
		return "rwregularbuffer";
	case PSSL_BUFFER_RWBYTEBUFFER:
		return "rwbytebuffer";
	case PSSL_BUFFER_APPENDBUFFER:
		return "appendbuffer";
	case PSSL_BUFFER_CONSUMEBUFFER:
		return "consumebuffer";
	case PSSL_BUFFER_CONSTANTBUFFER:
		return "constantbuffer";
	case PSSL_BUFFER_TEXTUREBUFFER:
		return "texturebuffer";
	case PSSL_BUFFER_POINTBUFFER:
		return "pointbuffer";
	case PSSL_BUFFER_LINEBUFFER:
		return "linebuffer";
	case PSSL_BUFFER_TRIANGLEBUFFER:
		return "trianglebuffer";
	case PSSL_BUFFER_SRTBUFFER:
		return "srtbuffer";
	case PSSL_BUFFER_DISPATCHDRAWBUFFER:
		return "dispatchdrawbuffer";
	case PSSL_BUFFER_RWTEXTURECUBE:
		return "rwtexturecube";
	case PSSL_BUFFER_RWTEXTURECUBEARRAY:
		return "rwtexturecubearray";
	case PSSL_BUFFER_RWMSTEXTURE2D:
		return "rwmstexture2d";
	case PSSL_BUFFER_RWMSTEXTURE2DARRAY:
		return "rwmstexture2darray";
	case PSSL_BUFFER_TEXTURE1DR128:
		return "texture1dr128";
	case PSSL_BUFFER_TEXTURE2DR128:
		return "texture2dr128";
	case PSSL_BUFFER_MSTEXTURE2DR128:
		return "mstexture2dr128";
	case PSSL_BUFFER_RWTEXTURE1DR128:
		return "rwtexture1dr128";
	case PSSL_BUFFER_RWTEXTURE2DR128:
		return "rwtexture2dr128";
	case PSSL_BUFFER_RWMSTEXTURE2DR128:
		return "rwmstexture2dr128";
	default:
		return "Invalid type";
	}
}

const char* psslStrInternalBufferType(PsslInternalBufferType type) {
	switch (type) {
	case PSSL_INTERNAL_BUFFER_UAV:
		return "UAV";
	case PSSL_INTERNAL_BUFFER_SRV:
		return "SRV";
	case PSSL_INTERNAL_BUFFER_LDS:
		return "LDS";
	case PSSL_INTERNAL_BUFFER_GDS:
		return "GDS";
	case PSSL_INTERNAL_BUFFER_CBUFFER:
		return "CBuffer";
	case PSSL_INTERNAL_BUFFER_TEXTURE_SAMPLER:
		return "Texture Sampler";
	case PSSL_INTERNAL_BUFFER_INTERNAL:
		return "Internal";
	default:
		return "Invalid type";
	}
}

const char* psslStrElementType(PsslElementType type) {
	switch (type) {
	case PSSL_ELEMENT_ELEMENT:
		return "Element";
	case PSSL_ELEMENT_BUFFER:
		return "Buffer";
	case PSSL_ELEMENT_SAMPLERSTATE:
		return "Sampler State";
	case PSSL_ELEMENT_CONSTANTBUFFER:
		return "Constant Buffer";
	default:
		return "Invalid type";
	}
}

const char* psslStrShaderType(PsslShaderType type) {
	switch (type) {
	case PSSL_SHADER_VS:
		return "Vertex Shader";
	case PSSL_SHADER_FS:
		return "Fragment Shader";
	case PSSL_SHADER_CS:
		return "Compute Shader";
	case PSSL_SHADER_GS:
		return "Geometry Shader";
	case PSSL_SHADER_HS:
		return "Hull Shader";
	case PSSL_SHADER_DS:
		return "Domain Shader";
	default:
		return "Invalid type";
	}
}

const char* psslStrCodeType(PsslCodeType type) {
	switch (type) {
	case PSSL_CODE_IL:
		return "IL";
	case PSSL_CODE_ISA:
		return "ISA";
	case PSSL_CODE_SCU:
		return "SCU";
	default:
		return "Invalid type";
	}
}

const char* psslStrCompilerType(PsslCompilerType type) {
	switch (type) {
	case PSSL_COMPILER_UNSPECIFIED:
		return "Unspecified";
	case PSSL_COMPILER_ORBIS_PSSLC:
		return "orbis-psslc";
	case PSSL_COMPILER_ORBIS_ESSLC:
		return "orbis-esslc";
	case PSSL_COMPILER_ORBIS_WAVE:
		return "orbis-wave";
	case PSSL_COMPILER_ORBIS_CU_AS:
		return "orbis-cu-as";
	default:
		return "Invalid type";
	}
}

const char* psslStrVertexVariant(PsslVertexVariant variant) {
	switch (variant) {
	case PSSL_VSVAR_VERTEX:
		return "Vertex";
	case PSSL_VSVAR_EXPORT:
		return "Export";
	case PSSL_VSVAR_LOCAL:
		return "Local";
	case PSSL_VSVAR_DISPATCHDRAW:
		return "Dispatch draw";
	case PSSL_VSVAR_EXPORTONCHIP:
		return "Export on chip";
	default:
		return "Invalid variant";
	}
}

const char* psslStrDomainVariant(PsslDomainVariant variant) {
	switch (variant) {
	case PSSL_DSVAR_VERTEX:
		return "Vertex";
	case PSSL_DSVAR_EXPORT:
		return "Export";
	case PSSL_DSVAR_VERTEX_HS_ONBUFFER:
		return "Vertex HS on buffer";
	case PSSL_DSVAR_EXPORT_HS_ONBUFFER:
		return "Export HS on buffer";
	case PSSL_DSVAR_VERTEX_HS_DYNAMIC:
		return "Vertex HS dynamic";
	case PSSL_DSVAR_EXPORT_HS_DYNAMIC:
		return "Export HS dynamic";
	case PSSL_DSVAR_EXPORT_ONCHIP:
		return "Export on chip";
	default:
		return "Invalid variant";
	}
}

const char* psslStrGeometryVariant(PsslGeometryVariant variant) {
	switch (variant) {
	case PSSL_GSVAR_ONBUFFER:
		return "On buffer";
	case PSSL_GSVAR_ONCHIP:
		return "On chip";
	default:
		return "Invalid variant";
	}
}

const char* psslStrHullVariant(PsslHullVariant variant) {
	switch (variant) {
	case PSSL_HSVAR_ONCHIP:
		return "On chip";
	case PSSL_HSVAR_ONBUFFER:
		return "On buffer";
	case PSSL_HSVAR_DYNAMIC:
		return "Dynamic";
	default:
		return "Invalid variant";
	}
}

const char* psslStrGsIoType(PsslGsIoType type) {
	switch (type) {
	case PSSL_GSIO_TRI:
		return "Triangle";
	case PSSL_GSIO_LINE:
		return "Line";
	case PSSL_GSIO_POINT:
		return "Point";
	case PSSL_GSIO_ADJTRI:
		return "Adjacent triangle";
	case PSSL_GSIO_ADJLINE:
		return "Adjacent line";
	default:
		return "Invalid type";
	}
}

const char* psslStrHsDsPatchType(PsslHsDsPatchType type) {
	switch (type) {
	case PSSL_HSDSPATCH_ISOLINE:
		return "Isoline";
	case PSSL_HSDSPATCH_TRI:
		return "Triangle";
	case PSSL_HSDSPATCH_QUAD:
		return "Quad";
	default:
		return "Invalid type";
	}
}

const char* psslStrHsTopologyType(PsslHsTopologyType type) {
	switch (type) {
	case PSSL_HSTOPOLOGY_POINT:
		return "Point";
	case PSSL_HSTOPOLOGY_LINE:
		return "Line";
	case PSSL_HSTOPOLOGY_CWTRI:
		return "Clockwise triangle";
	case PSSL_HSTOPOLOGY_CCWTRI:
		return "Counter-clockwise triangle";
	default:
		return "Invalid type";
	}
}

const char* psslStrHsPartitioningType(PsslHsPartitioningType type) {
	switch (type) {
	case PSSL_HSPARTITIONING_INTEGER:
		return "Integer";
	case PSSL_HSPARTITIONING_POWEROF2:
		return "Power of two";
	case PSSL_HSPARTITIONING_ODDFACTORIAL:
		return "Odd factorial";
	case PSSL_HSPARTITIONING_EVENFACTORIAL:
		return "Even factorial";
	default:
		return "Invalid type";
	}
}
