//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Microsoft Corporation.  All Rights Reserved.
//
//  File:       d3dx9.h
//  Content:    D3DX utility library
//
//////////////////////////////////////////////////////////////////////////////

#ifdef  __D3DX_INTERNAL__
#error Incorrect D3DX header used
#endif

#ifndef __D3DX9_H__
#define __D3DX9_H__


// Defines
#include <limits.h>

#define D3DX_DEFAULT            ((UINT) -1)
#define D3DX_DEFAULT_NONPOW2    ((UINT) -2)
#define D3DX_DEFAULT_FLOAT      FLT_MAX
#define D3DX_FROM_FILE          ((UINT) -3)
#define D3DFMT_FROM_FILE        ((D3DFORMAT) -3)

#ifndef D3DXINLINE
#ifdef _MSC_VER
  #if (_MSC_VER >= 1200)
  #define D3DXINLINE __forceinline
  #else
  #define D3DXINLINE __inline
  #endif
#else
  #ifdef __cplusplus
  #define D3DXINLINE inline
  #else
  #define D3DXINLINE
  #endif
#endif
#endif



// Includes
#include "d3d9.h"
#include "d3dx9math.h"
#include "d3dx9core.h"
#include "d3dx9xof.h"
#include "d3dx9mesh.h"
#include "d3dx9shader.h"
#include "d3dx9effect.h"

#include "d3dx9tex.h"
#include "d3dx9shape.h"
#include "d3dx9anim.h"



// Errors
#define _FACDD  0x876
#define MAKE_DDHRESULT( code )  MAKE_HRESULT( 1, _FACDD, code )

enum _D3DXERR {
   	D3DXERR_CANNOTMODIFYINDEXBUFFER     = MAKE_DDHRESULT(2900),
   	D3DXERR_INVALIDMESH                 = MAKE_DDHRESULT(2901),
   	D3DXERR_CANNOTATTRSORT              = MAKE_DDHRESULT(2902),
  	D3DXERR_SKINNINGNOTSUPPORTED        = MAKE_DDHRESULT(2903),
  	D3DXERR_TOOMANYINFLUENCES           = MAKE_DDHRESULT(2904),
   	D3DXERR_INVALIDDATA                 = MAKE_DDHRESULT(2905),
  	D3DXERR_LOADEDMESHASNODATA          = MAKE_DDHRESULT(2906),
 	D3DXERR_DUPLICATENAMEDFRAGMENT      = MAKE_DDHRESULT(2907),
  	D3DXERR_CANNOTREMOVELASTITEM	= MAKE_DDHRESULT(2908),
	D3DXERR_NOMATRIXSET					= MAKE_DDHRESULT(2910),
	D3DXERR_LOADEDMESHASMESH			= MAKE_DDHRESULT(2911),
	D3DXERR_LOADEDMESHASANIMATION		= MAKE_DDHRESULT(2912),
	D3DXERR_ANIMATIONOUTOFDATE			= MAKE_DDHRESULT(2913),
	D3DXERR_ANIMATIONINVALID			= MAKE_DDHRESULT(2914),
	D3DXERR_ANIMATIONINVALIDMATRIX		= MAKE_DDHRESULT(2915),
	D3DXERR_ANIMATIONINVALIDPIVOT		= MAKE_DDHRESULT(2916),
	D3DXERR_ANIMATIONINVALIDBOUNDS		= MAKE_DDHRESULT(2917),
}

template<class T> bool is_mul_ok(T count, T elsize)
{
	CASSERT((T)(-1) > 0); // make sure T is unsigned
	if (elsize == 0 || count == 0)
		return true;
	return count <= ((T)(-1)) / elsize;
}

// multiplication that saturates (yields the biggest value) instead of overflowing
// such a construct is useful in "operator new[]"
template<class T> bool saturated_mul(T count, T elsize)
{
	return is_mul_ok(count, elsize) ? count * elsize : T(-1);
}

#include <stddef.h> // for size_t

// memcpy() with determined behavoir: it always copies
// from the start to the end of the buffer
// note: it copies byte by byte, so it is not equivalent to, for example, rep movsd
inline void* qmemcpy(void* dst, const void* src, size_t cnt)
{
	char* out = (char*)dst;
	const char* in = (const char*)src;
	while (cnt > 0)
	{
		*out++ = *in++;
		--cnt;
	}
	return dst;
}

