/*
 * Copyright (c) 2013-2014 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

/*!
 * \file
 * \brief The Bitwise Kernels: And, Or, Xor, Not.
 * \author Hans-Peter Nilsson <hp@axis.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include "vxcl_kernel_module.h"

/*
 * The three bitwise kernels with binary parameters have the same parameter domain so
 * let's just have one set of validators.
 */

static vx_status VX_CALLBACK vxBinaryBitwiseInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_df_image format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format == VX_DF_IMAGE_U8)
                status = VX_SUCCESS;
            vxReleaseImage(&input);
        }
        vxReleaseParameter(&param);
    }
    else if (index == 1)
    {
        vx_image images[2];
        vx_parameter param[2] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
        };
        vxQueryParameter(param[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
        vxQueryParameter(param[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
        if (images[0] && images[1])
        {
            vx_uint32 width[2], height[2];
            vx_df_image format[2];

            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));
            if (width[0] == width[1] && height[0] == height[1] && format[0] == format[1])
                status = VX_SUCCESS;
            vxReleaseImage(&images[1]);
            vxReleaseImage(&images[0]);
        }
        vxReleaseParameter(&param[0]);
        vxReleaseParameter(&param[1]);
    }
    return status;
}

static vx_status VX_CALLBACK vxBinaryBitwiseOutputValidator(vx_node node, vx_uint32 index, vx_meta_format ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param0 = vxGetParameterByIndex(node, 0);
        if (param0)
        {
            vx_image image0 = 0;
            vxQueryParameter(param0, VX_PARAMETER_ATTRIBUTE_REF, &image0, sizeof(image0));
            /*
             * When passing on the geometry to the output image, we only look at image 0, as
             * both input images are verified to match, at input validation.
             */
            if (image0)
            {
                vx_uint32 width = 0, height = 0;
                vxQueryImage(image0, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(image0, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));

                /* fill in the meta data with the attributes so that the checker will pass */
                vx_df_image format = VX_DF_IMAGE_U8;
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));

                status = VX_SUCCESS;
                vxReleaseImage(&image0);
            }
            vxReleaseParameter(&param0);
        }
    }
    return status;
}

static vx_param_description_t binary_bitwise_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_cl_kernel_description_t and_kernel = {
    {
        VX_KERNEL_AND,
        "com.samsung.opencl.and",
        NULL,
        binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
        vxBinaryBitwiseInputValidator,
        vxBinaryBitwiseOutputValidator,
        NULL,
        NULL,
    },
	"/system/usr/vxcl/vx_and.cl",
    "vx_and",
    INIT_PROGRAMS,
    INIT_KERNELS,
    INIT_NUMKERNELS,
    INIT_RETURNS,
    NULL,
};

vx_cl_kernel_description_t orr_kernel = {
    {
        VX_KERNEL_OR,
        "com.samsung.opencl.or",
        NULL,
        binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
        vxBinaryBitwiseInputValidator,
        vxBinaryBitwiseOutputValidator,
        NULL,
        NULL,
    },
	"/system/usr/vxcl/vx_orr.cl",
    "vx_orr",
    INIT_PROGRAMS,
    INIT_KERNELS,
    INIT_NUMKERNELS,
    INIT_RETURNS,
    NULL,
};

vx_cl_kernel_description_t xor_kernel = {
    {
        VX_KERNEL_XOR,
        "com.samsung.opencl.xor",
        NULL,
        binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
        vxBinaryBitwiseInputValidator,
        vxBinaryBitwiseOutputValidator,
        NULL,
        NULL,
    },
	"/system/usr/vxcl/vx_xor.cl",
    "vx_xor",
    INIT_PROGRAMS,
    INIT_KERNELS,
    INIT_NUMKERNELS,
    INIT_RETURNS,
    NULL,
};

/* The Not kernel is an unary operator, requiring separate validators. */

static vx_status VX_CALLBACK vxUnaryBitwiseInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_df_image format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format == VX_DF_IMAGE_U8)
                status = VX_SUCCESS;
            vxReleaseImage(&input);
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status VX_CALLBACK vxUnaryBitwiseOutputValidator(vx_node node, vx_uint32 index, vx_meta_format ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image inimage = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &inimage, sizeof(inimage));
            if (inimage)
            {
                vx_uint32 width = 0, height = 0;
                vxQueryImage(inimage, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(inimage, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));

                /* fill in the meta data with the attributes so that the checker will pass */
                vx_df_image format = VX_DF_IMAGE_U8;
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxSetMetaFormatAttribute(ptr, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                status = VX_SUCCESS;
                vxReleaseImage(&inimage);
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t unary_bitwise_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_cl_kernel_description_t not_kernel = {
    {
        VX_KERNEL_NOT,
        "com.samsung.opencl.not",
        NULL,
        unary_bitwise_kernel_params, dimof(unary_bitwise_kernel_params),
        vxUnaryBitwiseInputValidator,
        vxUnaryBitwiseOutputValidator,
        NULL,
        NULL,
    },
	"/system/usr/vxcl/vx_not.cl",
    "vx_not",
    INIT_PROGRAMS,
    INIT_KERNELS,
    INIT_NUMKERNELS,
    INIT_RETURNS,
    NULL,
};
