#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#define ROUNDUP(a, b) ((((a)-1) / (b) + 1) * (b))  // used for Alignment（对齐） 大于a的b的最小倍数
#define ROUNDDOWN(a, b) ((a) / (b) * (b))          // 小于a的b的最大倍数

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

char* safestrcpy(char*, const char*, int);

#endif