
/*
 * Copyright (c) 2016 Craig Stout cstout@chromium.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "gm.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

#include <SkPath.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkDevice.h>

#include "svg_cube.cpp"

void skiaRasterize(SkCanvas* canvas, NSVGimage* image, float tx, float ty, float scale, float rotate)
{
    NSVGshape *shape = NULL;
    bool anti_alias = false;

    for (shape = image->shapes; shape != NULL; shape = shape->next) {
        SkPaint paint[2];
        SkPaint *fill_paint = NULL;
        SkPaint *stroke_paint = NULL;

        if (!(shape->flags & NSVG_FLAGS_VISIBLE))
            continue;

        switch (shape->fill.type) {
            case NSVG_PAINT_NONE:
                break;
            case NSVG_PAINT_COLOR:
                fill_paint = &paint[0];
                fill_paint->reset();
                fill_paint->setStyle(SkPaint::Style::kFill_Style);
                fill_paint->setColor(shape->fill.color);
                fill_paint->setAntiAlias(anti_alias);
                break;
            default:
                SkDebugf("Unhandled fill type\n");
        }
        
        switch (shape->stroke.type) {
            case NSVG_PAINT_NONE:
                break;
            case NSVG_PAINT_COLOR:
                stroke_paint = &paint[1];
                stroke_paint->reset();
                stroke_paint->setStyle(SkPaint::Style::kStroke_Style);
                stroke_paint->setColor(shape->stroke.color);
                stroke_paint->setStrokeWidth(shape->strokeWidth);
                stroke_paint->setAntiAlias(anti_alias);
                break;
            default:
                SkDebugf("Unhandled stroke type\n");
        }

        // nanosvg converts everything to a path composed of cubic beziers
        for (NSVGpath *path = shape->paths; path; path = path->next) {
            SkPath skpath;
            //SkDebugf("got path %p with npts %d fill %p stroke %p\n", path, path->npts, fill_paint, stroke_paint);
            skpath.moveTo(path->pts[0], path->pts[1]);
            for (int ptindex = 1; ptindex < path->npts; ptindex += 3) {
                const float *p = &path->pts[ptindex * 2];
                skpath.cubicTo(p[0], p[1], p[2], p[3], p[4], p[5]);
            }
            if (path->closed) {
                skpath.close();
            }

            if (fill_paint) {
                canvas->drawPath(skpath, *fill_paint);
            }
            if (stroke_paint) {
                canvas->drawPath(skpath, *stroke_paint);
            }
        }
    }
}


class SvgTest {
public:
    SvgTest();
    ~SvgTest();

    int test(SkCanvas* canvas);

private:
    static NSVGimage *parse();

    int run_time_ms_ = 0;
    NSVGimage *image_ = nullptr;
};

SvgTest::SvgTest() {
    image_ = parse();
}

SvgTest::~SvgTest() {
    if (image_) {
        nsvgDelete(image_);
    }
}

NSVGimage *SvgTest::parse()
{   
    char* svg_xml = new char[sizeof(svg_cube)];
    memcpy(svg_xml, svg_cube, sizeof(svg_cube));

    NSVGimage *image = nsvgParse(svg_xml, "px", 96.0f);

    delete [] svg_xml;

    return image;
}

int SvgTest::test(SkCanvas* canvas)
{
    if (image_ == NULL) {
        printf("Could not open SVG image.\n");
        return -1;
    }

    canvas->drawColor(SK_ColorWHITE);

    int w = canvas->getDevice()->width();
    int h = canvas->getDevice()->height();

    float rotate = 0;
    float scaleh = (float) h / (int)image_->height;
    float scalew = (float) w / (int)image_->width;
    float scale = scaleh < scalew ? scaleh : scalew;

    printf("rasterizing image %d x %d scale %f\n", (int)image_->width, (int)image_->height, scale);

    if (rotate) {
        canvas->translate(w/2, h/2);
        canvas->rotate(rotate);
        canvas->translate(-w/2, -h/2);
    }

    if (scale != 1.0) {
        canvas->scale(scale, scale);
    }
    
    skiaRasterize(canvas, image_, 0,0, scale, rotate);

    return 0;
}


namespace skiagm {

class SvgGM : public GM {
    SvgTest test;
public:
    SvgGM() {
    }
    
protected:
    virtual SkString onShortName() {
        return SkString("svg");
    }

    virtual SkISize onISize() {
        return make_isize(128, 128);
    }

    virtual void onDraw(SkCanvas* canvas) {
        test.test(canvas);
    }
    
private:
    typedef GM INHERITED;
};

//////////////////////////////////////////////////////////////////////////////

static GM* MyFactory(void*) { return new SvgGM; }
static GMRegistry reg(MyFactory);

}
