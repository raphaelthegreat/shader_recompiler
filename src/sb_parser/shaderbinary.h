#ifndef _GNM_SHADERBINARY_H_
#define _GNM_SHADERBINARY_H_

#include "shader.h"

constexpr uint32_t GNM_SHADER_FILE_HEADER_ID = 0x72646853;  // Shdr
#define GNM_SHADER_BINARY_INFO_MAGIC "OrbShdr"

enum GnmImgNumFormat {
    GNM_IMG_NUM_FORMAT_UNORM = 0x0,
    GNM_IMG_NUM_FORMAT_SNORM = 0x1,
    GNM_IMG_NUM_FORMAT_USCALED = 0x2,
    GNM_IMG_NUM_FORMAT_SSCALED = 0x3,
    GNM_IMG_NUM_FORMAT_UINT = 0x4,
    GNM_IMG_NUM_FORMAT_SINT = 0x5,
    GNM_IMG_NUM_FORMAT_SNORM_OGL = 0x6,
    GNM_IMG_NUM_FORMAT_FLOAT = 0x7,
    GNM_IMG_NUM_FORMAT_SRGB = 0x9,
    GNM_IMG_NUM_FORMAT_UBNORM = 0xa,
    GNM_IMG_NUM_FORMAT_UBNORM_OGL = 0xb,
    GNM_IMG_NUM_FORMAT_UBINT = 0xc,
    GNM_IMG_NUM_FORMAT_UBSCALED = 0xd,
};

enum GnmImageFormat {
    GNM_IMG_DATA_FORMAT_INVALID = 0x0,
    GNM_IMG_DATA_FORMAT_8 = 0x1,
    GNM_IMG_DATA_FORMAT_16 = 0x2,
    GNM_IMG_DATA_FORMAT_8_8 = 0x3,
    GNM_IMG_DATA_FORMAT_32 = 0x4,
    GNM_IMG_DATA_FORMAT_16_16 = 0x5,
    GNM_IMG_DATA_FORMAT_10_11_11 = 0x6,
    GNM_IMG_DATA_FORMAT_11_11_10 = 0x7,
    GNM_IMG_DATA_FORMAT_10_10_10_2 = 0x8,
    GNM_IMG_DATA_FORMAT_2_10_10_10 = 0x9,
    GNM_IMG_DATA_FORMAT_8_8_8_8 = 0xa,
    GNM_IMG_DATA_FORMAT_32_32 = 0xb,
    GNM_IMG_DATA_FORMAT_16_16_16_16 = 0xc,
    GNM_IMG_DATA_FORMAT_32_32_32 = 0xd,
    GNM_IMG_DATA_FORMAT_32_32_32_32 = 0xe,
    GNM_IMG_DATA_FORMAT_5_6_5 = 0x10,
    GNM_IMG_DATA_FORMAT_1_5_5_5 = 0x11,
    GNM_IMG_DATA_FORMAT_5_5_5_1 = 0x12,
    GNM_IMG_DATA_FORMAT_4_4_4_4 = 0x13,
    GNM_IMG_DATA_FORMAT_8_24 = 0x14,
    GNM_IMG_DATA_FORMAT_24_8 = 0x15,
    GNM_IMG_DATA_FORMAT_X24_8_32 = 0x16,
    GNM_IMG_DATA_FORMAT_GB_GR = 0x20,
    GNM_IMG_DATA_FORMAT_BG_RG = 0x21,
    GNM_IMG_DATA_FORMAT_5_9_9_9 = 0x22,
    GNM_IMG_DATA_FORMAT_BC1 = 0x23,
    GNM_IMG_DATA_FORMAT_BC2 = 0x24,
    GNM_IMG_DATA_FORMAT_BC3 = 0x25,
    GNM_IMG_DATA_FORMAT_BC4 = 0x26,
    GNM_IMG_DATA_FORMAT_BC5 = 0x27,
    GNM_IMG_DATA_FORMAT_BC6 = 0x28,
    GNM_IMG_DATA_FORMAT_BC7 = 0x29,
    GNM_IMG_DATA_FORMAT_FMASK8_S2_F1 = 0x2c,
    GNM_IMG_DATA_FORMAT_FMASK8_S4_F1 = 0x2d,
    GNM_IMG_DATA_FORMAT_FMASK8_S8_F1 = 0x2e,
    GNM_IMG_DATA_FORMAT_FMASK8_S2_F2 = 0x2f,
    GNM_IMG_DATA_FORMAT_FMASK8_S4_F2 = 0x30,
    GNM_IMG_DATA_FORMAT_FMASK8_S4_F4 = 0x31,
    GNM_IMG_DATA_FORMAT_FMASK16_S16_F1 = 0x32,
    GNM_IMG_DATA_FORMAT_FMASK16_S8_F2 = 0x33,
    GNM_IMG_DATA_FORMAT_FMASK32_S16_F2 = 0x34,
    GNM_IMG_DATA_FORMAT_FMASK32_S8_F4 = 0x35,
    GNM_IMG_DATA_FORMAT_FMASK32_S8_F8 = 0x36,
    GNM_IMG_DATA_FORMAT_FMASK64_S16_F4 = 0x37,
    GNM_IMG_DATA_FORMAT_FMASK64_S16_F8 = 0x38,
    GNM_IMG_DATA_FORMAT_4_4 = 0x39,
    GNM_IMG_DATA_FORMAT_6_5_5 = 0x3a,
    GNM_IMG_DATA_FORMAT_1 = 0x3b,
    GNM_IMG_DATA_FORMAT_1_REVERSED = 0x3c,
    GNM_IMG_DATA_FORMAT_32_AS_8 = 0x3d,
    GNM_IMG_DATA_FORMAT_32_AS_8_8 = 0x3e,
    GNM_IMG_DATA_FORMAT_32_AS_32_32_32_32 = 0x3f,
};

enum GnmShaderType : uint8_t {
	GNM_SHADER_INVALID = 0x0,
	GNM_SHADER_VERTEX = 0x1,
	GNM_SHADER_PIXEL = 0x2,
	GNM_SHADER_GEOMETRY = 0x3,
	GNM_SHADER_COMPUTE = 0x4,
	GNM_SHADER_EXPORT = 0x5,
	GNM_SHADER_LOCAL = 0x6,
	GNM_SHADER_HULL = 0x7,
};

enum GnmTargetGpuMode : uint8_t {
	GNM_TARGETGPUMODE_UNSPECIFIED = 0x0,
	GNM_TARGETGPUMODE_BASE = 0x1,
	GNM_TARGETGPUMODE_NEO = 0x2,
};

typedef struct {
	uint32_t magic;	 // GNM_SHADER_FILE_HEADER_ID
	uint16_t vermajor;
	uint16_t verminor;
    GnmShaderType type;  // GnmShaderType
	uint8_t headersizedwords;
	uint8_t auxdata;
    GnmTargetGpuMode targetgpumodes;	 // GnmTargetGpuMode
	uint32_t _unused;
} GnmShaderFileHeader;
static_assert(sizeof(GnmShaderFileHeader) == 0x10, "");

typedef struct {
	uint32_t shadersize : 23;
	uint32_t isusingsrt : 1;
	uint32_t numinputusageslots : 8;
	uint16_t embeddedconstantbufferdqwords;	 // 16 byte double qwords
	uint16_t scratchsizeperthreaddwords;
} GnmShaderCommonData;
static_assert(sizeof(GnmShaderCommonData) == 0x8, "");

static inline uint32_t gnmShaderCommonCodeSize(const GnmShaderCommonData* data
) {
	return data->shadersize + data->embeddedconstantbufferdqwords * 16;
}

enum GnmVsShaderGsMode {
	GNM_VSGSMODE_G = 0x1,

	GNM_VSGSMODE_G_ONCHIP = 0x2,
	GNM_VSGSMODE_G_ES_PASSTHRU = 0x4,
	GNM_VSGSMODE_G_ES_ELEMENTINFO = 0x8,
	GNM_VSGSMODE_G_CUTMODE_1024 = 0x0,
	GNM_VSGSMODE_G_CUTMODE_512 = 0x10,
	GNM_VSGSMODE_G_CUTMODE_256 = 0x20,
	GNM_VSGSMODE_G_CUTMODE_128 = 0x30,
	GNM_VSGSMODE_G_SUPRESSCUTS = 0x40,
	GNM_VSGSMODE_G_CUTMODE_MASK = 0x30,
	GNM_VSGSMODE_G_CUTMODE_SHIFT = 0x4,

	GNM_VSGSMODE_OFF = 0x0,
	GNM_VSGSMODE_A = 0x2,
	GNM_VSGSMODE_B = 0x4,
	GNM_VSGSMODE_C = 0x8,
	GNM_VSGSMODE_SPRITE_EN = 0xA,
	GNM_VSGSMODE_MASK = 0xE,
	GNM_VSGSMODE_CPACK = 0x10,
};

typedef struct {
	GnmShaderCommonData common;
	GnmVsStageRegisters registers;

	uint8_t numinputsemantics;
	uint8_t numexportsemantics;
	uint8_t gsmodeornuminputsemanticscs;  // GnmVsShaderGsMode
	uint8_t fetchcontrol;
} GnmVsShader;
static_assert(sizeof(GnmVsShader) == 0x28, "");

typedef struct {
	GnmShaderCommonData common;
	GnmPsStageRegisters registers;

	uint8_t numinputsemantics;
	uint8_t _unused[3];
} GnmPsShader;
static_assert(sizeof(GnmPsShader) == 0x3c, "");

struct GnmCsStageRegisters {
    uint32_t m_computePgmLo;
    uint32_t m_computePgmHi;
    uint32_t m_computePgmRsrc1;
    uint32_t m_computePgmRsrc2;
    uint32_t m_computeNumThreadX;
    uint32_t m_computeNumThreadY;
    uint32_t m_computeNumThreadZ;
};

struct GnmCsShader {
    GnmShaderCommonData common;
    GnmCsStageRegisters registers;

    uint8_t	m_orderedAppendMode					: 2;	///< The mode to use when assigning ordered append indices to the dispatched CS wavefronts. Please see Gnm::DispatchOrderedAppendMode.
    uint8_t m_numInputSemantics					: 6;	///< 0 if m_version==0 or [0:32] to indicate the number of entries in the input semantic table for CS. (v7.2: added from unused bits of m_orderedAppendMode)
    uint8_t m_dispatchDrawIndexDeallocNumBits	: 5;	///< 0 if not DispatchDraw or [1:31] for DispatchDraw. Match index value (<c>0xFFFF & (0xFFFF << m_dispatchDrawIndexDeallocNumBits)</c>) if 16-bit IRB or (<c>0xFFFFFFFF & (0xFFFFFFFF << m_dispatchDrawIndexDeallocNumBits)</c>) if 32-bit IRB.
    uint8_t m_reserved_b13_15					: 3;	///< Reserved, 0 if m_version <= 1. (v7.2: reserved from unused bits of <c><i>m_dispatchDrawIndexDeallocNumBits</i></c>)
    uint8_t m_version;									///< 0 if pre-versioning non-DispatchDraw or DispatchDraw IRB-only with DispatchDrawTriangleCullData format input data; 1 for first revised version, etc. (v7.2: added from reserved bits)
    uint8_t m_reserved_b24_31;							///< Reserved, 0 if m_version <= 1
};

typedef enum {
	GNM_SHB_PS = 0,
	GNM_SHB_VS_VS = 1,
	GNM_SHB_VS_ES = 2,
	GNM_SHB_VS_LS = 3,
	GNM_SHB_CS = 4,
	GNM_SHB_GS = 5,
	GNM_SHB_GS_VS = 6,
	GNM_SHB_HS = 7,
	GNM_SHB_DS_VS = 8,
	GNM_SHB_DS_ES = 9,
} GnmShaderBinaryType;

typedef struct {
	uint8_t signature[7];  // GNM_SHADER_BINARY_INFO_MAGIC
	uint8_t version;

	// if true, it's PSSL/CG. else it's IL/shtb
	uint32_t ispsslcg : 1;
	// is debugging source cached?
	uint32_t issourcecached : 1;
    GnmShaderBinaryType type : 4;	  // GnmShaderBinaryType
	uint32_t sourcetype : 2;  // ShaderSourceType
	// Shader code length
	uint32_t length : 24;

	// starts at ((uint32_t*)&ShaderBinaryInfo) - chunkusagebaseoffsetdwords
	uint8_t chunkusagebaseoffsetdwords;
	uint8_t numinputusageslots;
	uint8_t hassrt : 1;
	uint8_t hassrtusedvalidinfo : 1;
	uint8_t hasextendedusageinfo : 1;
	uint8_t _unused : 5;
	uint8_t _unused2;

	uint32_t shaderhash0;
	uint32_t shaderhash1;
	// CRC32 of whole shader until this field
	uint32_t crc32;
} GnmShaderBinaryInfo;
static_assert(sizeof(GnmShaderBinaryInfo) == 0x1c, "");

static inline int32_t gnmShaderInputUsageTypeSize(GnmShaderInputUsageType type
) {
	switch (type) {
	case GNM_SHINPUTUSAGE_IMM_RESOURCE:
	case GNM_SHINPUTUSAGE_IMM_RWRESOURCE:
	case GNM_SHINPUTUSAGE_IMM_SRT:
		return 0;
	case GNM_SHINPUTUSAGE_IMM_SAMPLER:
	case GNM_SHINPUTUSAGE_IMM_CONSTBUFFER:
	case GNM_SHINPUTUSAGE_IMM_VERTEXBUFFER:
		return 4;
	case GNM_SHINPUTUSAGE_IMM_ALUFLOATCONST:
	case GNM_SHINPUTUSAGE_IMM_ALUBOOL32CONST:
	case GNM_SHINPUTUSAGE_IMM_GDSCOUNTERRANGE:
	case GNM_SHINPUTUSAGE_IMM_GDSMEMORYRANGE:
	case GNM_SHINPUTUSAGE_IMM_GWSBASE:
	case GNM_SHINPUTUSAGE_IMM_LDSESGSSIZE:
		return 1;
	case GNM_SHINPUTUSAGE_SUBPTR_FETCHSHADER:
	case GNM_SHINPUTUSAGE_PTR_RESOURCETABLE:
	case GNM_SHINPUTUSAGE_PTR_INTERNALRESOURCETABLE:
	case GNM_SHINPUTUSAGE_PTR_SAMPLERTABLE:
	case GNM_SHINPUTUSAGE_PTR_CONSTBUFFERTABLE:
	case GNM_SHINPUTUSAGE_PTR_VERTEXBUFFERTABLE:
	case GNM_SHINPUTUSAGE_PTR_SOBUFFERTABLE:
	case GNM_SHINPUTUSAGE_PTR_RWRESOURCETABLE:
	case GNM_SHINPUTUSAGE_PTR_INTERNALGLOBALTABLE:
	case GNM_SHINPUTUSAGE_PTR_EXTENDEDUSERDATA:
	case GNM_SHINPUTUSAGE_PTR_INDIRECTRESOURCETABLE:
	case GNM_SHINPUTUSAGE_PTR_INDIRECTINTERNALRESOURCETABLE:
	case GNM_SHINPUTUSAGE_PTR_INDIRECTRWRESOURCETABLE:
		return 2;
	default:
		return -1;
	}
}

static inline const GnmShaderCommonData* gnmShfCommonData(
    const GnmShaderFileHeader* shf
) {
	if (!shf) {
		return 0;
	}
	const uint8_t* ptr = (const uint8_t*)shf + sizeof(GnmShaderFileHeader);
	return (const GnmShaderCommonData*)ptr;
}

static inline const GnmInputUsageSlot* gnmVsShaderInputUsageSlotTable(const GnmVsShader* vs) {
	return (const GnmInputUsageSlot*)((const uint8_t*)vs +
					  sizeof(GnmVsShader));
}
static inline const GnmVertexInputSemantic* gnmVsShaderInputSemanticTable(const GnmVsShader* vs) {
    const uint8_t* ptr = (const uint8_t*)vs + sizeof(GnmVsShader) +
                         vs->common.numinputusageslots * sizeof(GnmInputUsageSlot);
	return (const GnmVertexInputSemantic*)ptr;
}
static inline const GnmVertexExportSemantic* gnmVsShaderExportSemanticTable(const GnmVsShader* vs) {
	const uint8_t* ptr =
	    (const uint8_t*)vs + sizeof(GnmVsShader) +
	    vs->common.numinputusageslots * sizeof(GnmInputUsageSlot) +
	    vs->numinputsemantics * sizeof(GnmVertexInputSemantic);
	return (const GnmVertexExportSemantic*)ptr;
}
static inline uint32_t gnmVsShaderCalcSize(const GnmVsShader* vs) {
	const uint32_t size =
	    sizeof(GnmVsShader) +
	    sizeof(GnmInputUsageSlot) * vs->common.numinputusageslots +
	    sizeof(GnmVertexInputSemantic) * vs->numinputsemantics +
	    sizeof(GnmVertexExportSemantic) * vs->numexportsemantics;
	return (size + 3) & ~3;
}
static inline const void* gnmVsShaderCodePtr(const GnmVsShader* vs) {
	if (!vs) {
		return 0;
	}
	const uint32_t offset = vs->registers.spishaderpgmlovs;
	return (const uint8_t*)vs + offset;
}

static inline const GnmInputUsageSlot* gnmPsShaderInputUsageSlotTable(const GnmPsShader* ps) {
	return (const GnmInputUsageSlot*)((const uint8_t*)ps +
					  sizeof(GnmPsShader));
}
static inline const GnmPixelInputSemantic* gnmPsShaderInputSemanticTable(const GnmPsShader* ps) {
	const uint8_t* ptr =
	    (const uint8_t*)ps + sizeof(GnmPsShader) +
	    ps->common.numinputusageslots * sizeof(GnmInputUsageSlot);
	return (const GnmPixelInputSemantic*)ptr;
}
static inline uint32_t gnmPsShaderCalcSize(const GnmPsShader* ps) {
	const uint32_t size =
	    sizeof(GnmPsShader) +
	    sizeof(GnmInputUsageSlot) * ps->common.numinputusageslots +
	    sizeof(GnmPixelInputSemantic) * ps->numinputsemantics;
	return (size + 3) & ~3;
}
static inline const void* gnmPsShaderCodePtr(const GnmPsShader* ps) {
	if (!ps) {
		return 0;
	}
	const uint32_t offset = ps->registers.spishaderpgmlops;
	return (const uint8_t*)ps + offset;
}
static inline const void* gnmCsShaderCodePtr(const GnmCsShader* cs) {
    if (!cs) {
        return 0;
    }
    const uint32_t offset = cs->registers.m_computePgmLo;
    return (const uint8_t*)cs + offset;
}

#endif	// _GNM_SHADERBINARY_H_
