#include <SkSVGParser.h>
#include <SkStream.h>

int main (int argc, char * const argv[]) {
    SkSVGParser parser;
    SkFILEStream stream("/home/cstout/Downloads/tiger.svg");

    if (!stream.isValid()) {
        SkDebugf("Stream invalid, file not found?\n");
        return 1;
    }

    if (!parser.parse(stream)) {
        SkDebugf("Error parsing stream\n");
        return 1;
    }

    SkTDict<SkSVGElement*>& ids = parser.getIDs();
    SkDebugf("Parser returned %d ids\n", ids.count());

    return 0;
}
