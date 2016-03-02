
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

#include "gm.h"

#include <SkDevice.h>
#include <SkCanvas.h>
#include <svg_test.h>
#include <nanosvg.h>

namespace skiagm {

class SvgGM : public GM {
public:
    SvgGM() {
        image_ = SvgTest::parse();
    }

    ~SvgGM() {
        if (image_) {
            nsvgDelete(image_);
        }
    }   

protected:
    virtual SkString onShortName() {
        return SkString("svg");
    }

    virtual SkISize onISize() {
        return make_isize(128, 128);
    }

    virtual void onDraw(SkCanvas* canvas) {
        if (image_ == NULL) {
            printf("Could not open SVG image.\n");
            return;
        }

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
        
        SvgTest::draw(canvas, image_, false);
    }
    
private:
    NSVGimage *image_ = nullptr;

    typedef GM INHERITED;
};

//////////////////////////////////////////////////////////////////////////////

static GM* MyFactory(void*) { return new SvgGM; }
static GMRegistry reg(MyFactory);

}
