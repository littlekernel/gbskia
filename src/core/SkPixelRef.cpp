#include "SkPixelRef.h"
#include "SkFlattenable.h"
#include "SkThread.h"

static SkMutex  gPixelRefMutex;
static int32_t  gPixelRefGenerationID;

SkPixelRef::SkPixelRef(SkMutex* mutex) {
    if (NULL == mutex) {
        mutex = &gPixelRefMutex;
    }
    fMutex = mutex;
    fPixels = NULL;
    fColorTable = NULL; // we do not track ownership of this
    fLockCount = 0;
    fGenerationID = 0;  // signal to rebuild
    fIsImmutable = false;
#ifdef SK_FEATURE_STRING
    fURI = new SkString();
#else
    fURI = nullptr;
#endif // SK_FEATURE_STRING
}

SkPixelRef::SkPixelRef(SkFlattenableReadBuffer& buffer, SkMutex* mutex) {
    if (NULL == mutex) {
        mutex = &gPixelRefMutex;
    }
    fMutex = mutex;
    fPixels = NULL;
    fColorTable = NULL; // we do not track ownership of this
    fLockCount = 0;
    fGenerationID = 0;  // signal to rebuild
    fIsImmutable = buffer.readBool();
#ifdef SK_FEATURE_STRING
    fURI = new SkString();
#else
    fURI = nullptr;
#endif // SK_FEATURE_STRING
}

void SkPixelRef::flatten(SkFlattenableWriteBuffer& buffer) const {
    buffer.writeBool(fIsImmutable);
}

void SkPixelRef::lockPixels() {
    SkAutoMutexAcquire  ac(*fMutex);
    
    if (1 == ++fLockCount) {
        fPixels = this->onLockPixels(&fColorTable);
    }
}

void SkPixelRef::unlockPixels() {
    SkAutoMutexAcquire  ac(*fMutex);
    
    SkASSERT(fLockCount > 0);
    if (0 == --fLockCount) {
        this->onUnlockPixels();
        fPixels = NULL;
        fColorTable = NULL;
    }
}

uint32_t SkPixelRef::getGenerationID() const {
    uint32_t genID = fGenerationID;
    if (0 == genID) {
        // do a loop in case our global wraps around, as we never want to
        // return a 0
        do {
            genID = sk_atomic_inc(&gPixelRefGenerationID) + 1;
        } while (0 == genID);
        fGenerationID = genID;
    }
    return genID;
}

void SkPixelRef::notifyPixelsChanged() {
#ifdef SK_DEBUG
    if (fIsImmutable) {
        SkDebugf("========== notifyPixelsChanged called on immutable pixelref");
    }
#endif
    // this signals us to recompute this next time around
    fGenerationID = 0;
}

void SkPixelRef::setImmutable() {
    fIsImmutable = true;
}

///////////////////////////////////////////////////////////////////////////////

#define MAX_PAIR_COUNT  16

struct Pair {
    const char*          fName;
    SkPixelRef::Factory  fFactory;
};

static int gCount;
static Pair gPairs[MAX_PAIR_COUNT];

void SkPixelRef::Register(const char name[], Factory factory) {
    SkASSERT(name);
    SkASSERT(factory);
    
    static bool gOnce;
    if (!gOnce) {
        gCount = 0;
        gOnce = true;
    }
    
    SkASSERT(gCount < MAX_PAIR_COUNT);
    
    gPairs[gCount].fName = name;
    gPairs[gCount].fFactory = factory;
    gCount += 1;
}

SkPixelRef::Factory SkPixelRef::NameToFactory(const char name[]) {
    const Pair* pairs = gPairs;
    for (int i = gCount - 1; i >= 0; --i) {
        if (strcmp(pairs[i].fName, name) == 0) {
            return pairs[i].fFactory;
        }
    }
    return NULL;
}

const char* SkPixelRef::FactoryToName(Factory fact) {
    const Pair* pairs = gPairs;
    for (int i = gCount - 1; i >= 0; --i) {
        if (pairs[i].fFactory == fact) {
            return pairs[i].fName;
        }
    }
    return NULL;
}

/** Return the optional URI string associated with this pixelref. May be
        null.
*/
const char* SkPixelRef::getURI() const {
#ifdef SK_FEATURE_STRING
    return fURI->size() ? fURI->c_str() : NULL;
#else
    SK_FEATURE_REMOVED("SK_FEATURE_STRING")
    return nullptr;
#endif
}

/** Copy a URI string to this pixelref, or clear the URI if the uri is null
 */
void SkPixelRef::setURI(const char uri[]) {
#ifdef SK_FEATURE_STRING
    fURI->set(uri);
#else
    SK_FEATURE_REMOVED("SK_FEATURE_STRING")
#endif // SK_FEATURE_STRING
}

/** Copy a URI string to this pixelref
 */
void SkPixelRef::setURI(const char uri[], size_t len) {
#ifdef SK_FEATURE_STRING
    fURI->set(uri, len);
#else
    SK_FEATURE_REMOVED("SK_FEATURE_STRING")
#endif // SK_FEATURE_STRING
}

/** Assign a URI string to this pixelref.
*/
void SkPixelRef::setURI(const SkString& uri) {
#ifdef SK_FEATURE_STRING
    fURI = uri;
#else
    SK_FEATURE_REMOVED("SK_FEATURE_STRING")
#endif // SK_FEATURE_STRING
}
