/*
 * Copyright (C) 2003-2015 FreeIPMI Core Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef IPMI_LAN_CONFIGURATION_PARAMETERS_OEM_INVENTEC_SPEC_H
#define IPMI_LAN_CONFIGURATION_PARAMETERS_OEM_INVENTEC_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Inventec 5441/Dell Xanadu II
 * Inventec 5442/Dell Xanadu III
 */

#define IPMI_LAN_CONFIGURATION_PARAMETER_OEM_INVENTEC_DHCP_RETRY 192

/* w/ IPMI_LAN_CONFIGURATION_PARAMETER_OEM_INVENTEC_DHCP_RETRY */
#define IPMI_LAN_CONFIGURATION_PARAMETER_OEM_INVENTEC_DHCP_RETRY_NO_RETRIES       0x00
#define IPMI_LAN_CONFIGURATION_PARAMETER_OEM_INVENTEC_DHCP_RETRY_INDEFINITE_RETRY 0xFF

#ifdef __cplusplus
}
#endif

#endif /* IPMI_LAN_CONFIGURATION_PARAMETERS_INVENTEC_SPEC_H */
