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

#ifndef _IPMI_API_UTIL_H
#define _IPMI_API_UTIL_H

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

#include "ipmi-api-defs.h"

  void api_set_api_errnum_by_errno (ipmi_ctx_t ctx, int __errno);

  void api_set_api_errnum_by_fiid_object (ipmi_ctx_t ctx, fiid_obj_t obj);

  void api_set_api_errnum_by_bad_response (ipmi_ctx_t ctx, fiid_obj_t obj_cmd_rs);

  void api_set_api_errnum_by_locate_errnum (ipmi_ctx_t ctx, int locate_errnum);

  void api_set_api_errnum_by_kcs_errnum (ipmi_ctx_t ctx, int kcs_errnum);

  void api_set_api_errnum_by_ssif_errnum (ipmi_ctx_t ctx, int ssif_errnum);

  void api_set_api_errnum_by_openipmi_errnum (ipmi_ctx_t ctx, int openipmi_errnum);

  void api_set_api_errnum_by_sunbmc_errnum (ipmi_ctx_t ctx, int sunbmc_errnum);

  int api_fiid_obj_packet_valid (ipmi_ctx_t ctx, fiid_obj_t obj);

  int api_fiid_obj_template_compare (ipmi_ctx_t ctx, fiid_obj_t obj, fiid_template_t tmpl);

  int api_fiid_obj_get (ipmi_ctx_t ctx, fiid_obj_t obj, char *field, uint64_t *val);

  int api_ipmi_cmd (ipmi_ctx_t ctx,
            uint8_t lun,
            uint8_t net_fn,
            fiid_obj_t obj_cmd_rq,
            fiid_obj_t obj_cmd_rs);

  int api_ipmi_cmd_ipmb (ipmi_ctx_t ctx,
             uint8_t rs_addr,
             uint8_t lun,
             uint8_t net_fn,
             fiid_obj_t obj_cmd_rq,
             fiid_obj_t obj_cmd_rs);

#ifdef __cplusplus
}
#endif

#endif /* ipmi-api-util.h */

