/*****************************************************************************
Copyright (c) 2011  David Guillen Fandos (david@davidgf.net)
Copyright (c) 2024  Alberto Mardegan (mardy@users.sourceforge.net)
All rights reserved.

Attention! Contains pieces of code from others such as Mesa and GRRLib

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of copyright holders nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "pixels.h"

#include "debug.h"
#include "opengx.h"

#include <algorithm>
#include <math.h>
#include <ogc/gx.h>
#include <variant>

#define MAX_FAST_CONVERSIONS 8

typedef void (FastConverter)(const void *data, GLenum type, int width, int height,
                              void *dst, int x, int y, int dstpitch);
static struct FastConversion {
    GLenum gl_format;
    u8 gx_format;
    union {
        uintptr_t id;
        FastConverter *func;
    } conv;
} s_registered_conversions[MAX_FAST_CONVERSIONS] = {
    { GL_RGB, GX_TF_RGB565, ogx_fast_conv_RGB_RGB565 },
    { GL_RGBA, GX_TF_RGBA8, ogx_fast_conv_RGBA_RGBA8 },
    { GL_LUMINANCE, GX_TF_I8, ogx_fast_conv_Intensity_I8 },
    0,
};

static inline uint8_t luminance_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    /* TODO: fix this, the three components do not have the same weight to the
     * human eye. Use a formula from
     * https://songho.ca/dsp/luminance/luminance.html
     */
    int luminance = int(r) + int(g) + int(b);
    return luminance / 3;
}

struct Texel {
    virtual void setColor(GXColor color) = 0;
    virtual void store (void *texture, int x, int y, int pitch) = 0;
    virtual int pitch_for_width(int width) = 0;
};

struct TexelRGBA8: public Texel {
    static constexpr bool has_rgb = true;
    static constexpr bool has_alpha = true;
    static constexpr bool has_luminance = false;

    TexelRGBA8() = default;
    TexelRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
        r(r), g(g), b(b), a(a) {}

    void setColor(GXColor c) override {
        r = c.r; g = c.g; b = c.b; a = c.a;
    }

    void store(void *texture, int x, int y, int pitch) override {
        int block_x = x / 4;
        int block_y = y / 4;
        uint8_t *d = static_cast<uint8_t*>(texture) +
            block_y * pitch * 4 + block_x * 64 + (y % 4) * 8 + (x % 4) * 2;
        d[0] = a;
        d[1] = r;
        d[32] = g;
        d[33] = b;
    }

    static inline int compute_pitch(int width) {
        /* texel are in pairs of 4x4 blocks, each element 2 bytes wide */
        return ((width + 3) / 4) * 16;
    }

    int pitch_for_width(int width) override { return compute_pitch(width); }

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Texel16: public Texel {
    Texel16() = default;
    Texel16(uint16_t value): word(value) {}
    void store(void *texture, int x, int y, int pitch) override {
        int block_x = x / 4;
        int block_y = y / 4;
        uint8_t *d = static_cast<uint8_t*>(texture) +
            block_y * pitch * 4 + block_x * 32 + (y % 4) * 8 + (x % 4) * 2;
        d[0] = byte0();
        d[1] = byte1();
    }

    static inline int compute_pitch(int width) {
        /* texel are in 4x4 blocks, each element 2 bytes wide */
        return ((width + 3) / 4) * 8;
    }

    int pitch_for_width(int width) override { return compute_pitch(width); }

    uint8_t byte0() const { return word >> 8; }
    uint8_t byte1() const { return word & 0xff; }

    uint16_t word;
};

struct TexelIA8: public Texel16 {
    static constexpr bool has_rgb = false;
    static constexpr bool has_alpha = true;
    static constexpr bool has_luminance = true;

    TexelIA8() = default;
    TexelIA8(uint8_t intensity, uint8_t alpha):
        Texel16((alpha << 8) | intensity) {}

    void setColor(GXColor c) override {
        int luminance = luminance_from_rgb(c.r, c.g, c.b);
        word = (c.a << 8) | luminance;
    }
};

struct TexelRGB565: public Texel16 {
    static constexpr bool has_alpha = false;
    static constexpr bool has_rgb = true;

    TexelRGB565() = default;
    TexelRGB565(uint8_t r, uint8_t g, uint8_t b):
        Texel16(((r & 0xf8) << 8) |
                ((g & 0xfc) << 3) |
                ((b & 0xf8) >> 3)) {}

    void setColor(GXColor c) override {
        word = ((c.r & 0xf8) << 8) |
            ((c.g & 0xfc) << 3) |
            ((c.b & 0xf8) >> 3);
    }
};

struct Texel8: public Texel {
    Texel8() = default;
    Texel8(uint8_t value):
        value(value) {}

    void store(void *texture, int x, int y, int pitch) override {
        int block_x = x / 8;
        int block_y = y / 4;
        uint8_t *d = static_cast<uint8_t*>(texture) +
            block_y * pitch * 4 + block_x * 32 + (y % 4) * 8 + (x % 8);
        d[0] = value;
    }

    static inline int compute_pitch(int width) {
        /* texel are in 8x4 blocks, each element 1 bytes wide */
        return ((width + 7) / 8) * 8;
    }

    int pitch_for_width(int width) override { return compute_pitch(width); }

    uint8_t value;
};

struct TexelI8: public Texel8 {
    static constexpr bool has_rgb = false;
    static constexpr bool has_alpha = false;
    static constexpr bool has_luminance = true;

    using Texel8::Texel8;
    void setColor(GXColor c) override {
        value = luminance_from_rgb(c.r, c.g, c.b);
    }
};

struct TexelA8: public Texel8 {
    static constexpr bool has_rgb = false;
    static constexpr bool has_alpha = true;
    static constexpr bool has_luminance = false;

    using Texel8::Texel8;
    void setColor(GXColor c) override {
        value = c.a;
    }
};

template <typename T> static inline uint8_t component(T value);
template <> inline uint8_t component(uint8_t value) { return value; }
template <> inline uint8_t component(uint16_t value) { return value >> 8; }
template <> inline uint8_t component(uint32_t value) { return value >> 24; }
template <> inline uint8_t component(float value) {
    return (uint8_t)(int(std::clamp(value, 0.0f, 1.0f) * 255.0f) & 0xff);
}

/* This template class is used to perform reading of a pixel and storing it in
 * the desired texture format in a single go. It does that in about 1/5th of
 * the time the generic algorithm (on Dolphin the difference is even bigger, up
 * to 1/10th), at the expense of a larger code size.
 *
 * Note that this class does not support packed pixel formats: each pixel
 * component must be at least one byte wide.
 */
template <typename T, char NUM_ELEMS, GLenum FORMAT>
struct DataReader {
    typedef T type;

    static inline int pitch_for_width(int width) {
        return width * NUM_ELEMS * sizeof(T);
    }

    static inline const T *row_ptr(const void *data, int y, int pitch) {
        return static_cast<const T *>(data) + y * pitch / sizeof(T);
    }

    template <typename P>
    static inline const T *read(const T *data, P &pixel) {
        if constexpr (NUM_ELEMS == 4 && P::has_rgb && P::has_alpha) {
            pixel = P(component(data[0]),
                      component(data[1]),
                      component(data[2]),
                      component(data[3]));
        } else if constexpr (NUM_ELEMS >= 3 && P::has_rgb && !P::has_alpha) {
            pixel = P(component(data[0]),
                      component(data[1]),
                      component(data[2]));
        } else if constexpr (NUM_ELEMS == 3 && P::has_rgb && P::has_alpha) {
            pixel = P(component(data[0]),
                      component(data[1]),
                      component(data[2]),
                      1.0f);
        } else {
            /* TODO (maybe) support converting from intensity to RGB */
            uint8_t luminance, alpha;
            if constexpr (P::has_luminance) {
                if constexpr (NUM_ELEMS >= 3) {
                    luminance = luminance_from_rgb(component(data[0]),
                                                   component(data[1]),
                                                   component(data[2]));
                } else if constexpr (NUM_ELEMS == 2) {
                    luminance = component(data[0]);
                } else { // Just a single component in the source data
                    if constexpr (FORMAT == GL_LUMINANCE) {
                        luminance = component(data[0]);
                    } else {
                        luminance = 0;
                    }
                }
            }

            if constexpr (P::has_alpha) {
                if constexpr (NUM_ELEMS == 4) {
                    alpha = component(data[3]);
                } else if constexpr (FORMAT == GL_LUMINANCE_ALPHA) {
                    alpha = component(data[1]);
                } else if constexpr (FORMAT == GL_ALPHA) {
                    alpha = component(data[0]);
                } else {
                    alpha = 255;
                }
            }

            if constexpr (P::has_luminance && P::has_alpha) {
                pixel = P(luminance, alpha);
            } else if constexpr (P::has_luminance) {
                pixel = P(luminance);
            } else { /* Only alpha */
                pixel = P(alpha);
            }
        }

        return data + NUM_ELEMS;
    }
};

template <typename T>
using DataReaderRGBA = DataReader<T, 4, GL_RGBA>;

template <typename T>
using DataReaderRGB = DataReader<T, 3, GL_RGB>;

template <typename T>
using DataReaderLA = DataReader<T, 2, GL_LUMINANCE_ALPHA>;

template <typename T>
using DataReaderIntensity = DataReader<T, 1, GL_LUMINANCE>;

template <typename T>
using DataReaderAlpha = DataReader<T, 1, GL_ALPHA>;

/* Base class for the generic reader: this is used as base class by the
 * CompoundDataReader and the GenericDataReader classes below.
 *
 * Note that for the time being we assume the pitch to be the minimum required
 * to store a row of pixels.
 */
struct DataReaderBase {
    virtual GXColor read() = 0;
};

static const struct MasksPerType {
    GLenum type;
    char bytes; /* number of bytes per pixel */
    char rbits; /* bits of data for each component */
    char gbits;
    char bbits;
    char abits;
    char roff; /* offsets (relative to memory layout, not registers */
    char goff;
    char boff;
    char aoff;
} s_masks_per_type[] = {
    {GL_UNSIGNED_BYTE_3_3_2, 1, 3, 3, 2, 0, 0, 3, 6, 0 },
    {GL_UNSIGNED_BYTE_2_3_3_REV, 1, 3, 3, 2, 0, 5, 2, 0, 0 },
    {GL_UNSIGNED_SHORT_5_6_5, 2, 5, 6, 5, 0, 0, 5, 11, 0 },
    {GL_UNSIGNED_SHORT_5_6_5_REV, 2, 5, 6, 5, 0, 11, 5, 0, 0 },
    {GL_UNSIGNED_SHORT_4_4_4_4, 2, 4, 4, 4, 4, 0, 4, 8, 12 },
    {GL_UNSIGNED_SHORT_4_4_4_4_REV, 2, 4, 4, 4, 4, 12, 8, 4, 0 },
    {GL_UNSIGNED_SHORT_5_5_5_1, 2, 5, 5, 5, 1, 0, 5, 10, 15 },
    {GL_UNSIGNED_SHORT_1_5_5_5_REV, 2, 5, 5, 5, 1, 11, 6, 1, 0 },
    {GL_UNSIGNED_INT_8_8_8_8, 4, 8, 8, 8, 8, 0, 8, 16, 24 },
    {GL_UNSIGNED_INT_8_8_8_8_REV, 4, 8, 8, 8, 8, 24, 16, 8, 0 },
    {GL_UNSIGNED_INT_10_10_10_2, 4, 10, 10, 10, 2, 0, 10, 20, 30 },
    {GL_UNSIGNED_INT_2_10_10_10_REV, 4, 10, 10, 10, 2, 22, 12, 2, 0 },
    {0, }
};

/* This class handles reading of pixels stored in one of the formats listed
 * above, where each pixel is packed in at most 32 bits. */
struct CompoundDataReader: public DataReaderBase {
    CompoundDataReader() = default;
    CompoundDataReader(const void *data, GLenum format, GLenum type):
        data(static_cast<const char *>(data)),
        format(format),
        mask_data(*find_mask_per_type(type)) {
        if (format == GL_BGR || format == GL_BGRA) { /* swap red and blue */
            char tmp = mask_data.roff;
            mask_data.roff = mask_data.boff;
            mask_data.boff = tmp;
        }
        rmask = compute_mask(mask_data.rbits, mask_data.roff);
        gmask = compute_mask(mask_data.gbits, mask_data.goff);
        bmask = compute_mask(mask_data.bbits, mask_data.boff);
        amask = compute_mask(mask_data.abits, mask_data.aoff);
    }

    static const MasksPerType *find_mask_per_type(GLenum type) {
        for (int i = 0; s_masks_per_type[i].type != 0; i++) {
            if (s_masks_per_type[i].type == type) {
                return &s_masks_per_type[i];
            }
        }
        return nullptr;
    }

    inline uint32_t compute_mask(int nbits, int offset) {
        uint32_t mask = (1 << nbits) - 1;
        return mask << (mask_data.bytes * 8 - (nbits + offset));
    }

    inline uint8_t read_component(uint32_t pixel, uint32_t mask,
                                  int nbits, int offset) {
        uint32_t value = pixel & mask;
        int shift = mask_data.bytes * 8 - offset - 8;
        uint8_t c = shift > 0 ? (value >> shift) : (value << -shift);
        if (nbits < 8) {
            c |= (c >> nbits);
        }
        return c;
    }

    inline uint32_t read_pixel() const {
        uint32_t pixel = 0;
        for (int i = 0; i < 4; i++) {
            if (i < mask_data.bytes) {
                pixel <<= 8;
                pixel |= data[n_read + i];
            }
        }
        return pixel;
    }

    GXColor read() override {
        uint32_t pixel = read_pixel();
        GXColor c;
        c.r = read_component(pixel, rmask, mask_data.rbits, mask_data.roff);
        c.g = read_component(pixel, gmask, mask_data.gbits, mask_data.goff);
        c.b = read_component(pixel, bmask, mask_data.bbits, mask_data.boff);
        if (mask_data.abits > 0) {
            c.a = read_component(pixel, amask, mask_data.abits, mask_data.aoff);
        } else {
            c.a = 255;
        }
        n_read += mask_data.bytes;
        return c;
    }

    const char *data;
    int n_read = 0;
    uint32_t rmask;
    uint32_t gmask;
    uint32_t bmask;
    uint32_t amask;
    MasksPerType mask_data;
    GLenum format;
};

static const struct ComponentsPerFormat {
    GLenum format;
    char components_per_pixel;
    char component_index[4]; /* component role (0=red, ..., 3=alpha) */
} s_components_per_format[] = {
    { GL_RGBA, 4, { 0, 1, 2, 3 }},
    { GL_BGRA, 4, { 2, 1, 0, 3 }},
    { GL_RGB, 3, { 0, 1, 2 }},
    { GL_BGR, 3, { 2, 1, 0 }},
    { GL_LUMINANCE_ALPHA, 2, { 0, 3 }},
    { GL_INTENSITY, 1, { 0 }},
    { GL_LUMINANCE, 1, { 0 }},
    { GL_RED, 1, { 0 }},
    { GL_GREEN, 1, { 1 }},
    { GL_BLUE, 1, { 2 }},
    { GL_ALPHA, 1, { 3 }},
    { 0, }
};

/* This is a generic class to read pixels whose components are expressed by 8,
 * 16, 32 bit wide integers or by 32 bit floats.
 */
template <typename T>
struct GenericDataReader: public DataReaderBase {
    GenericDataReader(const void *data, GLenum format, GLenum type):
        data(static_cast<const T *>(data)), format(format),
        component_data(*find_component_data(format)) {}

    static const ComponentsPerFormat *find_component_data(GLenum format) {
        for (int i = 0; s_components_per_format[i].format != 0; i++) {
            if (s_components_per_format[i].format == format) {
                return &s_components_per_format[i];
            }
        }
        return nullptr;
    }

    int pitch_for_width(int width) {
        return width * component_data.components_per_pixel * sizeof(T);
    }

    GXColor read() override {
        union {
            uint8_t components[4];
            GXColor c;
        } pixel = { 0, 0, 0, 255 };

        const ComponentsPerFormat &cd = component_data;
        for (int i = 0; i < cd.components_per_pixel; i++) {
            pixel.components[cd.component_index[i]] = component(data[n_read++]);
        }

        /* Some formats require a special handling */
        if (cd.format == GL_INTENSITY ||
            cd.format == GL_LUMINANCE ||
            cd.format == GL_LUMINANCE_ALPHA) {
            pixel.c.g = pixel.c.b = pixel.c.r;
            if (cd.format == GL_INTENSITY) pixel.c.a = pixel.c.r;
        }

        return pixel.c;
    }

    const T *data;
    GLenum format;
    int n_read = 0;
    ComponentsPerFormat component_data;
};

template <typename READER, typename TEXEL> static inline
void load_texture_typed(const void *src, int width, int height,
                        void *dest, int x, int y, int dstpitch)
{
    // TODO: add alignment options
    using DataType = typename READER::type;

    int srcpitch = READER::pitch_for_width(width);

    for (int ry = 0; ry < height; ry++) {
        const DataType *srcline = READER::row_ptr(src, ry, srcpitch);
        for (int rx = 0; rx < width; rx++) {
            TEXEL texel;
            srcline = READER::read(srcline, texel);
            texel.store(dest, rx + x, ry + y, dstpitch);
        }
    }
}

template <template<typename> typename READERBASE, typename TEXEL> static inline
void load_texture(const void *data, GLenum type, int width, int height,
                  void *dst, int x, int y, int dstpitch)
{
    using Texel = TEXEL;

    switch (type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        load_texture_typed<READERBASE<uint8_t>,Texel>(data, width, height,
                                                      dst, x, y, dstpitch);
        break;
    case GL_FLOAT:
        load_texture_typed<READERBASE<float>,Texel>(data, width, height,
                                                    dst, x, y, dstpitch);
        break;
    default:
        warning("Unsupported texture format %d", type);
    }
}

void _ogx_bytes_to_texture(const void *data, GLenum format, GLenum type,
                           int width, int height,
                           void *dst, uint32_t gx_format,
                           int x, int y, int dstpitch)
{
    /* Accelerate the most common transformations by using the specialized
     * readers. We only do this for some transformations, since every
     * instantiation of the template takes some space, and the number of
     * possible combinations is polynomial.
     */
    for (int i = 0; i < MAX_FAST_CONVERSIONS; i++) {
        const FastConversion &c = s_registered_conversions[i];
        if (c.gl_format == 0) break;

        if (c.gl_format == format && c.gx_format == gx_format) {
            c.conv.func(data, type, width, height, dst, x, y, dstpitch);
            return;
        }
    }

    debug(OGX_LOG_TEXTURE,
          "No fast conversion registered for GL format %04x to GX format %d",
          format, gx_format);

    /* Here starts the code for the generic converter. We start by selecting
     * the proper Texel subclass for the given GX texture format, then we
     * select the reader based on the GL type parameter, and then we do the
     * conversion pixel by pixel, by using GXColor as intermediate format.
     *
     * We use std::variant so that we can safely construct our objects on the
     * stack. */
    std::variant<
        TexelRGBA8,
        TexelRGB565,
        TexelIA8,
        TexelI8,
        TexelA8
    > texel_v;
    Texel *texel;

    std::variant<
        CompoundDataReader,
        GenericDataReader<uint8_t>,
        GenericDataReader<uint16_t>,
        GenericDataReader<uint32_t>,
        GenericDataReader<float>
    > reader_v;
    DataReaderBase *reader;

    switch (gx_format) {
    case GX_TF_RGBA8:
        texel_v = TexelRGBA8();
        texel = &std::get<TexelRGBA8>(texel_v);
        break;
    case GX_TF_RGB565:
        texel_v = TexelRGB565();
        texel = &std::get<TexelRGB565>(texel_v);
        break;
    case GX_TF_IA8:
        texel_v = TexelIA8();
        texel = &std::get<TexelIA8>(texel_v);
        break;
    case GX_TF_I8:
        texel_v = TexelI8();
        texel = &std::get<TexelI8>(texel_v);
        break;
    case GX_TF_A8:
        texel_v = TexelA8();
        texel = &std::get<TexelA8>(texel_v);
        break;
    }

    switch (type) {
    case GL_UNSIGNED_BYTE:
        reader_v = GenericDataReader<uint8_t>(data, format, type);
        reader = &std::get<GenericDataReader<uint8_t>>(reader_v);
        break;
    case GL_UNSIGNED_SHORT:
        reader_v = GenericDataReader<uint16_t>(data, format, type);
        reader = &std::get<GenericDataReader<uint16_t>>(reader_v);
        break;
    case GL_UNSIGNED_INT:
        reader_v = GenericDataReader<uint32_t>(data, format, type);
        reader = &std::get<GenericDataReader<uint32_t>>(reader_v);
        break;
    case GL_FLOAT:
        reader_v = GenericDataReader<float>(data, format, type);
        reader = &std::get<GenericDataReader<float>>(reader_v);
        break;
    case GL_UNSIGNED_BYTE_3_3_2:
    case GL_UNSIGNED_BYTE_2_3_3_REV:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        reader_v = CompoundDataReader(data, format, type);
        reader = &std::get<CompoundDataReader>(reader_v);
        break;
    default:
        warning("Unknown texture data type %x\n", type);
    }

    for (int ry = 0; ry < height; ry++) {
        for (int rx = 0; rx < width; rx++) {
            GXColor c = reader->read();
            texel->setColor(c);
            texel->store(dst, x + rx, y + ry, dstpitch);
        }
    }
}

int _ogx_pitch_for_width(uint32_t gx_format, int width)
{
    switch (gx_format) {
    case GX_TF_RGBA8:
        return TexelRGBA8::compute_pitch(width);
    case GX_TF_RGB565:
    case GX_TF_IA8:
        return TexelRGB565::compute_pitch(width);
    case GX_TF_I8:
    case GX_TF_A8:
        return TexelI8::compute_pitch(width);
    default:
        return -1;
    }
}

uint32_t _ogx_gl_format_to_gx(GLenum format)
{
    switch (format) {
    case 3:
    case GL_RGB:
    case GL_BGR:
    case GL_RGB4:
    case GL_RGB5:
    case GL_RGB8:
        return GX_TF_RGB565;
    case 4:
    case GL_RGBA:
    case GL_BGRA:
    case GL_COMPRESSED_RGBA_ARB: /* No support for compressed alpha textures */
        return GX_TF_RGBA8;
    case GL_LUMINANCE_ALPHA: return GX_TF_IA8;
    case GL_LUMINANCE: return GX_TF_I8;
    case GL_ALPHA:
        /* Note, we won't be really passing this to GX */
        return GX_TF_A8;
    default:
        return GX_TF_CMPR;
    }
}

#define DEFINE_FAST_CONVERSION(reader, texel) \
    static void fast_conv_##reader##_##texel( \
        const void *data, GLenum type, int width, int height, \
        void *dst, int x, int y, int dstpitch) \
    { \
        load_texture<DataReader ## reader, Texel ## texel>( \
            data, type, width, height, dst, x, y, dstpitch); \
    } \
    uintptr_t ogx_fast_conv_##reader##_##texel = (uintptr_t)fast_conv_##reader##_##texel;

/* Fast conversions marked by a star are enabled by default */
DEFINE_FAST_CONVERSION(RGBA, I8)
DEFINE_FAST_CONVERSION(RGBA, A8)
DEFINE_FAST_CONVERSION(RGBA, IA8)
DEFINE_FAST_CONVERSION(RGBA, RGB565)
DEFINE_FAST_CONVERSION(RGBA, RGBA8) // *
DEFINE_FAST_CONVERSION(RGB, I8)
DEFINE_FAST_CONVERSION(RGB, IA8)
DEFINE_FAST_CONVERSION(RGB, RGB565) // *
DEFINE_FAST_CONVERSION(RGB, RGBA8)
DEFINE_FAST_CONVERSION(LA, I8)
DEFINE_FAST_CONVERSION(LA, A8)
DEFINE_FAST_CONVERSION(LA, IA8)
DEFINE_FAST_CONVERSION(Intensity, I8) // *
DEFINE_FAST_CONVERSION(Alpha, A8)

void ogx_register_tex_conversion(GLenum format, GLenum internal_format,
                                 uintptr_t converter)
{
    uint8_t gx_format = _ogx_gl_format_to_gx(internal_format);
    int i;
    for (i = 0; i < MAX_FAST_CONVERSIONS; i++) {
        if (s_registered_conversions[i].gl_format == 0) break;
    }

    if (i >= MAX_FAST_CONVERSIONS) {
        /* Nothing especially bad happens, we'll just use the slower
         * conversion. But print a warning in any case. */
        warning("ogx_register_tex_conversion: reached max num of entries");
        return;
    }

    FastConversion &c = s_registered_conversions[i];
    c.gl_format = format;
    c.gx_format = gx_format;
    c.conv.id = converter;
}
