#include "error.h"

const char* psslStrError(PsslError err) {
	switch (err) {
	case PSSL_ERR_OK:
		return "No error";
	case PSSL_ERR_TOO_SMALL:
		return "The shader data is too small";
	case PSSL_ERR_INVALID_ARGUMENT:
		return "An invalid argument was used";
	case PSSL_ERR_INVALID_SHADER_TYPE:
		return "An invalid shader type was used";
	case PSSL_ERR_INVALID_CODE_TYPE:
		return "An invalid code type was used";
	case PSSL_ERR_INVALID_COMPILER_TYPE:
		return "An invalid compiler type was used";
	case PSSL_ERR_INVALID_PIPELINE_STAGE:
		return "An invalid pipeline stage was used";
	case PSSL_ERR_INVALID_SYSTEM_ATTRIBUTES:
		return "Invalid system attributes were used";
	case PSSL_ERR_INVALID_MAGIC:
		return "An invalid magic constant was found";
	case PSSL_ERR_INVALID_VERSION:
		return "Shader used is in an unsupported version";
	case PSSL_ERR_INVALID_INPUT_SEMANTIC:
		return "An invalid input semantic was used";
	case PSSL_ERR_MISSING_BINARY_INFO:
		return "The shader's binary info is missing from the data";
	default:
		return "Unknown error";
	}
}
