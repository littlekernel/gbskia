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

#include "svg_test.h"

// Required by nanosvg parser
extern "C" double atof(const char *nptr);
extern "C" int sscanf(const char *ibuf, const char *fmt, ...);

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

#include <SkPath.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkDevice.h>
#include <SkColor.h>

#include "svg_cube.cpp"

NSVGimage *SvgTest::parse()
{   
    char* svg_xml = new char[sizeof(svg_cube)];
    memcpy(svg_xml, svg_cube, sizeof(svg_cube));

    NSVGimage *image = nsvgParse(svg_xml, "px", 96.0f);

    delete [] svg_xml;

    return image;
}

void SvgTest::draw(SkCanvas* canvas, NSVGimage* image, bool anti_alias)
{
    canvas->drawColor(SK_ColorWHITE);

    for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
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

