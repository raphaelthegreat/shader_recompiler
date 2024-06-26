#ifndef _PSSL_ERROR_H_
#define _PSSL_ERROR_H_

typedef enum {
	PSSL_ERR_OK = 0,
	PSSL_ERR_TOO_SMALL,
	PSSL_ERR_INVALID_ARGUMENT,
	PSSL_ERR_INVALID_SHADER_TYPE,
	PSSL_ERR_INVALID_CODE_TYPE,
	PSSL_ERR_INVALID_COMPILER_TYPE,
	PSSL_ERR_INVALID_PIPELINE_STAGE,
	PSSL_ERR_INVALID_SYSTEM_ATTRIBUTES,
	PSSL_ERR_INVALID_MAGIC,
	PSSL_ERR_INVALID_VERSION,
	PSSL_ERR_INVALID_INPUT_SEMANTIC,
	PSSL_ERR_MISSING_BINARY_INFO,
} PsslError;

const char* psslStrError(PsslError err);

#endif	// _PSSL_ERROR_H_
