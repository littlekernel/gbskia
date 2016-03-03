/* 
** Copyright 2016 Craig Stout cstout@chromium.org
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "SkCoreBlitters.h"
#include "SkColorPriv.h"

SkRGB111_Blitter::SkRGB111_Blitter(const SkBitmap& device, const SkPaint& paint)
    : INHERITED(device)
{
    // We don't handle alpha blending
    SkASSERT(SkColorGetA(paint.getColor()) == 0xFF);

    // We also do not handle dithering or shaders or xfer modes :)
    SkColor c = paint.getColor();
    uint8_t r = SkColorGetR(c) >> 7;
    uint8_t g = SkColorGetG(c) >> 7;
    uint8_t b = SkColorGetB(c) >> 7;
    fVal = SkPackRGB111(r, g, b);
    fRep = fVal | (fVal << 4);
}

void SkRGB111_Blitter::blitH(int x, int y, int width)
{
    SkASSERT(x >= 0 && y >= 0 && x + width <= fDevice.width());

    uint8_t* dst = fDevice.getAddr4(x, y);

    if (x & 1) {
        *dst = (*dst & 0xF) | (fVal << 4);
        dst++;
        --width;
    }

    int nbytes = width >> 1;
    memset(dst, fRep, nbytes);

    if (width & 1) {
        dst += nbytes;
        *dst = (*dst & 0xF0) | fVal;
    }
}

void SkRGB111_Blitter::blitAntiH(int x, int y, const SkAlpha antialias[],
                                 const int16_t runs[]) {
    // Not implemented
}
