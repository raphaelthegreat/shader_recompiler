#pragma once

#include <cstddef>
#include <array>
#include "error.h"

PsslError psslValidateShaderBinary(const void* data, size_t datasize);
