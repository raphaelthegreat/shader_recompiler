cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(create_target_directory_groups target_name)
    source_group("Other Files" REGULAR_EXPRESSION ".")
    get_target_property(target_sources "${target_name}" SOURCES)
    foreach(file_name IN LISTS target_sources)
        get_filename_component(dir_name "${file_name}" PATH)
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

add_library(shader_recompiler STATIC
                      src/common/assert.h
                      src/common/enum.h
                      src/common/types.h
                      src/common/object_bank.h
                      src/common/struct_bank.cpp
                      src/common/struct_bank.h
                      src/control_flow/control_flow_graph.cpp
                      src/control_flow/control_flow_graph.h
                      src/control_flow/divergent_flow.cpp
                      src/control_flow/divergent_flow.h
                      src/control_flow/dominator_tree.cpp
                      src/control_flow/dominator_tree.h
                      src/control_flow/loop_info.cpp
                      src/control_flow/loop_info.h
                      src/control_flow/stackifier.cpp
                      src/control_flow/stackifier.h
                      src/control_flow/token_list.cpp
                      src/control_flow/token_list.h
                      src/spirv/GLSL.std.450.h
                      src/spirv/GLSL.std.450.hpp
                      src/spirv/NonSemanticDebugPrintf.hpp
                      src/spirv/spirv_code_buffer.cpp
                      src/spirv/spirv_code_buffer.h
                      src/spirv/spirv.hpp
                      src/spirv/spirv_instruction.h
                      src/spirv/spirv_module.cpp
                      src/spirv/spirv_module.h
                      src/analysis.cpp
                      src/analysis.h
                      src/common.h
                      src/compiler_control_flow.cpp
                      src/compiler.cpp
                      src/compiler_data_share.cpp
                      src/compiler_debug_profile.cpp
                      src/compiler_defs.h
                      src/compiler_export.cpp
                      src/compiler.h
                      src/compiler_scalar_alu.cpp
                      src/compiler_scalar_memory.cpp
                      src/compiler_vector_alu.cpp
                      src/compiler_vector_interpolation.cpp
                      src/compiler_vector_memory.cpp
                      src/decoder.cpp
                      src/decoder.h
                      src/fetch_shader.cpp
                      src/fetch_shader.h
                      src/gcn_constants.h
                      src/gcn_enum.h
                      src/gcn_mod_info.h
                      src/header.cpp
                      src/header.h
                      src/instruction.cpp
                      src/instruction.h
                      src/instruction_iterator.cpp
                      src/instruction_iterator.h
                      src/instruction_util.cpp
                      src/instruction_util.h
                      src/module.cpp
                      src/module.h
                      src/program_info.cpp
                      src/program_info.h
                      src/shader_binary.h
                      src/shader_key.h
                      src/shader_meta.h
                      src/shader_reg_field.h
                      src/shader_register.h
                      src/state_register.cpp
                      src/state_register.h
                      src/util.h
)
target_include_directories(shader_recompiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
create_target_directory_groups(shader_recompiler)

add_subdirectory(src/sb_parser)
