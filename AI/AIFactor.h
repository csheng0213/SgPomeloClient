#ifndef _AIFACTOR_H_
#define _AIFACTOR_H_

#include "AIEngine.h"

#define FLOAT_FACTOR (1 + sAIEngine.randf() * 0.1f - 0.05f)

#define FLOAT_FACTOR_LEVEL(factor) ((factor) > 1.03f ? 1 : 0)

#endif // !_AIFACTOR_H_
