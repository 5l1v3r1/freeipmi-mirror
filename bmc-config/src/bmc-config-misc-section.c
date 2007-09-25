#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "bmc-config.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-map.h"
#include "bmc-config-validate.h"

static config_err_t
power_restore_policy_checkout (const char *section_name,
			       struct config_keyvalue *kv,
                               void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t policy;
  config_err_t ret;

  if ((ret = get_bmc_power_restore_policy (state_data,
                                           &policy)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (!(kv->value = strdup (power_restore_policy_string (policy))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
power_restore_policy_commit (const char *section_name,
			     const struct config_keyvalue *kv,
                             void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  return set_bmc_power_restore_policy (state_data,
				       power_restore_policy_number (kv->value));
}

static config_diff_t
power_restore_policy_diff (const char *section_name,
			   const struct config_keyvalue *kv,
                           void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t got_value;
  uint8_t passed_value;
  config_err_t rc;
  config_diff_t ret;
  
  if ((rc = get_bmc_power_restore_policy (state_data,
                                          &got_value)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_value = power_restore_policy_number (kv->value);

  if (passed_value == got_value)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section_name,
                   kv->key_name,
                   kv->value,
                   power_restore_policy_string (got_value));
    }

  return ret;
}

struct config_section *
bmc_config_misc_section_get (bmc_config_state_data_t *state_data)
{
  struct config_section *misc_section = NULL;
  char *section_comment = 
    "The following miscellaneous configuration options are optionally "
    "implemented by the vendor.  They may not be available your system and "
    "may not be visible below."
    "\n"
    "The \"Power_Restore_Policy\" determines the behavior of the machine "
    "when AC power returns after a power loss.  The behavior can be set to "
    "always power on the machine (\"On_State_AC_Apply\"), power off the "
    "machine (\"Off_State_AC_Apply\"), or return the power to the state that "
    "existed before the power loss (\"Restore_State_AC_Apply\").";

  if (!(misc_section = config_section_create ("Misc",
                                                  "Misc",
                                                  section_comment,
                                                  0)))
    goto cleanup;

  if (config_section_add_keyvalue (misc_section,
                                       "Power_Restore_Policy",
                                       "Possible values: Off_State_AC_Apply/Restore_State_AC_Apply/On_State_AC_Apply",
                                       CONFIG_CHECKOUT_KEY_COMMENTED_OUT_IF_VALUE_EMPTY,
                                       power_restore_policy_checkout,
                                       power_restore_policy_commit,
                                       power_restore_policy_diff,
                                       power_restore_policy_number_validate) < 0)
    goto cleanup;

  return misc_section;

 cleanup:
  if (misc_section)
    config_section_destroy(misc_section);
  return NULL;
}
