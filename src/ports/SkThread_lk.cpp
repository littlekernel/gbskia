#include "SkThread.h"

#include <kernel/mutex.h>

SkMutex gAtomicMutex;

int32_t sk_atomic_inc(int32_t* addr)
{
    SkAutoMutexAcquire ac(gAtomicMutex);

    int32_t value = *addr;
    *addr = value + 1;
    return value;
}

int32_t sk_atomic_dec(int32_t* addr)
{
    SkAutoMutexAcquire ac(gAtomicMutex);
    
    int32_t value = *addr;
    *addr = value - 1;
    return value;
}

//////////////////////////////////////////////////////////////////////////////

static void print_error(status_t status)
{
    if (status != 0) {
        SkDebugf("SkMutex: status error (%d)\n", status);
    }
}

SkMutex::SkMutex(bool isGlobal) : fIsGlobal(isGlobal)
{
    if (sizeof(mutex_t) > sizeof(fStorage))
    {
        SkDEBUGF(("mutex size = %d\n", sizeof(mutex_t)));
        SkASSERT(!"mutex storage is too small");
    }

    mutex_init((mutex_t*)fStorage);
}

SkMutex::~SkMutex()
{
  mutex_destroy((mutex_t*)fStorage);
}

void SkMutex::acquire()
{
  status_t status = mutex_acquire((mutex_t*)fStorage);
  print_error(status);
  SkASSERT(0 == status);
}

void SkMutex::release()
{
  status_t status = mutex_release((mutex_t*)fStorage);
  print_error(status);
  SkASSERT(0 == status);
}

