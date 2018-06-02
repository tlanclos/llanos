#pragma once
#include <llanos/types.h>

#define U64MAX  ((u64)-1)
#define U32MAX  ((u32)-1)
#define U16MAX  ((u16)-1)
#define U8MAX   ((u8)-1)

#define U64MIN  ((u64)0)
#define U32MIN  ((u32)0)
#define U16MIN  ((u16)0)
#define U8MIN   ((u8)0)

#define S64MAX  ((s64)((U64MAX / 2)))
#define S32MAX  ((s32)((U32MAX / 2)))
#define S16MAX  ((s16)((U16MAX / 2)))
#define S8MAX   ((s8)((U8MAX / 2)))

#define S64MIN  ((s64)((u64)S64MAX + 1))
#define S32MIN  ((s32)((u32)S32MAX + 1))
#define S16MIN  ((s16)((u16)S16MAX + 1))
#define S8MIN   ((s8)((u8)S8MAX + 1))
