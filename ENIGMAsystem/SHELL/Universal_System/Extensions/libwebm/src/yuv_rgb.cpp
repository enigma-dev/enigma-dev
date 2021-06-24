// Copyright 2016 Adrien Descamps
// Distributed under BSD 3-Clause License

#include "yuv_rgb.h"

#include <x86intrin.h>

#include <stdio.h>


uint8_t clamp(int16_t value)
{
	return value<0 ? 0 : (value>255 ? 255 : value);
}

// Definitions
//
// E'R, E'G, E'B, E'Y, E'Cb and E'Cr refer to the analog signals
// E'R, E'G, E'B and E'Y range is [0:1], while E'Cb and E'Cr range is [-0.5:0.5]
// R, G, B, Y, Cb and Cr refer to the digitalized values
// The digitalized values can use their full range ([0:255] for 8bit values),
// or a subrange (typically [16:235] for Y and [16:240] for CbCr).
// We assume here that RGB range is always [0:255], since it is the case for 
// most digitalized images.
// For 8bit values :
// * Y = round((YMax-YMin)*E'Y + YMin)
// * Cb = round((CbRange)*E'Cb + 128)
// * Cr = round((CrRange)*E'Cr + 128)
// Where *Min and *Max are the range of each channel
//
// In the analog domain , the RGB to YCbCr transformation is defined as:
// * E'Y = Rf*E'R + Gf*E'G + Bf*E'B
// Where Rf, Gf and Bf are constants defined in each standard, with 
// Rf + Gf + Bf = 1 (necessary to ensure that E'Y range is [0:1])
// * E'Cb = (E'B - E'Y) / CbNorm
// * E'Cr = (E'R - E'Y) / CrNorm
// Where CbNorm and CrNorm are constants, dependent of Rf, Gf, Bf, computed 
// to normalize to a [-0.5:0.5] range : CbNorm=2*(1-Bf) and CrNorm=2*(1-Rf)
//
// Algorithms
//
// Most operations will be made in a fixed point format for speed, using 
// N bits of precision. In next section the [x] convention is used for 
// a fixed point rounded value, that is (int being the c type conversion)
// * [x] = int(x*(2^N)+0.5)
// N can be different for each factor, we simply use the highest value
// that will not overflow in 16 bits intermediate variables.
//.
// For RGB to YCbCr conversion, we start by generating a pseudo Y value 
// (noted Y') in fixed point format, using the full range for now.
// * Y' = ([Rf]*R + [Gf]*G + [Bf]*B)>>N
// We can then compute Cb and Cr by
// * Cb = ((B - Y')*[CbRange/(255*CbNorm)])>>N + 128
// * Cr = ((R - Y')*[CrRange/(255*CrNorm)])>>N + 128
// And finally, we normalize Y to its digital range
// * Y = (Y'*[(YMax-YMin)/255])>>N + YMin
// 
// For YCbCr to RGB conversion, we first compute the full range Y' value :
// * Y' = ((Y-YMin)*[255/(YMax-YMin)])>>N
// We can then compute B and R values by :
// * B = ((Cb-128)*[(255*CbNorm)/CbRange])>>N + Y'
// * R = ((Cr-128)*[(255*CrNorm)/CrRange])>>N + Y'
// And finally, for G we know that:
// * G = (Y' - (Rf*R + Bf*B)) / Gf
// From above:
// * G = (Y' - Rf * ((Cr-128)*(255*CrNorm)/CrRange + Y') - Bf * ((Cb-128)*(255*CbNorm)/CbRange + Y')) / Gf
// Since 1-Rf-Bf=Gf, we can take Y' out of the division by Gf, and we get:
// * G = Y' - (Cr-128)*Rf/Gf*(255*CrNorm)/CrRange - (Cb-128)*Bf/Gf*(255*CbNorm)/CbRange
// That we can compute, with fixed point arithmetic, by
// * G = Y' - ((Cr-128)*[Rf/Gf*(255*CrNorm)/CrRange] + (Cb-128)*[Bf/Gf*(255*CbNorm)/CbRange])>>N
// 
// Note : in ITU-T T.871(JPEG), Y=Y', so that part could be optimized out


#if !defined(FIXED_POINT_VALUE)
#define FIXED_POINT_VALUE(value, precision) ((int)(((value)*(1<<precision))+0.5))
#endif

// see above for description
typedef struct
{
	uint8_t r_factor;    // [Rf]
	uint8_t g_factor;    // [Rg]
	uint8_t b_factor;    // [Rb]
	uint8_t cb_factor;   // [CbRange/(255*CbNorm)]
	uint8_t cr_factor;   // [CrRange/(255*CrNorm)]
	uint8_t y_factor;    // [(YMax-YMin)/255]
	uint8_t y_offset;    // YMin
} RGB2YUVParam;

typedef struct
{
	uint8_t cb_factor;   // [(255*CbNorm)/CbRange]
	uint8_t cr_factor;   // [(255*CrNorm)/CrRange]
	uint8_t g_cb_factor; // [Bf/Gf*(255*CbNorm)/CbRange]
	uint8_t g_cr_factor; // [Rf/Gf*(255*CrNorm)/CrRange]
	uint8_t y_factor;    // [(YMax-YMin)/255]
	uint8_t y_offset;    // YMin
} YUV2RGBParam;

#if !defined(RGB2YUV_PARAM)
#define RGB2YUV_PARAM(Rf, Bf, YMin, YMax, CbCrRange) \
{.r_factor=FIXED_POINT_VALUE(Rf, 8), \
.g_factor=256-FIXED_POINT_VALUE(Rf, 8)-FIXED_POINT_VALUE(Bf, 8), \
.b_factor=FIXED_POINT_VALUE(Bf, 8), \
.cb_factor=FIXED_POINT_VALUE((CbCrRange/255.0)/(2.0*(1-Bf)), 8), \
.cr_factor=FIXED_POINT_VALUE((CbCrRange/255.0)/(2.0*(1-Rf)), 8), \
.y_factor=FIXED_POINT_VALUE((YMax-YMin)/255.0, 7), \
.y_offset=YMin}
#endif

#if !defined(YUV2RGB_PARAM)
#define YUV2RGB_PARAM(Rf, Bf, YMin, YMax, CbCrRange) \
{.cb_factor=FIXED_POINT_VALUE(255.0*(2.0*(1-Bf))/CbCrRange, 6), \
.cr_factor=FIXED_POINT_VALUE(255.0*(2.0*(1-Rf))/CbCrRange, 6), \
.g_cb_factor=FIXED_POINT_VALUE(Bf/(1.0-Bf-Rf)*255.0*(2.0*(1-Bf))/CbCrRange, 7), \
.g_cr_factor=FIXED_POINT_VALUE(Rf/(1.0-Bf-Rf)*255.0*(2.0*(1-Rf))/CbCrRange, 7), \
.y_factor=FIXED_POINT_VALUE(255.0/(YMax-YMin), 7), \
.y_offset=YMin}
#endif

static const RGB2YUVParam RGB2YUV[3] = {
	// ITU-T T.871 (JPEG)
	RGB2YUV_PARAM(0.299, 0.114, 0.0, 255.0, 255.0),
	// ITU-R BT.601-7
	RGB2YUV_PARAM(0.299, 0.114, 16.0, 235.0, 224.0),
	// ITU-R BT.709-6
	RGB2YUV_PARAM(0.2126, 0.0722, 16.0, 235.0, 224.0)
};

static const YUV2RGBParam YUV2RGB[3] = {
	// ITU-T T.871 (JPEG)
	YUV2RGB_PARAM(0.299, 0.114, 0.0, 255.0, 255.0),
	// ITU-R BT.601-7
	YUV2RGB_PARAM(0.299, 0.114, 16.0, 235.0, 224.0),
	// ITU-R BT.709-6
	YUV2RGB_PARAM(0.2126, 0.0722, 16.0, 235.0, 224.0)
};


void rgb24_yuv420_std(
	uint32_t width, uint32_t height, 
	const uint8_t *RGB, uint32_t RGB_stride, 
	uint8_t *Y, uint8_t *U, uint8_t *V, uint32_t Y_stride, uint32_t UV_stride, 
	YCbCrType yuv_type)
{
	const RGB2YUVParam *const param = &(RGB2YUV[yuv_type]);
	
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *rgb_ptr1=RGB+y*RGB_stride,
			*rgb_ptr2=RGB+(y+1)*RGB_stride;
		
		uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*u_ptr=U+(y/2)*UV_stride,
			*v_ptr=V+(y/2)*UV_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			// compute yuv for the four pixels, u and v values are summed
			uint8_t y_tmp;
			int16_t u_tmp, v_tmp;
			
			y_tmp = (param->r_factor*rgb_ptr1[0] + param->g_factor*rgb_ptr1[1] + param->b_factor*rgb_ptr1[2])>>8;
			u_tmp = rgb_ptr1[2]-y_tmp;
			v_tmp = rgb_ptr1[0]-y_tmp;
			y_ptr1[0]=((y_tmp*param->y_factor)>>7) + param->y_offset;
			
			y_tmp = (param->r_factor*rgb_ptr1[3] + param->g_factor*rgb_ptr1[4] + param->b_factor*rgb_ptr1[5])>>8;
			u_tmp += rgb_ptr1[5]-y_tmp;
			v_tmp += rgb_ptr1[3]-y_tmp;
			y_ptr1[1]=((y_tmp*param->y_factor)>>7) + param->y_offset;

			y_tmp = (param->r_factor*rgb_ptr2[0] + param->g_factor*rgb_ptr2[1] + param->b_factor*rgb_ptr2[2])>>8;
			u_tmp += rgb_ptr2[2]-y_tmp;
			v_tmp += rgb_ptr2[0]-y_tmp;
			y_ptr2[0]=((y_tmp*param->y_factor)>>7) + param->y_offset;
			
			y_tmp = (param->r_factor*rgb_ptr2[3] + param->g_factor*rgb_ptr2[4] + param->b_factor*rgb_ptr2[5])>>8;
			u_tmp += rgb_ptr2[5]-y_tmp;
			v_tmp += rgb_ptr2[3]-y_tmp;
			y_ptr2[1]=((y_tmp*param->y_factor)>>7) + param->y_offset;

			u_ptr[0] = (((u_tmp>>2)*param->cb_factor)>>8) + 128;
			v_ptr[0] = (((v_tmp>>2)*param->cb_factor)>>8) + 128;
			
			rgb_ptr1 += 6;
			rgb_ptr2 += 6;
			y_ptr1 += 2;
			y_ptr2 += 2;
			u_ptr += 1;
			v_ptr += 1;
		}
	}
}

void rgb32_yuv420_std(
	uint32_t width, uint32_t height, 
	const uint8_t *RGBA, uint32_t RGBA_stride, 
	uint8_t *Y, uint8_t *U, uint8_t *V, uint32_t Y_stride, uint32_t UV_stride, 
	YCbCrType yuv_type)
{
	const RGB2YUVParam *const param = &(RGB2YUV[yuv_type]);
	
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *rgb_ptr1=RGBA+y*RGBA_stride,
			*rgb_ptr2=RGBA+(y+1)*RGBA_stride;
		
		uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*u_ptr=U+(y/2)*UV_stride,
			*v_ptr=V+(y/2)*UV_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			// compute yuv for the four pixels, u and v values are summed
			uint8_t y_tmp;
			int16_t u_tmp, v_tmp;
			
			y_tmp = (param->r_factor*rgb_ptr1[0] + param->g_factor*rgb_ptr1[1] + param->b_factor*rgb_ptr1[2])>>8;
			u_tmp = rgb_ptr1[2]-y_tmp;
			v_tmp = rgb_ptr1[0]-y_tmp;
			y_ptr1[0]=((y_tmp*param->y_factor)>>7) + param->y_offset;
			
			y_tmp = (param->r_factor*rgb_ptr1[4] + param->g_factor*rgb_ptr1[5] + param->b_factor*rgb_ptr1[6])>>8;
			u_tmp += rgb_ptr1[6]-y_tmp;
			v_tmp += rgb_ptr1[4]-y_tmp;
			y_ptr1[1]=((y_tmp*param->y_factor)>>7) + param->y_offset;

			y_tmp = (param->r_factor*rgb_ptr2[0] + param->g_factor*rgb_ptr2[1] + param->b_factor*rgb_ptr2[2])>>8;
			u_tmp += rgb_ptr2[2]-y_tmp;
			v_tmp += rgb_ptr2[0]-y_tmp;
			y_ptr2[0]=((y_tmp*param->y_factor)>>7) + param->y_offset;
			
			y_tmp = (param->r_factor*rgb_ptr2[4] + param->g_factor*rgb_ptr2[5] + param->b_factor*rgb_ptr2[6])>>8;
			u_tmp += rgb_ptr2[6]-y_tmp;
			v_tmp += rgb_ptr2[4]-y_tmp;
			y_ptr2[1]=((y_tmp*param->y_factor)>>7) + param->y_offset;

			u_ptr[0] = (((u_tmp>>2)*param->cb_factor)>>8) + 128;
			v_ptr[0] = (((v_tmp>>2)*param->cb_factor)>>8) + 128;
			
			rgb_ptr1 += 8;
			rgb_ptr2 += 8;
			y_ptr1 += 2;
			y_ptr2 += 2;
			u_ptr += 1;
			v_ptr += 1;
		}
	}
}


void yuv420_rgb24_std(
	uint32_t width, uint32_t height, 
	const uint8_t *Y, const uint8_t *U, const uint8_t *V, uint32_t Y_stride, uint32_t UV_stride, 
	uint8_t *RGB, uint32_t RGB_stride, 
	YCbCrType yuv_type)
{
	const YUV2RGBParam *const param = &(YUV2RGB[yuv_type]);
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*u_ptr=U+(y/2)*UV_stride,
			*v_ptr=V+(y/2)*UV_stride;
		
		uint8_t *rgb_ptr1=RGB+y*RGB_stride,
			*rgb_ptr2=RGB+(y+1)*RGB_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			int8_t u_tmp, v_tmp;
			u_tmp = u_ptr[0]-128;
			v_tmp = v_ptr[0]-128;
			
			//compute Cb Cr color offsets, common to four pixels
			int16_t b_cb_offset, r_cr_offset, g_cbcr_offset;
			b_cb_offset = (param->cb_factor*u_tmp)>>6;
			r_cr_offset = (param->cr_factor*v_tmp)>>6;
			g_cbcr_offset = (param->g_cb_factor*u_tmp + param->g_cr_factor*v_tmp)>>7;
			
			int16_t y_tmp;
			y_tmp = (param->y_factor*(y_ptr1[0]-param->y_offset))>>7;
			rgb_ptr1[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr1[1]-param->y_offset))>>7;
			rgb_ptr1[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[5] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[0]-param->y_offset))>>7;
			rgb_ptr2[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[1]-param->y_offset))>>7;
			rgb_ptr2[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[5] = clamp(y_tmp + b_cb_offset);
			
			rgb_ptr1 += 6;
			rgb_ptr2 += 6;
			y_ptr1 += 2;
			y_ptr2 += 2;
			u_ptr += 1;
			v_ptr += 1;
		}
	}
}

void nv12_rgb24_std(
	uint32_t width, uint32_t height, 
	const uint8_t *Y, const uint8_t *UV, uint32_t Y_stride, uint32_t UV_stride, 
	uint8_t *RGB, uint32_t RGB_stride, 
	YCbCrType yuv_type)
{
	const YUV2RGBParam *const param = &(YUV2RGB[yuv_type]);
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*uv_ptr=UV+(y/2)*UV_stride;
		
		uint8_t *rgb_ptr1=RGB+y*RGB_stride,
			*rgb_ptr2=RGB+(y+1)*RGB_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			int8_t u_tmp, v_tmp;
			u_tmp = uv_ptr[0]-128;
			v_tmp = uv_ptr[1]-128;
			
			//compute Cb Cr color offsets, common to four pixels
			int16_t b_cb_offset, r_cr_offset, g_cbcr_offset;
			b_cb_offset = (param->cb_factor*u_tmp)>>6;
			r_cr_offset = (param->cr_factor*v_tmp)>>6;
			g_cbcr_offset = (param->g_cb_factor*u_tmp + param->g_cr_factor*v_tmp)>>7;
			
			int16_t y_tmp;
			y_tmp = (param->y_factor*(y_ptr1[0]-param->y_offset))>>7;
			rgb_ptr1[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr1[1]-param->y_offset))>>7;
			rgb_ptr1[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[5] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[0]-param->y_offset))>>7;
			rgb_ptr2[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[1]-param->y_offset))>>7;
			rgb_ptr2[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[5] = clamp(y_tmp + b_cb_offset);
			
			rgb_ptr1 += 6;
			rgb_ptr2 += 6;
			y_ptr1 += 2;
			y_ptr2 += 2;
			uv_ptr += 2;
		}
	}
}

void nv21_rgb24_std(
	uint32_t width, uint32_t height, 
	const uint8_t *Y, const uint8_t *UV, uint32_t Y_stride, uint32_t UV_stride, 
	uint8_t *RGB, uint32_t RGB_stride, 
	YCbCrType yuv_type)
{
	const YUV2RGBParam *const param = &(YUV2RGB[yuv_type]);
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*uv_ptr=UV+(y/2)*UV_stride;
		
		uint8_t *rgb_ptr1=RGB+y*RGB_stride,
			*rgb_ptr2=RGB+(y+1)*RGB_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			int8_t u_tmp, v_tmp;
			u_tmp = uv_ptr[1]-128;
			v_tmp = uv_ptr[0]-128;
			
			//compute Cb Cr color offsets, common to four pixels
			int16_t b_cb_offset, r_cr_offset, g_cbcr_offset;
			b_cb_offset = (param->cb_factor*u_tmp)>>6;
			r_cr_offset = (param->cr_factor*v_tmp)>>6;
			g_cbcr_offset = (param->g_cb_factor*u_tmp + param->g_cr_factor*v_tmp)>>7;
			
			int16_t y_tmp;
			y_tmp = (param->y_factor*(y_ptr1[0]-param->y_offset))>>7;
			rgb_ptr1[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr1[1]-param->y_offset))>>7;
			rgb_ptr1[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[5] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[0]-param->y_offset))>>7;
			rgb_ptr2[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[1]-param->y_offset))>>7;
			rgb_ptr2[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[5] = clamp(y_tmp + b_cb_offset);
			
			rgb_ptr1 += 6;
			rgb_ptr2 += 6;
			y_ptr1 += 2;
			y_ptr2 += 2;
			uv_ptr += 2;
		}
	}
}
