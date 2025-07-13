#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#ifndef uint8_t
#undef uint8_t
#define uint8_t unsigned char
#endif

#ifndef int8_t
#undef int8_t
#define int8_t signed char
#endif

#ifndef uint16_t
#undef uint16_t
#define uint16_t unsigned short
#endif

#ifndef int16_t
#undef int16_t
#define int16_t signed short
#endif

#ifndef uint32_t
#undef uint32_t
#define uint32_t unsigned long
#endif

#ifndef int32_t
#undef int32_t
#define int32_t signed long
#endif

#ifndef bool
#undef bool
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifdef __C51__
#ifndef const_t
#define const_t code
#endif
#ifndef xdata_t
#define xdata_t xdata
#endif
#else
#ifndef const_t
#define const_t const
#endif
#ifndef xdata_t
#define xdata_t
#endif
#define reentrant
#endif

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#endif
