#include <cstring>
#include "validate.h"
#include "types.h"

static PsslError validateplstage(const PsslPipelineStage* plstage, PsslShaderType shadertype) {
	switch (shadertype) {
	case PSSL_SHADER_VS:
		if (plstage->vertexvariant > PSSL_VSVAR_EXPORTONCHIP) {
			return PSSL_ERR_INVALID_PIPELINE_STAGE;
		}
		break;
	case PSSL_SHADER_GS:
		if (plstage->geometryvariant > PSSL_GSVAR_ONCHIP) {
			return PSSL_ERR_INVALID_PIPELINE_STAGE;
		}
		break;
	case PSSL_SHADER_HS:
		if (plstage->hullvariant > PSSL_HSVAR_DYNAMIC) {
			return PSSL_ERR_INVALID_PIPELINE_STAGE;
		}
		break;
	case PSSL_SHADER_DS:
		if (plstage->domainvariant > PSSL_DSVAR_EXPORT_ONCHIP) {
			return PSSL_ERR_INVALID_PIPELINE_STAGE;
		}
		break;
	default:
		break;
	}

	return PSSL_ERR_OK;
}

static PsslError validatesysattrs(const PsslSystemAttributes* attrs, PsslShaderType shadertype) {
	switch (shadertype) {
	case PSSL_SHADER_GS:
		if (attrs->gs.inputtype > PSSL_GSIO_ADJLINE ||
		    attrs->gs.outputtype > PSSL_GSIO_ADJLINE) {
			return PSSL_ERR_INVALID_SYSTEM_ATTRIBUTES;
		}
		break;
	case PSSL_SHADER_HS:
		if (attrs->hs.patchtype > PSSL_HSDSPATCH_QUAD ||
		    attrs->hs.outputtopologytype > PSSL_HSTOPOLOGY_CCWTRI ||
		    attrs->hs.partitioningtype >
			PSSL_HSPARTITIONING_EVENFACTORIAL) {
			return PSSL_ERR_INVALID_SYSTEM_ATTRIBUTES;
		}
		break;
	case PSSL_SHADER_DS:
		if (attrs->ds.patchtype > PSSL_HSDSPATCH_QUAD) {
			return PSSL_ERR_INVALID_SYSTEM_ATTRIBUTES;
		}
		break;
	default:
		break;
	}

	return PSSL_ERR_OK;
}

static PsslError validategnmshaderfile(const void* data, size_t datasize) {
	const size_t twoheadersize =
	    sizeof(PsslBinaryHeader) + sizeof(GnmShaderFileHeader);
	if (datasize < twoheadersize + sizeof(GnmShaderCommonData)) {
		return PSSL_ERR_TOO_SMALL;
	}

    const PsslBinaryHeader* hdr = (const PsslBinaryHeader*)data;
	const GnmShaderFileHeader* sfhdr = psslSbGnmShader(hdr);

	if (sfhdr->magic != GNM_SHADER_FILE_HEADER_ID) {
		return PSSL_ERR_INVALID_MAGIC;
	}
	if (sfhdr->type > PSSL_SHADER_DS) {
		return PSSL_ERR_INVALID_SHADER_TYPE;
	}

    const auto allpossiblegpu =
	    GNM_TARGETGPUMODE_BASE | GNM_TARGETGPUMODE_NEO;
	if (sfhdr->targetgpumodes > allpossiblegpu) {
		return PSSL_ERR_INVALID_SHADER_TYPE;
	}

	const size_t sfhdrsize = sfhdr->headersizedwords * sizeof(uint32_t);
	if (datasize < twoheadersize + sfhdrsize) {
		return PSSL_ERR_TOO_SMALL;
	}

	const GnmShaderCommonData* common = gnmShfCommonData(sfhdr);
	const GnmShaderBinaryInfo* bininfo = psslSbGnmShaderBinaryInfo(hdr);

	const void* shadercode = NULL;
	const uint32_t shadercodesize = bininfo->length;

	switch (sfhdr->type) {
    case GNM_SHADER_VERTEX: {
		if (datasize < twoheadersize + sizeof(GnmVsShader)) {
			return PSSL_ERR_TOO_SMALL;
		}

		const GnmVsShader* vsdata = (const GnmVsShader*)common;
		shadercode = gnmVsShaderCodePtr(vsdata);
		break;
    }
    case GNM_SHADER_PIXEL: {
		if (datasize < twoheadersize + sizeof(GnmPsShader)) {
			return PSSL_ERR_TOO_SMALL;
		}

		const GnmPsShader* psdata = (const GnmPsShader*)common;
		shadercode = gnmPsShaderCodePtr(psdata);
		break;
    }
    case GNM_SHADER_COMPUTE: {
        const GnmCsShader* csdata = (const GnmCsShader*)common;
        shadercode = gnmCsShaderCodePtr(csdata);
        break;
    }
	case GNM_SHADER_INVALID:
	default:
		// TODO: other types
		return PSSL_ERR_INVALID_SHADER_TYPE;
	}

	const size_t shadercodeoff =
	    (const uint8_t*)shadercode - (const uint8_t*)common;
	if (datasize < twoheadersize + shadercodeoff + shadercodesize) {
		return PSSL_ERR_TOO_SMALL;
	}

	return PSSL_ERR_OK;
}

static PsslError validategnmshaderbinaryinfo(
    const void* data, size_t datasize
) {
	// check if BinaryInfo header exists
    const PsslBinaryHeader* hdr = (const PsslBinaryHeader*)data;
	const size_t infooff = sizeof(PsslBinaryHeader) + hdr->codesize -
			       sizeof(GnmShaderBinaryInfo);
	if (datasize < infooff + sizeof(GnmShaderBinaryInfo)) {
		return PSSL_ERR_MISSING_BINARY_INFO;
	}

	const GnmShaderBinaryInfo* bininfo = psslSbGnmShaderBinaryInfo(hdr);

	for (uint8_t i = 0; i < sizeof(bininfo->signature); i += 1) {
		if (bininfo->signature[i] != GNM_SHADER_BINARY_INFO_MAGIC[i]) {
			return PSSL_ERR_INVALID_MAGIC;
		}
	}
	if (bininfo->type > GNM_SHB_DS_ES) {
		return PSSL_ERR_INVALID_SHADER_TYPE;
	}

	// check if ParamInfo exists
	const size_t paramoff = infooff + sizeof(GnmShaderBinaryInfo);
	if (datasize < paramoff + sizeof(PsslBinaryParamInfo)) {
		return PSSL_ERR_MISSING_BINARY_INFO;
	}

	const PsslBinaryParamInfo* paraminfo = psslSbParameterInfo(hdr);

	const uint32_t maxbinarysize =
	    sizeof(GnmShaderBinaryInfo) + sizeof(PsslBinaryParamInfo) +
	    sizeof(PsslBinaryBuffer) * paraminfo->numbufferresources +
	    sizeof(PsslBinaryConstant) * paraminfo->numconstants +
	    sizeof(PsslBinaryElement) * paraminfo->numvariables +
	    sizeof(PsslBinarySamplerState) * paraminfo->numsamplerresources +
	    sizeof(PsslBinaryAttribute) * paraminfo->numinputattributes +
	    sizeof(PsslBinaryAttribute) * paraminfo->numoutputattributes +
	    paraminfo->valuetablesize;
	if (maxbinarysize > datasize) {
		return PSSL_ERR_TOO_SMALL;
	}

	// validate string table's bounds
	const void* dataend = (const uint8_t*)data + datasize;
	const char* strtable = psslParamStringTable(paraminfo);
	const void* strtableend =
	    (const void*)&strtable[paraminfo->stringtablesize];
	if (strtableend > dataend) {
		return PSSL_ERR_TOO_SMALL;
	}

	// validate attributes' names
	for (uint32_t i = 0; i < paraminfo->numinputattributes; i += 1) {
		const PsslBinaryAttribute* inattr =
		    psslParamInputAttr(paraminfo, i);

		const char* name = psslAttrSemanticName(inattr);
		if ((const void*)name >= strtableend) {
			return PSSL_ERR_TOO_SMALL;
		}
		const size_t maxnamesize =
		    (const uint8_t*)strtableend - (const uint8_t*)name;
		if (strlen(name) > maxnamesize) {
			return PSSL_ERR_TOO_SMALL;
		}
	}

	return PSSL_ERR_OK;
}

PsslError psslValidateShaderBinary(const void* data, size_t datasize) {
	if (datasize < sizeof(PsslBinaryHeader)) {
		return PSSL_ERR_TOO_SMALL;
	}

    const PsslBinaryHeader* hdr = (const PsslBinaryHeader*)data;

	switch (hdr->shadertype) {
	case PSSL_SHADER_VS:
	case PSSL_SHADER_FS:
	case PSSL_SHADER_CS:
	case PSSL_SHADER_GS:
	case PSSL_SHADER_HS:
	case PSSL_SHADER_DS:
		break;
	default:
		return PSSL_ERR_INVALID_SHADER_TYPE;
	}

	switch (hdr->codetype) {
	case PSSL_CODE_IL:
	case PSSL_CODE_ISA:
	case PSSL_CODE_SCU:
		break;
	default:
		return PSSL_ERR_INVALID_CODE_TYPE;
	}

	const uint8_t comptype = hdr->compilertype & 0xf;
	switch (comptype) {
	case PSSL_COMPILER_UNSPECIFIED:
	case PSSL_COMPILER_ORBIS_PSSLC:
	case PSSL_COMPILER_ORBIS_ESSLC:
	case PSSL_COMPILER_ORBIS_WAVE:
	case PSSL_COMPILER_ORBIS_CU_AS:
		break;
	default:
		return PSSL_ERR_INVALID_COMPILER_TYPE;
	}

	PsslError perr = validateplstage(&hdr->stageinfo, hdr->shadertype);
	if (perr != PSSL_ERR_OK) {
		return perr;
	}
	perr = validatesysattrs(&hdr->sysattributeinfo, hdr->shadertype);
	if (perr != PSSL_ERR_OK) {
		return perr;
	}
	// validate GnmShaderBinaryInfo first since
	// we need it to validate GnmShader
	perr = validategnmshaderbinaryinfo(data, datasize);
	if (perr != PSSL_ERR_OK) {
		return perr;
	}
	perr = validategnmshaderfile(data, datasize);
	if (perr != PSSL_ERR_OK) {
		return perr;
	}

	return PSSL_ERR_OK;
}
