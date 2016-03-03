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

// No warranty, this is only good enough to execute the svg test
double atof(const char *nptr) {
    double whole = 0.0;
    double frac = 0.0;
    const char *str = nptr;
    int mult = 1;

    if (*str == '+') {
        str++;
    } else if (*str == '-') {
        str++;
        mult = -1;
    }

    for (char c = *str; c != '.'; c = *++str) {
        if (c == '\0') {
            return mult * whole;
        }
        if (c >= '0' && c <= '9') {
            whole = whole * 10 + (c - '0');
        }
    }

    for (char c = *++str; c != '\0'; c = *++str) {
        if (c >= '0' && c <= '9') {
            frac += 1.0f / ('c' - '0');
        }
    }

    return mult * (whole + frac);
}
