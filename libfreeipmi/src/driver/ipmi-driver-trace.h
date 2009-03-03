/*
  Copyright (C) 2003-2009 FreeIPMI Core Team

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.

*/

#ifndef _IPMI_DRIVER_TRACE_H
#define _IPMI_DRIVER_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>

#include "freeipmi/api/ipmi-api.h"
#include "freeipmi/fiid/fiid.h"
#include "freeipmi/util/ipmi-error-util.h"
#include "freeipmi/util/ipmi-util.h"

#include "libcommon/ipmi-trace.h"

#define KCS_SET_ERRNUM(__ctx, __errnum)                \
  do {                                \
    (__ctx)->errnum = (__errnum);                \
    __MSG_TRACE (ipmi_kcs_ctx_errormsg ((__ctx)), (__errnum));    \
  } while (0)

#define KCS_ERRNO_TO_KCS_ERRNUM(__ctx, __errno)        \
  do {                            \
    _set_kcs_ctx_errnum_by_errno ((__ctx), (__errno));    \
    __ERRNO_TRACE (__errno);                \
  } while (0)

#define KCS_FIID_OBJECT_ERROR_TO_KCS_ERRNUM(__ctx, __obj)                 \
  do {                                                                    \
    _set_kcs_errnum_by_fiid_object ((__ctx), (__obj));              \
    __MSG_TRACE (fiid_obj_errormsg ((__obj)), fiid_obj_errnum ((__obj))); \
  } while (0)

#define SSIF_SET_ERRNUM(__ctx, __errnum)            \
  do {                                \
    (__ctx)->errnum = (__errnum);                \
    __MSG_TRACE (ipmi_ssif_ctx_errormsg ((__ctx)), (__errnum));    \
  } while (0)

#define SSIF_ERRNO_TO_SSIF_ERRNUM(__ctx, __errno)    \
  do {                            \
    _set_ssif_ctx_errnum_by_errno ((__ctx), (__errno));    \
    __ERRNO_TRACE (__errno);                \
  } while (0)

#define SSIF_FIID_OBJECT_ERROR_TO_SSIF_ERRNUM(__ctx, __obj)               \
  do {                                                                    \
    _set_ssif_errnum_by_fiid_object ((__ctx), (__obj));              \
    __MSG_TRACE (fiid_obj_errormsg ((__obj)), fiid_obj_errnum ((__obj))); \
  } while (0)

#define OPENIPMI_SET_ERRNUM(__ctx, __errnum)                            \
  do {                                                                  \
    (__ctx)->errnum = (__errnum);                                       \
    __MSG_TRACE (ipmi_openipmi_ctx_errormsg ((__ctx)), (__errnum));    \
  } while (0)

#define OPENIPMI_ERRNO_TO_OPENIPMI_ERRNUM(__ctx, __errno)    \
  do {                                \
    _set_openipmi_ctx_errnum_by_errno ((__ctx), (__errno));    \
    __ERRNO_TRACE (__errno);                    \
  } while (0)

#define SUNBMC_SET_ERRNUM(__ctx, __errnum)                              \
  do {                                                                  \
    (__ctx)->errnum = (__errnum);                                       \
    __MSG_TRACE (ipmi_sunbmc_ctx_errormsg ((__ctx)), (__errnum));    \
  } while (0)

#define SUNBMC_ERRNO_TO_SUNBMC_ERRNUM(__ctx, __errno)        \
  do {                                \
    _set_sunbmc_ctx_errnum_by_errno ((__ctx), (__errno));    \
    __ERRNO_TRACE (__errno);                    \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif /* ipmi-driver-trace.h */

