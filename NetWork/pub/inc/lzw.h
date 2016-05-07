#ifndef _PUB_LZW_H_
#define _PUB_LZW_H_

namespace pub {
	PUB_API size_t LzwEncode(const void *src, size_t srcSize, void *dst, size_t dstSize);
	PUB_API size_t LzwDecode(const void *src, size_t srcSize, void *dst, size_t dstSize);
}

#endif /* _PUB_LZW_H_ */
