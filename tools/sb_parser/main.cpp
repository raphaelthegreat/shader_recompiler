#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>
#include <fmt/core.h>
#include <getopt.h>

#include "common/assert.h"
#include "frontend/control_flow_graph.h"
#include "frontend/decode.h"
#include "frontend/structured_control_flow.h"
#include "ir/abstract_syntax_list.h"
#include "ir/basic_block.h"
#include "ir/post_order.h"
#include "object_pool.h"

#include "shaderbinary.h"
#include "strings.h"
#include "types.h"
#include "validate.h"

namespace fs = std::filesystem;

namespace Shader::Optimization {
void SsaRewritePass(IR::BlockList& program);
void IdentityRemovalPass(IR::BlockList& program);
} // namespace Shader::Optimization

struct ShaderBinaryInfo {
    uint8_t m_signature[7]; // 'OrbShdr'
    uint8_t m_version;      // ShaderBinaryInfoVersion

    unsigned int m_pssl_or_cg : 1; // 1 = PSSL / Cg, 0 = IL / shtb
    unsigned int m_cached : 1;     // 1 = when compile, debugging source was cached.  May only make
                                   // sense for PSSL=1
    uint32_t m_type : 4;           // See enum ShaderBinaryType
    uint32_t m_source_type : 2;    // See enum ShaderSourceType
    unsigned int m_length : 24; // Binary code length (does not include this structure or any of its
                                // preceding associated tables)

    uint8_t m_chunkUsageBaseOffsetInDW; // in DW, which starts at ((uint32_t*)&ShaderBinaryInfo) -
                                        // m_chunkUsageBaseOffsetInDW; max is currently 7 dwords
                                        // (128 T# + 32 V# + 20 CB V# + 16 UAV T#/V#)
    uint8_t m_numInputUsageSlots; // Up to 16 user data reg slots + 128 extended user data dwords
                                  // supported by CUE; up to 16 user data reg slots + 240 extended
                                  // user data dwords supported by Gnm::InputUsageSlot
    uint8_t m_isSrt : 1; // 1 if this shader uses shader resource tables and has an SrtDef table
                         // embedded below the input usage table and any extended usage info
    uint8_t m_isSrtUsedInfoValid : 1;  // 1 if SrtDef::m_isUsed=0 indicates an element is definitely
                                       // unused; 0 if SrtDef::m_isUsed=0 indicates only that the
                                       // element is not known to be used (m_isUsed=1 always
                                       // indicates a resource is known to be used)
    uint8_t m_isExtendedUsageInfo : 1; // 1 if this shader has extended usage info for the
                                       // InputUsage table embedded below the input usage table
    uint8_t m_reserved2 : 5;           // For future use
    uint8_t m_reserved3;               // For future use

    uint32_t m_shaderHash0; // Association hash first 4 bytes
    uint32_t m_shaderHash1; // Association hash second 4 bytes
    uint32_t m_crc32;       // crc32 of shader + this struct, just up till this field
};

Shader::IR::BlockList GenerateBlocks(const Shader::IR::AbstractSyntaxList& syntax_list) {
    size_t num_syntax_blocks{};
    for (const auto& node : syntax_list) {
        if (node.type == Shader::IR::AbstractSyntaxNode::Type::Block) {
            ++num_syntax_blocks;
        }
    }
    Shader::IR::BlockList blocks;
    blocks.reserve(num_syntax_blocks);
    for (const auto& node : syntax_list) {
        if (node.type == Shader::IR::AbstractSyntaxNode::Type::Block) {
            blocks.push_back(node.data.block);
        }
    }
    return blocks;
}

static void parseshadercode(const u8* code, uint32_t codesize) {
    std::ofstream out("shader.bin", std::ios::binary);
    out.write((const char*)code, codesize);
    out.close();

    puts("\nInstructions:");
    puts("--------------------------------------");

    const u32* token = reinterpret_cast<const u32*>(code);
    const u32 tokenMovVccHi = 0xBEEB03FF;
    ASSERT_MSG(token[0] == tokenMovVccHi, "first instruction is not s_mov_b32 vcc_hi, #imm");
    auto* bininfo = reinterpret_cast<const ShaderBinaryInfo*>(token + (token[1] + 1) * 2);

    const u32* start = reinterpret_cast<const u32*>(code);
    const u32* end = reinterpret_cast<const u32*>(code + codesize);
    Shader::Gcn::GcnCodeSlice slice(start, end);
    std::vector<Shader::Gcn::GcnInst> insList;
    Shader::Gcn::GcnDecodeContext decoder;

    // Decode and save instructions
    insList.reserve(bininfo->m_length / sizeof(u32));
    while (!slice.atEnd()) {
        insList.emplace_back(decoder.decodeInstruction(slice));
    }

    Shader::ObjectPool<Shader::Gcn::Block> block_pool{64};
    Shader::ObjectPool<Shader::IR::Block> blk_pool{64};
    Shader::ObjectPool<Shader::IR::Inst> inst_pool{64};
    Shader::Gcn::CFG cfg{block_pool, insList};
    fmt::print("{}\n\n\n", cfg.Dot());
    const auto ret = Shader::Gcn::BuildASL(inst_pool, blk_pool, cfg);
    auto blocks = Shader::IR::PostOrder(ret.front());
    auto block = GenerateBlocks(ret);
    Shader::Optimization::SsaRewritePass(blocks);
    Shader::Optimization::IdentityRemovalPass(block);
    for (auto& blk : block) {
        fmt::print("{}\n\n", Shader::IR::DumpBlock(*blk));
    }

    while (0) {
        // ctx.decodeInstruction(slice);
        // const auto instr = ctx.getInstruction();

        char strinstr[256] = {0};

        /*gerr = gcnFormatInstruction(&instr, strinstr, sizeof(strinstr));
        if (gerr != GCN_ERR_OK) {
            printf(
                "Failed to format instruction with: %s\n",
                gcnStrError(gerr)
            );
            return;
        }

        printf("%08x: %s\n", instr.offset, strinstr);*/
    }
}

static inline void printinputslot(const GnmInputUsageSlot* slot) {
    printf("Usage type: %s (%u)\n", gnmStrShaderInputUsageType(slot->usagetype), slot->usagetype);
    printf("API slot: %u\n", slot->apislot);
    printf("Start register: %u\n", slot->startregister);

    if (slot->usagetype == GNM_SHINPUTUSAGE_IMM_SRT) {
        printf("SRT register count: %u DWORDs\n", slot->srtdwordsminusone + 1);
    } else {
        const char* restype = NULL;
        if (slot->usagetype == GNM_SHINPUTUSAGE_IMM_SRT) {
            restype = "SRT";
        } else if (slot->usagetype < GNM_SHINPUTUSAGE_SUBPTR_FETCHSHADER) {
            if (slot->resourcetype == 1) {
                restype = "T#";
            } else {
                restype = "V#";
            }
        } else {
            restype = "Pointer";
        }

        printf("Register count: %u\n", slot->registercount);
        printf("Resource type: %s (%u)\n", restype, slot->resourcetype);
        printf("Chunk mask: %u\n", slot->chunkmask);
    }

    char isareg[32] = {0};
    const uint8_t inputsize = slot->registercount ? 8 : 4;
    snprintf(isareg, sizeof(isareg), "s[%u:%u]", slot->startregister,
             slot->startregister + inputsize - 1);

    printf("(default ISA register string): %s\n", isareg);
}

static inline void printgnmheadervs(const GnmVsShader* vsdata) {
    puts("\nGNM vertex shader:");
    printf("Shader size: %u\n", vsdata->common.shadersize);
    printf("Constant buffer size DQWORDs: %u\n", vsdata->common.embeddedconstantbufferdqwords);
    printf("Scratch size DWORDs: %u\n", vsdata->common.scratchsizeperthreaddwords);
    printf("Num input semantics: %u\n", vsdata->numinputsemantics);
    printf("Num export semantics: %u\n", vsdata->numexportsemantics);
    printf("GS mode: %u\n", vsdata->gsmodeornuminputsemanticscs);
    printf("Fetch control: %u\n", vsdata->fetchcontrol);

    puts("\nGNM vertex stage registers:");
    printf("SPI Shader Program Low: %u (0x%08x)\n", vsdata->registers.spishaderpgmlovs,
           vsdata->registers.spishaderpgmlovs);
    printf("SPI Shader Program High: %u (0x%08x)\n", vsdata->registers.spishaderpgmhivs,
           vsdata->registers.spishaderpgmhivs);
    printf("SPI Shader Resource 1: %u (0x%08x)\n", vsdata->registers.spishaderpgmrsrc1vs,
           vsdata->registers.spishaderpgmrsrc1vs);
    printf("SPI Shader Resource 2: %u (0x%08x)\n", vsdata->registers.spishaderpgmrsrc2vs,
           vsdata->registers.spishaderpgmrsrc2vs);
    printf("SPI VS Out Config: %u (0x%08x)\n", vsdata->registers.spivsoutconfig,
           vsdata->registers.spivsoutconfig);
    printf("SPI Shader position format: %u (0x%08x)\n", vsdata->registers.spishaderposformat,
           vsdata->registers.spishaderposformat);
    printf("PA Cl VS Out Control: %u (0x%08x)\n", vsdata->registers.paclvsoutcntl,
           vsdata->registers.paclvsoutcntl);

    const GnmInputUsageSlot* inputslots = gnmVsShaderInputUsageSlotTable(vsdata);
    for (uint8_t i = 0; i < vsdata->common.numinputusageslots; i += 1) {
        const GnmInputUsageSlot* slot = &inputslots[i];
        printf("\nGnm input usage slot %u:\n", i);
        printinputslot(slot);
    }

    const GnmVertexInputSemantic* inputstable = gnmVsShaderInputSemanticTable(vsdata);
    for (uint8_t i = 0; i < vsdata->numinputsemantics; i += 1) {
        const GnmVertexInputSemantic* input = &inputstable[i];
        printf("\nGnm vertex input semantic %u:\n", i);
        printf("Semantic: %u\n", input->semantic);
        printf("VGPR: %u\n", input->vgpr);
        printf("Size in elements: %u\n", input->sizeinelements);
    }

    const GnmVertexExportSemantic* exportstable = gnmVsShaderExportSemanticTable(vsdata);
    for (uint8_t i = 0; i < vsdata->numexportsemantics; i += 1) {
        const GnmVertexExportSemantic* exp = &exportstable[i];
        printf("\nGnm vertex export semantic %u:\n", i);
        printf("Semantic: %u\n", exp->semantic);
        printf("Output index: %u\n", exp->outindex);
        printf("Export F16: 0x%x\n", exp->exportf16);
    }
}

static inline void printgnmheaderps(const GnmPsShader* psdata) {
    puts("\nGNM pixel shader:");
    printf("Shader size: %u\n", psdata->common.shadersize);
    printf("Constant buffer size DQWORDs: %u\n", psdata->common.embeddedconstantbufferdqwords);
    printf("Scratch size DWORDs: %u\n", psdata->common.scratchsizeperthreaddwords);
    printf("Num input semantics: %u\n", psdata->numinputsemantics);

    puts("\nGNM pixel stage registers:");
    printf("SPI Shader Program Low: %u (0x%08x)\n", psdata->registers.spishaderpgmlops,
           psdata->registers.spishaderpgmlops);
    printf("SPI Shader Program High: %u (0x%08x)\n", psdata->registers.spishaderpgmhips,
           psdata->registers.spishaderpgmhips);
    printf("SPI Shader Resource 1: %u (0x%08x)\n", psdata->registers.spishaderpgmrsrc1ps,
           psdata->registers.spishaderpgmrsrc1ps);
    printf("SPI Shader Resource 2: %u (0x%08x)\n", psdata->registers.spishaderpgmrsrc2ps,
           psdata->registers.spishaderpgmrsrc2ps);
    printf("SPI Shader Z format: %u (0x%08x)\n", psdata->registers.spishaderzformat,
           psdata->registers.spishaderzformat);
    printf("SPI Shader color format: %u (0x%08x)\n", psdata->registers.spishadercolformat,
           psdata->registers.spishadercolformat);
    printf("SPI Shader input enable: %u (0x%08x)\n", psdata->registers.spipsinputena,
           psdata->registers.spipsinputena);
    printf("SPI Shader input address: %u (0x%08x)\n", psdata->registers.spipsinputaddr,
           psdata->registers.spipsinputaddr);
    printf("SPI Shader input control: %u (0x%08x)\n", psdata->registers.spipsincontrol,
           psdata->registers.spipsincontrol);
    printf("SPI barycentric control: %u (0x%08x)\n", psdata->registers.spibaryccntl,
           psdata->registers.spibaryccntl);
    printf("DB shader control: %u (0x%08x)\n", psdata->registers.dbshadercontrol,
           psdata->registers.dbshadercontrol);
    printf("CB shader mask: %u (0x%08x)\n", psdata->registers.cbshadermask,
           psdata->registers.cbshadermask);

    const GnmInputUsageSlot* inputslots = gnmPsShaderInputUsageSlotTable(psdata);
    for (uint8_t i = 0; i < psdata->common.numinputusageslots; i += 1) {
        const GnmInputUsageSlot* slot = &inputslots[i];
        printf("\nGnm input usage slot %u:\n", i);
        printinputslot(slot);
    }

    const GnmPixelInputSemantic* inputstable = gnmPsShaderInputSemanticTable(psdata);
    for (uint8_t i = 0; i < psdata->numinputsemantics; i += 1) {
        const GnmPixelInputSemantic* input = &inputstable[i];
        printf("\nGnm pixel input semantic %u:\n", i);
        printf("Semantic: %u\n", input->semantic);

        if (input->interpf16 != 0) {
            printf("Default value f16_lo: %s (%u)\n", gnmStrPixelDefaultValue(input->defaultvalue),
                   input->defaultvalue);
        } else {
            printf("Default value: %s (%u)\n", gnmStrPixelDefaultValue(input->defaultvalue),
                   input->defaultvalue);
        }

        printf("Is flat shaded: %s\n", input->isflatshaded ? "true" : "false");
        printf("Is linear: %s\n", input->islinear ? "true" : "false");

        if (input->interpf16 != 0) {
            printf("Interpolate F16: 0x%x\n", input->interpf16);
            printf("Default value f16_hi: %s (%u)\n",
                   gnmStrPixelDefaultValue(input->defaultvaluehi), input->defaultvaluehi);
        } else {
            printf("Is custom: %s\n", input->iscustom ? "true" : "false");
        }
    }
}

static inline void printgnmheadercs(const GnmCsShader* csdata) {
    puts("\nGNM compute shader:");
    printf("Shader size: %u\n", csdata->common.shadersize);
    printf("Constant buffer size DQWORDs: %u\n", csdata->common.embeddedconstantbufferdqwords);
    printf("Scratch size DWORDs: %u\n", csdata->common.scratchsizeperthreaddwords);
    puts("\nGNM compute stage registers:");
    printf("SPI Shader Program Low: %u (0x%08x)\n", csdata->registers.m_computePgmLo,
           csdata->registers.m_computePgmLo);
    printf("SPI Shader Program High: %u (0x%08x)\n", csdata->registers.m_computePgmHi,
           csdata->registers.m_computePgmHi);
    printf("SPI Shader Resource 1: %u (0x%08x)\n", csdata->registers.m_computePgmRsrc1,
           csdata->registers.m_computePgmRsrc1);
    printf("SPI Shader Resource 2: %u (0x%08x)\n", csdata->registers.m_computePgmRsrc2,
           csdata->registers.m_computePgmRsrc2);
    printf("SPI Shader Num Threads X: %u\n", csdata->registers.m_computeNumThreadX);
    printf("SPI Shader Num Threads Y: %u\n", csdata->registers.m_computeNumThreadY);
    printf("SPI Shader Num Threads Z: %u\n", csdata->registers.m_computeNumThreadZ);
}

static void printgnmshaderfile(const PsslBinaryHeader* hdr) {
    const GnmShaderFileHeader* sfhdr = psslSbGnmShader(hdr);

    puts("\nGNM Header:");
    puts("--------------------------------------");

    GnmShaderType shtype = sfhdr->type;
    GnmTargetGpuMode gpumodes = sfhdr->targetgpumodes;

    printf("Magic: 0x%x\n", sfhdr->magic);
    printf("Version: %u.%u\n", sfhdr->vermajor, sfhdr->verminor);
    printf("Shader type: %s\n", gnmStrShaderType(shtype));
    printf("Header size (DWORDs): %u\n", sfhdr->headersizedwords);
    printf("Auxiliary data: %u\n", sfhdr->auxdata);
    printf("Target GPU mode: %s\n", gnmStrTargetGpuMode(gpumodes));

    const GnmShaderCommonData* common = gnmShfCommonData(sfhdr);

    printf("Shader size: %u\n", common->shadersize);
    printf("Is using SRT: %s\n", common->isusingsrt ? "true" : "false");
    printf("Num input usage slots: %u\n", common->numinputusageslots);
    printf("Embedded constant buffer (DQWORDS): %u\n", common->embeddedconstantbufferdqwords);
    printf("Scratch size per thread (DWORDS): %u\n", common->scratchsizeperthreaddwords);

    const GnmShaderBinaryInfo* bininfo = psslSbGnmShaderBinaryInfo(hdr);
    const void* shadercode = NULL;
    const uint32_t shadercodesize = bininfo->length;

    switch (shtype) {
    case GNM_SHADER_VERTEX: {
        const GnmVsShader* vsdata = (const GnmVsShader*)common;
        shadercode = gnmVsShaderCodePtr(vsdata);
        printgnmheadervs(vsdata);
        break;
    }
    case GNM_SHADER_PIXEL: {
        const GnmPsShader* psdata = (const GnmPsShader*)common;
        shadercode = gnmPsShaderCodePtr(psdata);
        printgnmheaderps(psdata);
        break;
    }
    case GNM_SHADER_COMPUTE: {
        const GnmCsShader* csdata = (const GnmCsShader*)common;
        shadercode = gnmCsShaderCodePtr(csdata);
        printgnmheadercs(csdata);
        break;
    }
    case GNM_SHADER_INVALID:
        assert(0);
    default:
        // TODO: other types
        printf("TODO: get this shader type data");
        std::exit(0);
    }

    const uint32_t shadercodeoff = (const uint8_t*)shadercode - (const uint8_t*)hdr;

    printf("\nCode length %u at offset %u", shadercodesize, shadercodeoff);
    parseshadercode((const u8*)shadercode, shadercodesize);
}

static void printgnmshaderbininfo(const PsslBinaryHeader* hdr) {
    const GnmShaderBinaryInfo* bininfo = psslSbGnmShaderBinaryInfo(hdr);

    puts("\nCode section:");
    puts("--------------------------------------");

    printf("ShaderBinary version: %u\n", bininfo->version);
    printf("Is PSSL/CG: %s\n", bininfo->ispsslcg ? "true" : "false");
    printf("Is source cached: %s\n", bininfo->issourcecached ? "true" : "false");
    printf("Shader type: %s\n", gnmStrShaderBinaryType(bininfo->type));
    printf("Shader code length: %u\n", bininfo->length);
    printf("Num input usage slots: %u\n", bininfo->numinputusageslots);
    printf("Has Shader Resource Tables (SRT): %s\n", bininfo->hassrt ? "true" : "false");
    printf("Has SRT used valid info: %s\n", bininfo->hassrtusedvalidinfo ? "true" : "false");
    printf("Has Extended Usage Info: %s\n", bininfo->hasextendedusageinfo ? "true" : "false");
    printf("Source hash type: %u\n", bininfo->sourcetype);
    printf("Association hash: 0x%08x 0x%08x\n", bininfo->shaderhash0, bininfo->shaderhash1);
    printf("CRC32: 0x%08x\n", bininfo->crc32);

    // Get usage masks and input usage slots
    u32 const* usageMasks =
        (u32 const*)((u8 const*)bininfo - bininfo->chunkusagebaseoffsetdwords * 4);
    InputUsageSlot const* inputUsageSlots =
        (InputUsageSlot const*)usageMasks - bininfo->numinputusageslots;
    printf("Resource Table List:\n");
    for (int i = 0; i < bininfo->numinputusageslots; i++) {
        printf("\nResource %d\n", i);
        printf("Usage Type: %d\n", inputUsageSlots[i].m_usageType);
        printf("Api Slot: %d\n", inputUsageSlots[i].m_apiSlot);
        printf("Start Sgpr: %d\n", inputUsageSlots[i].m_startRegister);
    }
}

static inline void printshaderattribute(const PsslBinaryAttribute* attr) {
    printf("Name: %s\n", psslAttrName(attr));
    printf("Semantic name: %s\n", psslAttrSemanticName(attr));
    printf("Type: %s\n", psslStrType(attr->type));
    printf("Semantic: %s\n", psslStrSemantic(attr->semantic));
    printf("Semantic index: %u\n", attr->semanticindex);
    printf("Resource index: %u\n", attr->resourceindex);
    printf("Interpolation type: %s\n", psslStrFragmentInterpType(attr->interptype));
    printf("Stream number: %u\n", attr->streamnumber);
}

static void printshaderparaminfo(const PsslBinaryHeader* hdr) {
    const PsslBinaryParamInfo* paraminfo = psslSbParameterInfo(hdr);

    puts("\nSB Parameter Information:");
    puts("--------------------------------------");

    printf("Num of buffer resources: %u\n", paraminfo->numbufferresources);
    printf("Num of constants: %u\n", paraminfo->numconstants);
    printf("Num of variables (elements) resources: %u\n", paraminfo->numvariables);
    printf("Num of sampler resources: %u\n", paraminfo->numsamplerresources);
    printf("Num of stream out: %u\n", paraminfo->numstreamout);
    printf("Size of value table: %u\n", paraminfo->valuetablesize);
    printf("Size of string table: %u\n", paraminfo->stringtablesize);

    printf("\n%u Buffer resources:\n", paraminfo->numbufferresources);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numbufferresources; i += 1) {
        const PsslBinaryBuffer* buf = psslParamBuffer(paraminfo, i);
        printf("\nBuffer %u of %u\n", i + 1, paraminfo->numbufferresources);
        printf("Name: %s\n", psslBufferName(buf));
        printf("Resource index: %u\n", buf->resourceidx);
        printf("Stride size: %u\n", buf->stridesize);
        const std::string_view type = psslStrBufferType(buf->buffertype);
        if (type.ends_with("buffer")) {
            // meta.dfmt = buf->
            // meta.nfmt = std::bit_cast<BufferChannelType>(vsharp->num_format.Value());
            // meta.isSwizzle = false;
            // meta.indexStride = indexStrideTable[vsharp->index_stride];
            // meta.elementSize = elementSizeTable[vsharp->element_size];
            // metas[0].vs.bufferInfos[res.startRegister] = meta;
            // meta.cs.bufferInfos[num_buffers++] = bmeta;
        }
        printf("Buffer type: %s\n", type.data());
        printf("Internal type: %s\n", psslStrInternalBufferType(buf->internaltype));
        printf("Type: %s\n", psslStrType(buf->type));
        printf("Num of sub elements: %u\n", buf->numelems);
        printf("Sub elements offset: %u\n", buf->elementoff);
    }

    printf("\n%u Constants:\n", paraminfo->numconstants);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numconstants; i += 1) {
        const PsslBinaryConstant* cnst = psslParamConstant(paraminfo, i);
        printf("\nConstant %u of %u\n", i + 1, paraminfo->numconstants);
        printf("Name: %s\n", psslConstantName(cnst));
        printf("Element offset: %u\n", cnst->elementoffset);
        printf("Register index: %u\n", cnst->registeridx);
        printf("Buffer type: %s\n", psslStrBufferType(cnst->buffertype));
        printf("Element type: %s\n", psslStrType(cnst->elementtype));
        printf("Default values (f32): %f %f %f %f\n", cnst->defaultf32[0], cnst->defaultf32[1],
               cnst->defaultf32[2], cnst->defaultf32[3]);
        printf("Default values (u32): %u %u %u %u\n", cnst->defaultu32[0], cnst->defaultu32[1],
               cnst->defaultu32[2], cnst->defaultu32[3]);
    }

    printf("\n%u Variables (Elements):\n", paraminfo->numvariables);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numvariables; i += 1) {
        const PsslBinaryElement* elem = psslParamVariable(paraminfo, i);
        printf("\nVariable %u of %u\n", i + 1, paraminfo->numvariables);
        printf("Name: %s\n", psslElementName(elem));
        printf("Type name: %s\n", psslElementTypeName(elem));
        printf("Is used: %s\n", elem->isused ? "true" : "false");
        printf("Is row major: %s\n", elem->isrowmajor ? "true" : "false");
        printf("Is pointer: %s\n", elem->ispointer ? "true" : "false");
        printf("Element type: %s\n", psslStrElementType(elem->elementtype));
        printf("Byte offset: %u\n", elem->byteoffset);
        printf("Size: %u\n", elem->size);
        printf("Array size: %u\n", elem->arraysize);
        printf("Default value offset OR resource index: %i\n", elem->defaultvalueoff);
        printf("Num elements: %u\n", elem->numelements);
        printf("Element offset: %i\n", elem->elementoffset);
    }

    printf("\n%u Sampler resources:\n", paraminfo->numsamplerresources);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numsamplerresources; i += 1) {
        const PsslBinarySamplerState* sampler = psslParamSampler(paraminfo, i);
        printf("\nSampler %u of %u\n", i + 1, paraminfo->numsamplerresources);
        printf("Name: %s\n", psslSamplerStateName(sampler));
        printf("Resource index: %u\n", sampler->resourceidx);
    }

    printf("\n%u Input attributes:\n", paraminfo->numinputattributes);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numinputattributes; i += 1) {
        const PsslBinaryAttribute* attr = psslParamInputAttr(paraminfo, i);
        printf("\nAttribute %u of %u\n", i + 1, paraminfo->numinputattributes);
        printf("attr offset: %zu\n", ((const uint8_t*)attr - (const uint8_t*)hdr));
        printshaderattribute(attr);
    }

    printf("\n%u Output attributes:\n", paraminfo->numoutputattributes);
    puts("--------------------------------------");
    for (uint32_t i = 0; i < paraminfo->numoutputattributes; i += 1) {
        const PsslBinaryAttribute* attr = psslParamOutputAttr(paraminfo, i);
        printf("\nAttribute %u of %u\n", i + 1, paraminfo->numoutputattributes);
        printshaderattribute(attr);
    }
}

static void printshaderinfo(const void* data) {
    const PsslBinaryHeader* hdr = (const PsslBinaryHeader*)data;

    puts("SB Header:");
    puts("--------------------------------------");
    printf("Version: %u.%u (revision %u)\n", hdr->vermajor, hdr->verminor, hdr->compiler_revision);
    printf("Association hash: 0x%x 0x%x\n", hdr->association_hash0, hdr->association_hash1);

    PsslShaderType shtype = hdr->shadertype;
    PsslCodeType codetype = hdr->codetype;
    PsslCompilerType comptype = psslSbCompilerType(hdr);

    printf("Shader type: %s\n", psslStrShaderType(shtype));
    printf("Code type: %s\n", psslStrCodeType(codetype));
    printf("Uses SRT: %s\n", hdr->uses_srt ? "true" : "false");
    printf("Compiler type: %s\n", psslStrCompilerType(comptype));
    printf("Code size: %u\n", hdr->codesize);

    switch (shtype) {
    case PSSL_SHADER_VS: {
        PsslVertexVariant vvar = hdr->stageinfo.vertexvariant;
        printf("Vertex variant: %s\n", psslStrVertexVariant(vvar));
        break;
    }
    case PSSL_SHADER_GS: {
        PsslGeometryVariant gvar = hdr->stageinfo.geometryvariant;
        printf("Geometry variant: %s\n", psslStrGeometryVariant(gvar));
        break;
    }
    case PSSL_SHADER_HS: {
        PsslHullVariant hvar = hdr->stageinfo.hullvariant;
        printf("Hull variant: %s\n", psslStrHullVariant(hvar));
        break;
    }
    case PSSL_SHADER_DS: {
        PsslDomainVariant dvar = hdr->stageinfo.domainvariant;
        printf("Domain variant: %s\n", psslStrDomainVariant(dvar));
        break;
    }
    default:
        break;
    }

    printgnmshaderfile(hdr);
    printgnmshaderbininfo(hdr);
    printshaderparaminfo(hdr);
}

static inline void printhelp(void) {
    printf("psb-dis\n"
           "Usage: psb-dis [options] file\n"
           "Options:\n"
           "\t-b -- Batch processing\n"
           "\t-h -- Show this help message\n");
}

static std::vector<u8> read_file(const std::string& name) {
    std::ifstream file(name, std::ios_base::binary);
    if (!file.is_open()) {
        return {};
    }

    file.seekg(0, std::ios_base::end);
    auto length = file.tellg();
    file.seekg(0, std::ios_base::beg);

    std::vector<u8> buffer(length);
    file.read(reinterpret_cast<char*>(buffer.data()), length);
    return buffer;
}

static bool process_sb_file(const std::string& name) {
    const auto data = read_file(name);
    if (data.empty()) {
        fmt::print("Invalid file path: {}\n", name);
        return 0;
    }
    fmt::print("Read {} bytes from {}\n", data.size(), name);

    PsslError perr = psslValidateShaderBinary(data.data(), data.size());
    if (perr != PSSL_ERR_OK) {
        printf("Failed to validate shader binary. %s\n", psslStrError(perr));
        return 0;
    }

    printshaderinfo(data.data());
}

int main(int argc, char* argv[]) {
    bool batch_mode{};

    int c = -1;
    while ((c = getopt(argc, argv, "hvb")) != -1) {
        switch (c) {
        case 'h': {
            printhelp();
            return EXIT_SUCCESS;
        }
        case 'b': {
            batch_mode = true;
            break;
        }
        }
    }

    const char* inputfile = argv[optind];
    if (!inputfile) {
        printf("Please pass an input file\n");
        return 0;
    }

    if (batch_mode) {
        for (auto& node : fs::recursive_directory_iterator(inputfile)) {
            if (node.is_directory()) {
                continue;
            }

            if (node.path().extension().string() != ".sb") {
                continue;
            }

            process_sb_file(node.path().string());
        }
    } else {
        process_sb_file(inputfile);
    }
    return EXIT_SUCCESS;
}
