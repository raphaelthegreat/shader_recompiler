#ifndef _GNM_SHADER_H_
#define _GNM_SHADER_H_

#include "error.h"
#include <cstdint>

enum GnmShaderInputUsageType : uint8_t {
	GNM_SHINPUTUSAGE_IMM_RESOURCE = 0x0,
	GNM_SHINPUTUSAGE_IMM_SAMPLER = 0x1,
	GNM_SHINPUTUSAGE_IMM_CONSTBUFFER = 0x2,
	GNM_SHINPUTUSAGE_IMM_VERTEXBUFFER = 0x3,
	GNM_SHINPUTUSAGE_IMM_RWRESOURCE = 0x4,
	GNM_SHINPUTUSAGE_IMM_ALUFLOATCONST = 0x5,
	GNM_SHINPUTUSAGE_IMM_ALUBOOL32CONST = 0x6,
	GNM_SHINPUTUSAGE_IMM_GDSCOUNTERRANGE = 0x7,
	GNM_SHINPUTUSAGE_IMM_GDSMEMORYRANGE = 0x8,
	GNM_SHINPUTUSAGE_IMM_GWSBASE = 0x9,
	GNM_SHINPUTUSAGE_IMM_SRT = 0xa,
	GNM_SHINPUTUSAGE_IMM_LDSESGSSIZE = 0xd,
	GNM_SHINPUTUSAGE_SUBPTR_FETCHSHADER = 0x12,
	GNM_SHINPUTUSAGE_PTR_RESOURCETABLE = 0x13,
	GNM_SHINPUTUSAGE_PTR_INTERNALRESOURCETABLE = 0x14,
	GNM_SHINPUTUSAGE_PTR_SAMPLERTABLE = 0x15,
	GNM_SHINPUTUSAGE_PTR_CONSTBUFFERTABLE = 0x16,
	GNM_SHINPUTUSAGE_PTR_VERTEXBUFFERTABLE = 0x17,
	GNM_SHINPUTUSAGE_PTR_SOBUFFERTABLE = 0x18,
	GNM_SHINPUTUSAGE_PTR_RWRESOURCETABLE = 0x19,
	GNM_SHINPUTUSAGE_PTR_INTERNALGLOBALTABLE = 0x1a,
	GNM_SHINPUTUSAGE_PTR_EXTENDEDUSERDATA = 0x1b,
	GNM_SHINPUTUSAGE_PTR_INDIRECTRESOURCETABLE = 0x1c,
	GNM_SHINPUTUSAGE_PTR_INDIRECTINTERNALRESOURCETABLE = 0x1d,
	GNM_SHINPUTUSAGE_PTR_INDIRECTRWRESOURCETABLE = 0x1e,
};

typedef struct {
    GnmShaderInputUsageType usagetype;  // GnmShaderInputUsageType
	uint8_t apislot;
	uint8_t startregister;

	union {
		struct {
			uint8_t registercount : 1;
			uint8_t resourcetype : 1;
			uint8_t _unused : 2;
			uint8_t chunkmask : 4;
		};
		uint8_t srtdwordsminusone;
	};
} GnmInputUsageSlot;
_Static_assert(sizeof(GnmInputUsageSlot) == 0x4, "");

typedef struct {
	uint8_t semantic;
	uint8_t vgpr;
	uint8_t sizeinelements;
	uint8_t _unused;
} GnmVertexInputSemantic;
_Static_assert(sizeof(GnmVertexInputSemantic) == 0x4, "");

typedef struct {
	uint8_t semantic;
	uint8_t outindex : 5;
	uint8_t _unused : 1;
	uint8_t exportf16 : 2;
} GnmVertexExportSemantic;
_Static_assert(sizeof(GnmVertexExportSemantic) == 0x2, "");

typedef struct {
	uint32_t spishaderpgmlovs;
	uint32_t spishaderpgmhivs;

	uint32_t spishaderpgmrsrc1vs;
	uint32_t spishaderpgmrsrc2vs;

	uint32_t spivsoutconfig;
	uint32_t spishaderposformat;
	uint32_t paclvsoutcntl;
} GnmVsStageRegisters;
_Static_assert(sizeof(GnmVsStageRegisters) == 0x1c, "");

enum GnmPixelDefaultValue : uint16_t {
	GNM_PX_DEFVAL_NONE = 0,
	GNM_PX_DEFVAL_0_0_0_1 = 1,
	GNM_PX_DEFVAL_1_1_1_0 = 2,
	GNM_PX_DEFVAL_1_1_1_1 = 3,
};

typedef union {
	struct {
		uint16_t semantic : 8;
        GnmPixelDefaultValue defaultvalue : 2;  // GnmPixelDefaultValue
		uint16_t isflatshaded : 1;
		uint16_t islinear : 1;
		uint16_t iscustom : 1;
		uint16_t _unused : 3;
	};
	// NEO mode only
	struct {
		uint16_t : 12;
        GnmPixelDefaultValue defaultvaluehi : 2;  // GnmPixelDefaultValue
		// bitmask for each enabled f16 interp
		uint16_t interpf16 : 2;
	};
} GnmPixelInputSemantic;
_Static_assert(sizeof(GnmPixelInputSemantic) == 0x2, "");

typedef enum {
	GNM_PS_EXP_FMT_ZERO = 0x0,
	GNM_PS_EXP_FMT_32R = 0x1,
	GNM_PS_EXP_FMT_32GR = 0x2,
	GNM_PS_EXP_FMT_32AR = 0x3,
	GNM_PS_EXP_FMT_FP16_ABGR = 0x4,
	GNM_PS_EXP_FMT_UNORM16_ABGR = 0x5,
	GNM_PS_EXP_FMT_SNORM16_ABGR = 0x6,
	GNM_PS_EXP_FMT_UINT16_ABGR = 0x7,
	GNM_PS_EXP_FMT_SINT16_ABGR = 0x8,
	GNM_PS_EXP_FMT_32_ABGR = 0x9,
	GNM_PS_EXP_FMT_FP16_OR_32_OPT = 0x84,
	GNM_PS_EXP_FMT_32_OPT = 0x89,
} GnmPsExportFormat;

typedef struct {
	uint32_t spishaderpgmlops;
	uint32_t spishaderpgmhips;

	uint32_t spishaderpgmrsrc1ps;
	uint32_t spishaderpgmrsrc2ps;

	uint32_t spishaderzformat;
	uint32_t spishadercolformat;  // GnmPsExportFormat

	uint32_t spipsinputena;
	uint32_t spipsinputaddr;

	uint32_t spipsincontrol;
	uint32_t spibaryccntl;

	uint32_t dbshadercontrol;
	uint32_t cbshadermask;
} GnmPsStageRegisters;
_Static_assert(sizeof(GnmPsStageRegisters) == 0x30, "");

typedef enum {
	GNM_FETCH_FLAG_NONE = 0x0,
	GNM_FETCH_FLAG_SHIFT_OVERSTEP0 = 0x1,
} GnmFetchShaderFlags;

typedef enum {
	GNM_FETCH_MODE_VERTEXINDEX = 0,
	GNM_FETCH_MODE_INSTANCEID = 1,
	GNM_FETCH_MODE_INSTANCEID_OVERSTEPRATE0 = 2,
	GNM_FETCH_MODE_INSTANCEID_OVERSTEPRATE1 = 3,
} GnmFetchShaderInstancingMode;

static inline void gnmVsRegsSetAddress(GnmVsStageRegisters* regs, void* addr) {
	regs->spishaderpgmlovs = (uintptr_t)addr >> 8;
	regs->spishaderpgmhivs = (uintptr_t)addr >> 40;
}

static inline void gnmPsRegsSetAddress(GnmPsStageRegisters* regs, void* addr) {
	regs->spishaderpgmlops = (uintptr_t)addr >> 8;
	regs->spishaderpgmhips = (uintptr_t)addr >> 40;
}

typedef struct {
	const GnmVsStageRegisters* regs;
	GnmFetchShaderFlags flags;

	const GnmInputUsageSlot* inputusages;
	uint32_t numinputusages;

	const GnmVertexInputSemantic* vtxinputs;
	uint32_t numvtxinputs;

	const uint32_t* remaptable;
	uint32_t remaptablecount;

	const GnmFetchShaderInstancingMode* instancedata;
	uint32_t numinstancedata;

	uint8_t vertexbaseusgpr;
	uint8_t instancebaseusgpr;
} GnmFetchShaderCreateInfo;

typedef struct {
	uint32_t sgprs;
	uint32_t vgprcompcnt;
} GnmFetchShaderResults;

//GnmError gnmFetchShaderCalcSize(
//    uint32_t* outsize, const GnmFetchShaderCreateInfo* ci
//);
//GnmError gnmCreateFetchShader(
//    void* outdata, uint32_t outdatasize, const GnmFetchShaderCreateInfo* ci,
//    GnmFetchShaderResults* r
//);

void gnmVsRegsSetFetchShaderModifier(
    GnmVsStageRegisters* regs, const GnmFetchShaderResults* r
);

#endif	// _GNM_SHADER_H_
