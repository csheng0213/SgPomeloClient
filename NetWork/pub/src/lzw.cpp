#include "pub.h"

namespace pub {
	static const uint32_t MAX_BITS = 12;
	static const uint32_t HASH_SIZE = 5003;

	static const uint32_t CODE_CLEAR = (1 << 8);
	static const uint32_t CODE_EOF = (CODE_CLEAR + 1);
	static const uint32_t CODE_BEGIN = (CODE_EOF + 1);
	static const uint32_t CODE_MAX = ((1 << MAX_BITS) - 1);

	size_t LzwEncode(const void *src, size_t srcSize, void *dst, size_t dstSize)
	{
#define RESET() { \
	curCodeBits = 9; \
	curCodeMax = ((1 << curCodeBits) - 1); \
	curCode = CODE_BEGIN; \
	memset(hashTab, -1, sizeof(hashTab)); \
}

#define PUT(code) { \
	*dstCur |= (code << curBits); \
	curBits += curCodeBits; \
	if (curBits & 0xffffffe0) { \
		curBits &= 0x1f; \
		dstCur++; \
		if (dstCur == dstEnd) \
			return 0; \
		*dstCur = (code >> (curCodeBits - curBits)); \
	} \
}

		const uint8_t *srcCur = (const uint8_t *)src;
		const uint8_t *srcEnd = srcCur + srcSize;
		uint32_t *dstCur = (uint32_t *)dst;
		uint32_t *dstEnd = dstCur + (dstSize >> 2);

		uint32_t curBits = 0;
		uint32_t curCodeBits, curCodeMax;
		uint32_t prefix, surffix, entry, curCode;

		uint32_t hashTab[HASH_SIZE];
		uint32_t i, step = 1;

		*dstCur = 0;
		RESET();
		for (prefix = *srcCur++; srcCur != srcEnd; srcCur++) {
			surffix = *srcCur;
			entry = ((prefix << 8) | surffix);
			i = ((surffix << 4) | prefix);

HASH_FIND:
			if (i >= HASH_SIZE) i %= HASH_SIZE;

			if (hashTab[i] == 0xffffffff)
				goto FIND_FAILED;
			if ((hashTab[i] & 0xfffff) == entry) {
				prefix = (hashTab[i] >> 20);
				continue;
			}

			i += step;
			goto HASH_FIND;

FIND_FAILED:
			PUT(prefix);
			prefix = surffix;

			if (curCode >= CODE_MAX) {
				PUT(CODE_CLEAR);
				RESET();
			}
			else {
				if (curCode > curCodeMax) {
					curCodeBits++;
					curCodeMax = ((1 << curCodeBits) - 1);
				}
				hashTab[i] = ((curCode << 20) | entry);
				curCode++;
			}
		}

		PUT(prefix);
		if (curCode >= CODE_MAX) {
			PUT(CODE_CLEAR);
			RESET();
		}
		else if (curCode > curCodeMax) {
			curCodeBits++;
			curCodeMax = ((1 << curCodeBits) - 1);
		}
		PUT(CODE_EOF);

		return (((char *)dstCur - (char *)dst) + (curBits ? 4 : 0));

#undef PUT
#undef RESET
	}


	size_t LzwDecode(const void *src, size_t srcSize, void *dst, size_t dstSize)
	{
#define RESET() { \
	curCodeBits = 9; \
	curCodeMax = ((1 << curCodeBits) - 1); \
	curCode = CODE_BEGIN; \
}

#define GET() { \
	if (srcCur >= srcEnd) return 0; \
	inCode = ((*srcCur >> curBits) & ((1 << curCodeBits) - 1)); \
	curBits += curCodeBits; \
	if (curBits & 0xffffffe0) { \
		curBits &= 0x1f; srcCur++; \
		if (curBits > 0) \
			if (srcCur >= srcEnd) return 0; \
			else inCode |= ( (*srcCur & ((1 << curBits) - 1)) << (curCodeBits - curBits) ); \
	} \
}

#define OUTPUT(c) { if (dstCur == dstEnd) return 0; *dstCur++ = c; }

		const uint32_t *srcCur = (const uint32_t *)src;
		const uint32_t *srcEnd = srcCur + (srcSize >> 2);
		uint8_t *dstCur = (uint8_t *)dst;
		uint8_t *dstEnd = dstCur + dstSize;

		uint32_t curBits = 0;
		uint32_t curCodeBits, curCodeMax, curCode;
		uint32_t prefix, surffix, inCode;

		uint32_t prefixTab[4096];
		uint8_t surffixTab[4096];
		uint32_t outCodeCount = 0;
		uint8_t outCode[4096];

		RESET();
		GET();
		prefix = surffix = inCode;
		OUTPUT(prefix);
		while (dstCur < dstEnd) {
			GET();
			if (inCode == CODE_EOF) {
				return (char *)dstCur - (char *)dst;
			}

			if (inCode == CODE_CLEAR) {
				RESET();
				GET();
				prefix = surffix = inCode;
				OUTPUT(prefix);
				continue;
			}

			if (inCode > curCode) {
				return 0;
			}
			else if (inCode == curCode) {
				outCode[outCodeCount++] = surffix;
				surffix = prefix;
			}
			else
				surffix = inCode;
			while (surffix > 0xff) {
				outCode[outCodeCount++] = surffixTab[surffix];
				surffix = prefixTab[surffix];
			}
			outCode[outCodeCount++] = surffix;

			while (outCodeCount > 0)
				*dstCur++ = outCode[--outCodeCount];

			prefixTab[curCode] = prefix;
			surffixTab[curCode] = surffix;
			curCode++;
			if (curCode > curCodeMax) {
				curCodeBits++;
				curCodeMax = (1 << curCodeBits) - 1;
			}
			prefix = inCode;
		}

		return 0;

#undef OUTPUT
#undef GET
#undef RESET
	}
}
