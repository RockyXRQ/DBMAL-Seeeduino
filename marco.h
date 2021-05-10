#ifndef __MARCO_H
#define __MARCO_H

// 返回更小的值
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

// 返回更大的值
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

// 返回上下限之间的值
#define LIMIT(val, min, max) MAX((min), MIN(val, max))

#endif
