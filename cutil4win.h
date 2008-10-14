#define fprintf(fp, fmt, ...) do {				\
		char buf[200];							\
		sprintf_s(buf, 200, fmt, __VA_ARGS__);	\
		MessageBox(0, buf, "fprintf", MB_OK);	\
		} while (0)

#include <cuda_runtime.h>
#include <cutil.h>