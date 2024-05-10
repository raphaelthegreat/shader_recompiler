#ifndef _GNM_STRINGS_H_
#define _GNM_STRINGS_H_

#include "shaderbinary.h"

static inline const char* gnmStrShaderInputUsageType(
    GnmShaderInputUsageType type
) {
	switch (type) {
	case GNM_SHINPUTUSAGE_IMM_RESOURCE:
		return "imm_resource";
	case GNM_SHINPUTUSAGE_IMM_SAMPLER:
		return "imm_sampler";
	case GNM_SHINPUTUSAGE_IMM_CONSTBUFFER:
		return "imm_constbuffer";
	case GNM_SHINPUTUSAGE_IMM_VERTEXBUFFER:
		return "imm_vertexbuffer";
	case GNM_SHINPUTUSAGE_IMM_RWRESOURCE:
		return "imm_rwresource";
	case GNM_SHINPUTUSAGE_IMM_ALUFLOATCONST:
		return "imm_alufloatconst";
	case GNM_SHINPUTUSAGE_IMM_ALUBOOL32CONST:
		return "imm_alubool32const";
	case GNM_SHINPUTUSAGE_IMM_GDSCOUNTERRANGE:
		return "imm_gdscounterrange";
	case GNM_SHINPUTUSAGE_IMM_GDSMEMORYRANGE:
		return "imm_gdsmemoryrange";
	case GNM_SHINPUTUSAGE_IMM_GWSBASE:
		return "imm_gwsbase";
	case GNM_SHINPUTUSAGE_IMM_SRT:
		return "imm_srt";
	case GNM_SHINPUTUSAGE_IMM_LDSESGSSIZE:
		return "imm_ldsesgssize";
	case GNM_SHINPUTUSAGE_SUBPTR_FETCHSHADER:
		return "subptr_fetchshader";
	case GNM_SHINPUTUSAGE_PTR_RESOURCETABLE:
		return "ptr_resourcetable";
	case GNM_SHINPUTUSAGE_PTR_INTERNALRESOURCETABLE:
		return "ptr_internalresourcetable";
	case GNM_SHINPUTUSAGE_PTR_SAMPLERTABLE:
		return "ptr_samplertable";
	case GNM_SHINPUTUSAGE_PTR_CONSTBUFFERTABLE:
		return "ptr_constbuffertable";
	case GNM_SHINPUTUSAGE_PTR_VERTEXBUFFERTABLE:
		return "ptr_vertexbuffertable";
	case GNM_SHINPUTUSAGE_PTR_SOBUFFERTABLE:
		return "ptr_sobuffertable";
	case GNM_SHINPUTUSAGE_PTR_RWRESOURCETABLE:
		return "ptr_rwresourcetable";
	case GNM_SHINPUTUSAGE_PTR_INTERNALGLOBALTABLE:
		return "ptr_internalglobaltable";
	case GNM_SHINPUTUSAGE_PTR_EXTENDEDUSERDATA:
		return "ptr_extendeduserdata";
	case GNM_SHINPUTUSAGE_PTR_INDIRECTRESOURCETABLE:
		return "ptr_indirectresourcetable";
	case GNM_SHINPUTUSAGE_PTR_INDIRECTINTERNALRESOURCETABLE:
		return "ptr_indirectinternalresourcetable";
	case GNM_SHINPUTUSAGE_PTR_INDIRECTRWRESOURCETABLE:
		return "ptr_indirectrwresourcetable";
	default:
		return "Unknown type";
	}
}

static inline const char* gnmStrPixelDefaultValue(GnmPixelDefaultValue type) {
	switch (type) {
	case GNM_PX_DEFVAL_NONE:
		return "None";
	case GNM_PX_DEFVAL_0_0_0_1:
		return "{0,0,0,1}";
	case GNM_PX_DEFVAL_1_1_1_0:
		return "{1,1,1,0}";
	case GNM_PX_DEFVAL_1_1_1_1:
		return "{1,1,1,1}";
	default:
		return "Unknown value";
	}
}

static inline const char* gnmStrShaderType(GnmShaderType type) {
	switch (type) {
	case GNM_SHADER_INVALID:
		return "Invalid shader";
	case GNM_SHADER_VERTEX:
		return "Vertex shader";
	case GNM_SHADER_PIXEL:
		return "Pixel shader";
	case GNM_SHADER_GEOMETRY:
		return "Geometry shader";
	case GNM_SHADER_COMPUTE:
		return "Compute shader";
	case GNM_SHADER_EXPORT:
		return "Export shader";
	case GNM_SHADER_LOCAL:
		return "Local shader";
	case GNM_SHADER_HULL:
		return "Hull shader";
	default:
		return "Unknown shader";
	}
}

static inline const char* gnmStrShaderBinaryType(GnmShaderBinaryType type) {
	switch (type) {
	case GNM_SHB_PS:
		return "Pixel Shader";
	case GNM_SHB_VS_VS:
		return "Vertex Shader (VS)";
	case GNM_SHB_VS_ES:
		return "Vertex Shader (ES)";
	case GNM_SHB_VS_LS:
		return "Vertex Shader (LS)";
	case GNM_SHB_CS:
		return "Compute Shader";
	case GNM_SHB_GS:
		return "Geometry Shader";
	case GNM_SHB_GS_VS:
		return "Geometry Shader (VS)";
	case GNM_SHB_HS:
		return "Hull Shader";
	case GNM_SHB_DS_VS:
		return "Domain Shader (VS)";
	case GNM_SHB_DS_ES:
		return "Domain Shader (ES)";
	default:
		return "Unknown shader";
	}
}

static inline const char* gnmStrTargetGpuMode(GnmTargetGpuMode mode) {
	if (mode & GNM_TARGETGPUMODE_BASE && mode & GNM_TARGETGPUMODE_NEO) {
		return "Base and NEO modes";
	}
	switch (mode) {
	case GNM_TARGETGPUMODE_UNSPECIFIED:
		return "Unspecified";
	case GNM_TARGETGPUMODE_BASE:
		return "Base mode";
	case GNM_TARGETGPUMODE_NEO:
		return "NEO mode";
	default:
		return "Unknown mode";
	}
}

static inline const char* gnmStrTexChannelType(GnmImgNumFormat type) {
    switch (type) {
    case GNM_IMG_NUM_FORMAT_UNORM:
        return "UNORM";
    case GNM_IMG_NUM_FORMAT_SNORM:
        return "SNORM";
    case GNM_IMG_NUM_FORMAT_USCALED:
        return "USCALED";
    case GNM_IMG_NUM_FORMAT_SSCALED:
        return "SSCALED";
    case GNM_IMG_NUM_FORMAT_UINT:
        return "UINT";
    case GNM_IMG_NUM_FORMAT_SINT:
        return "SINT";
    case GNM_IMG_NUM_FORMAT_SNORM_OGL:
        return "SNORM_OGL";
    case GNM_IMG_NUM_FORMAT_FLOAT:
        return "FLOAT";
    case GNM_IMG_NUM_FORMAT_SRGB:
        return "SRGB";
    case GNM_IMG_NUM_FORMAT_UBNORM:
        return "UBNORM";
    case GNM_IMG_NUM_FORMAT_UBNORM_OGL:
        return "UBNORM_OGL";
    case GNM_IMG_NUM_FORMAT_UBINT:
        return "UBINT";
    case GNM_IMG_NUM_FORMAT_UBSCALED:
        return "UBSCALED";
    default:
        return "Unknown type";
    }
}

static inline const char* gnmStrSurfaceFormat(GnmImageFormat fmt) {
    switch (fmt) {
    case GNM_IMG_DATA_FORMAT_INVALID:
        return "Invalid";
    case GNM_IMG_DATA_FORMAT_8:
        return "8";
    case GNM_IMG_DATA_FORMAT_16:
        return "16";
    case GNM_IMG_DATA_FORMAT_8_8:
        return "8_8";
    case GNM_IMG_DATA_FORMAT_32:
        return "32";
    case GNM_IMG_DATA_FORMAT_16_16:
        return "16_16";
    case GNM_IMG_DATA_FORMAT_10_11_11:
        return "10_11_11";
    case GNM_IMG_DATA_FORMAT_11_11_10:
        return "11_11_10";
    case GNM_IMG_DATA_FORMAT_10_10_10_2:
        return "10_10_10_2";
    case GNM_IMG_DATA_FORMAT_2_10_10_10:
        return "2_10_10_10";
    case GNM_IMG_DATA_FORMAT_8_8_8_8:
        return "8_8_8_8";
    case GNM_IMG_DATA_FORMAT_32_32:
        return "32_32";
    case GNM_IMG_DATA_FORMAT_16_16_16_16:
        return "16_16_16_16";
    case GNM_IMG_DATA_FORMAT_32_32_32:
        return "32_32_32";
    case GNM_IMG_DATA_FORMAT_32_32_32_32:
        return "32_32_32_32";
    case GNM_IMG_DATA_FORMAT_5_6_5:
        return "5_6_5";
    case GNM_IMG_DATA_FORMAT_1_5_5_5:
        return "1_5_5_5";
    case GNM_IMG_DATA_FORMAT_5_5_5_1:
        return "5_5_5_1";
    case GNM_IMG_DATA_FORMAT_4_4_4_4:
        return "4_4_4_4";
    case GNM_IMG_DATA_FORMAT_8_24:
        return "8_24";
    case GNM_IMG_DATA_FORMAT_24_8:
        return "24_8";
    case GNM_IMG_DATA_FORMAT_X24_8_32:
        return "X24_8_32";
    case GNM_IMG_DATA_FORMAT_GB_GR:
        return "GB_GR";
    case GNM_IMG_DATA_FORMAT_BG_RG:
        return "BG_RG";
    case GNM_IMG_DATA_FORMAT_5_9_9_9:
        return "5_9_9_9";
    case GNM_IMG_DATA_FORMAT_BC1:
        return "BC1";
    case GNM_IMG_DATA_FORMAT_BC2:
        return "BC2";
    case GNM_IMG_DATA_FORMAT_BC3:
        return "BC3";
    case GNM_IMG_DATA_FORMAT_BC4:
        return "BC4";
    case GNM_IMG_DATA_FORMAT_BC5:
        return "BC5";
    case GNM_IMG_DATA_FORMAT_BC6:
        return "BC6";
    case GNM_IMG_DATA_FORMAT_BC7:
        return "BC7";
    case GNM_IMG_DATA_FORMAT_FMASK8_S2_F1:
        return "FMASK8_S2_F1";
    case GNM_IMG_DATA_FORMAT_FMASK8_S4_F1:
        return "FMASK8_S4_F1";
    case GNM_IMG_DATA_FORMAT_FMASK8_S8_F1:
        return "FMASK8_S8_F1";
    case GNM_IMG_DATA_FORMAT_FMASK8_S2_F2:
        return "FMASK8_S2_F2";
    case GNM_IMG_DATA_FORMAT_FMASK8_S4_F2:
        return "FMASK8_S4_F2";
    case GNM_IMG_DATA_FORMAT_FMASK8_S4_F4:
        return "FMASK8_S4_F4";
    case GNM_IMG_DATA_FORMAT_FMASK16_S16_F1:
        return "FMASK16_S16_F1";
    case GNM_IMG_DATA_FORMAT_FMASK16_S8_F2:
        return "FMASK16_S8_F2";
    case GNM_IMG_DATA_FORMAT_FMASK32_S16_F2:
        return "FMASK32_S16_F2";
    case GNM_IMG_DATA_FORMAT_FMASK32_S8_F4:
        return "FMASK32_S8_F4";
    case GNM_IMG_DATA_FORMAT_FMASK32_S8_F8:
        return "FMASK32_S8_F8";
    case GNM_IMG_DATA_FORMAT_FMASK64_S16_F4:
        return "FMASK64_S16_F4";
    case GNM_IMG_DATA_FORMAT_FMASK64_S16_F8:
        return "FMASK64_S16_F8";
    case GNM_IMG_DATA_FORMAT_4_4:
        return "4_4";
    case GNM_IMG_DATA_FORMAT_6_5_5:
        return "6_5_5";
    case GNM_IMG_DATA_FORMAT_1:
        return "1";
    case GNM_IMG_DATA_FORMAT_1_REVERSED:
        return "1_REVERSED";
    default:
        return "Unknown format";
    }
}

#endif	// _GNM_STRINGS_H_
