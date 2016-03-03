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
#include "SkColor.h"

SkBW_Blitter::SkBW_Blitter(const SkBitmap& device, const SkPaint& paint)
    : INHERITED(device)
{
    // We don't handle alpha blending.
    SkASSERT(SkColorGetA(paint.getColor()) == 0xFF);
    fVal = paint.getColor() == SK_ColorBLACK ? 0 : 1;
}

void SkBW_Blitter::blitH(int x, int y, int width)
{
    SkASSERT(x >= 0 && y >= 0 && (unsigned)(x + width) <= (unsigned)fDevice.width());
    //SkDebugf("SkBW_Blitter::blitH x %d y %d width %d fVal %d\n", x, y, width, fVal);

    uint8_t* dst = fDevice.getAddr1(x, y);
    int right = x + width;

    int left_mask = 0xFF << (x & 7);
    int rite_mask = 0xFF >> (8 - (right & 7));
    int full_runs = (right >> 3) - ((x + 7) >> 3);

    // check for empty right mask, so we don't read off the end (or go slower than we need to)
    if (rite_mask == 0) {
        SkASSERT(full_runs >= 0);
        full_runs -= 1;
        rite_mask = 0xFF;
    }
    if (left_mask == 0xFF) {
        full_runs -= 1;
    }

    if (full_runs < 0) {
        SkASSERT((left_mask & rite_mask) != 0);
        if (fVal) {
            *dst |= (left_mask & rite_mask);
        } else {
            *dst &= ~(left_mask & rite_mask);
        }
    } else if (fVal) {
        *dst++ |= left_mask;
        memset(dst, 0xFF, full_runs);
        dst += full_runs;
        *dst |= rite_mask;
    } else {
        *dst++ &= ~left_mask;
        memset(dst, 0, full_runs);
        dst += full_runs;
        *dst &= ~rite_mask;
    }
}

void SkBW_Blitter::blitAntiH(int x, int y, const SkAlpha antialias[],
                             const int16_t runs[]) {
    // Not implemented
}
