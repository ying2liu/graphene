/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2014 Stony Brook University */

/*
 * This file contains APIs to create, exit and yield a thread.
 */

#include "api.h"
#include "pal.h"
#include "pal_debug.h"
#include "pal_defs.h"
#include "pal_error.h"
#include "pal_internal.h"

/* PAL call DkThreadCreate: create a thread inside the current process */
PAL_HANDLE DkThreadCreate(PAL_PTR addr, PAL_PTR param) {
    PAL_HANDLE handle = NULL;
    int ret = _DkThreadCreate(&handle, (int (*)(void*))addr, (const void*)param);

    if (ret < 0) {
        _DkRaiseFailure(-ret);
        handle = NULL;
    }

    return handle;
}

/* PAL call DkThreadDelayExecution. Delay the current thread (sleep) for the given duration */
PAL_NUM DkThreadDelayExecution(PAL_NUM duration) {
    unsigned long dur = duration;
    int ret = _DkThreadDelayExecution(&dur);

    if (ret < 0) {
        _DkRaiseFailure(PAL_ERROR_INTERRUPTED);
        duration = dur;
    }

    return duration;
}

/* PAL call DkThreadYieldExecution. Yield the execution of the current thread. */
void DkThreadYieldExecution(void) {
    _DkThreadYieldExecution();
}

/* PAL call DkThreadExit: simply exit the current thread no matter what */
noreturn void DkThreadExit(PAL_PTR clear_child_tid) {
    _DkThreadExit((int*)clear_child_tid);
    /* UNREACHABLE */
}

/* PAL call DkThreadResume: resume the execution of a thread which is delayed before */
PAL_BOL DkThreadResume(PAL_HANDLE threadHandle) {
    if (!threadHandle || !IS_HANDLE_TYPE(threadHandle, thread)) {
        _DkRaiseFailure(PAL_ERROR_INVAL);
        return PAL_FALSE;
    }

    int ret = _DkThreadResume(threadHandle);

    if (ret < 0) {
        _DkRaiseFailure(PAL_ERROR_DENIED);
        return PAL_FALSE;
    }

    return PAL_TRUE;
}

PAL_BOL DkThreadSetCpuAffinity(PAL_HANDLE thread, PAL_NUM cpumask_size, PAL_PTR cpu_mask) {
    int ret = _DkThreadSetCpuAffinity(thread, cpumask_size, cpu_mask);

    if (ret < 0) {
        _DkRaiseFailure(-ret);
        return PAL_FALSE;
    }

    return PAL_TRUE;
}

PAL_BOL DkThreadGetCpuAffinity(PAL_HANDLE thread, PAL_NUM cpumask_size, PAL_PTR cpu_mask) {
    int ret = _DkThreadGetCpuAffinity(thread, cpumask_size, cpu_mask);

    if (ret < 0) {
        _DkRaiseFailure(-ret);
        return PAL_FALSE;
    }

    return PAL_TRUE;
}
