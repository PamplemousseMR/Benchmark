#pragma once

/*	=============== Includes ===============	*/

#include <stdint.h>		/*	uint_fast32	*/
#include <assert.h>		/*	assert	*/

#ifdef __cplusplus
extern "C" {
#endif

/*	=============== Funtions ===============	*/

static __inline uint_fast32_t mod_add(uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mul(uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mac(uint_fast32_t, uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mac2(uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mac3(uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mac4(uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast32_t);
static __inline uint_fast32_t mod_mul_x(uint_fast32_t);
static __inline uint_fast32_t mod_mul_y(uint_fast32_t);
static __inline uint_fast32_t mod_mac_y(uint_fast32_t, uint_fast32_t);

/*	=============== Inline definitions ===============	*/

static __inline uint_fast32_t mod_add(uint_fast32_t a, uint_fast32_t b) {
    uint_fast32_t x;
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    x = a + b;
    x = (x >= 0x7FFFFFFF) ? (x - 0x7FFFFFFF) : x;
    return x;
}

static __inline uint_fast32_t mod_mul(uint_fast32_t a, uint_fast32_t b) {
    uint_fast64_t temp;
    uint_fast32_t temp2;
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    temp = (uint_fast64_t)a * b;
    temp2 = (uint_fast32_t)(temp & 0x7FFFFFFF) + (uint_fast32_t)(temp >> 31);
    return (temp2 >= 0x7FFFFFFF) ? (temp2 - 0x7FFFFFFF) : temp2;
}

static __inline uint_fast32_t mod_mac(uint_fast32_t sum, uint_fast32_t a, uint_fast32_t b) {
    uint_fast64_t temp;
    uint_fast32_t temp2;
    assert (sum <= 0x7FFFFFFE);
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    temp = (uint_fast64_t)a * b + sum;
    temp2 = (uint_fast32_t)(temp & 0x7FFFFFFF) + (uint_fast32_t)(temp >> 31);
    return (temp2 >= 0x7FFFFFFF) ? (temp2 - 0x7FFFFFFF) : temp2;
}

static __inline uint_fast32_t mod_mac2(uint_fast32_t sum, uint_fast32_t a, uint_fast32_t b, uint_fast32_t c, uint_fast32_t d) {
    assert (sum <= 0x7FFFFFFE);
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    assert (c <= 0x7FFFFFFE);
    assert (d <= 0x7FFFFFFE);
    return mod_mac(mod_mac(sum, a, b), c, d);
}

static __inline uint_fast32_t mod_mac3(uint_fast32_t sum, uint_fast32_t a, uint_fast32_t b, uint_fast32_t c, uint_fast32_t d, uint_fast32_t e, uint_fast32_t f) {
    assert (sum <= 0x7FFFFFFE);
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    assert (c <= 0x7FFFFFFE);
    assert (d <= 0x7FFFFFFE);
    assert (e <= 0x7FFFFFFE);
    assert (f <= 0x7FFFFFFE);
    return mod_mac2(mod_mac(sum, a, b), c, d, e, f);
}

static __inline uint_fast32_t mod_mac4(uint_fast32_t sum, uint_fast32_t a, uint_fast32_t b, uint_fast32_t c, uint_fast32_t d, uint_fast32_t e, uint_fast32_t f, uint_fast32_t g, uint_fast32_t h) {
    assert (sum <= 0x7FFFFFFE);
    assert (a <= 0x7FFFFFFE);
    assert (b <= 0x7FFFFFFE);
    assert (c <= 0x7FFFFFFE);
    assert (d <= 0x7FFFFFFE);
    assert (e <= 0x7FFFFFFE);
    assert (f <= 0x7FFFFFFE);
    assert (g <= 0x7FFFFFFE);
    assert (h <= 0x7FFFFFFE);
    return mod_mac2(mod_mac2(sum, a, b, c, d), e, f, g, h);
}

static __inline uint_fast32_t mod_mul_x(uint_fast32_t a) {
    static const int32_t q = 20;
    static const int32_t r = 7;
    int_fast32_t result = (int_fast32_t)(a) / q;
    result = 107374182 * ((int_fast32_t)(a) - result * q) - result * r;
    result += (result < 0 ? 0x7FFFFFFF : 0);
    assert ((uint_fast32_t)(result) == mod_mul(a, 107374182));
    return (uint_fast32_t)result;
}

static __inline uint_fast32_t mod_mul_y(uint_fast32_t a) {
    static const int32_t q = 20554;
    static const int32_t r = 1727;
    int_fast32_t result = (int_fast32_t)(a) / q;
    result = 104480 * ((int_fast32_t)(a) - result * q) - result * r;
    result += (result < 0 ? 0x7FFFFFFF : 0);
    assert ((uint_fast32_t)(result) == mod_mul(a, 104480));
    return (uint_fast32_t)result;
}

static __inline uint_fast32_t mod_mac_y(uint_fast32_t sum, uint_fast32_t a) {
    uint_fast32_t result = mod_add(sum, mod_mul_y(a));
    assert (result == mod_mac(sum, a, 104480));
    return result;
}

#ifdef __cplusplus
}
#endif
