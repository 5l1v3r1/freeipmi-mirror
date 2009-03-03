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

#ifndef _IPMI_RMCPPLUS_SUPPORT_AND_PAYLOAD_CMDS_API_H
#define _IPMI_RMCPPLUS_SUPPORT_AND_PAYLOAD_CMDS_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freeipmi/api/ipmi-api.h>
#include <freeipmi/fiid/fiid.h>

  int8_t ipmi_cmd_set_user_payload_access (ipmi_ctx_t ctx,
                       uint8_t channel_number,
                       uint8_t user_id,
                       uint8_t operation,
                       uint8_t standard_payload_1,
                       uint8_t standard_payload_2,
                       uint8_t standard_payload_3,
                       uint8_t standard_payload_4,
                       uint8_t standard_payload_5,
                       uint8_t standard_payload_6,
                       uint8_t standard_payload_7,
                       uint8_t oem_payload_0,
                       uint8_t oem_payload_1,
                       uint8_t oem_payload_2,
                       uint8_t oem_payload_3,
                       uint8_t oem_payload_4,
                       uint8_t oem_payload_5,
                       uint8_t oem_payload_6,
                       uint8_t oem_payload_7,
                       fiid_obj_t obj_cmd_rs);

  int8_t ipmi_cmd_get_user_payload_access (ipmi_ctx_t ctx,
                       uint8_t channel_number,
                       uint8_t user_id,
                       fiid_obj_t obj_cmd_rs);


#ifdef __cplusplus
}
#endif

#endif /* _IPMI_RMCPPLUS_SUPPORT_AND_PAYLOAD_CMDS_API_H */


