#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/mman.h>

#ifdef STDC_HEADERS
#include <string.h>
#endif

#include <limits.h>
#include "freeipmi.h"

/* SMBIOS Reference Specification: map area between 000f0000 and
   000fffff.  The IPMI Entry Structure begins on a 16-byte boundary,
   with a 4 byte "_SM_" signature.  */


/* is_ipmi_entry
   ARGUMENTS:
   sigp = points to start of purported SMBIOS entry structure
   RETURNS:
   0 = not really a SMBIOS entry structure
   1 = yes, a real SMBIOS entry structure */ 
static int
is_ipmi_entry (u_int8_t* sigp)
{
  static const char smbios_entry_sig[4] = { '_', 'S', 'M', '_' };
  static const char smbios_entry_anchor[5] = { '_', 'D', 'M', 'I', '_' };
  u_int32_t csum_computed;
  u_int8_t csum_given;
  u_int8_t entry_len;
  u_int8_t* bp;

  if (memcmp (sigp, smbios_entry_sig, sizeof (smbios_entry_sig)) != 0)
    return 0;

  entry_len = sigp[SMBIOS_ENTRY_LEN_OFFSET];

  csum_given = sigp[SMBIOS_ENTRY_CSUM_OFFSET];
  csum_computed = 0;
  for (bp = sigp; bp < sigp + entry_len; bp++)
    csum_computed = (csum_computed + (*bp)) % (1 << CHAR_BIT);

  if (memcmp (sigp + SMBIOS_ENTRY_ANCHOR_OFFSET, smbios_entry_anchor,
	      sizeof (smbios_entry_anchor)) != 0)
    return 0;

  csum_given = sigp[SMBIOS_ENTRY_ANCHOR_CSUM_OFFSET];
  csum_computed = 0;
  for (bp = sigp + SMBIOS_ENTRY_ANCHOR_CSUM_OFFSET; bp < sigp + entry_len; bp++)
    csum_computed = (csum_computed + (*bp)) % (1 << CHAR_BIT);
    
  return 1;

}


/* is_ipmi_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   dev_info_p = points to start of purported IPMI device info structure
   RETURNS:
   0 = not a IPMI device info structure for TYPE
   1 = yes, IPMI device info structure for TYPE */
static int
is_ipmi_dev_info (ipmi_interface_t type, u_int8_t* dev_info_p)
{
  if (*dev_info_p != SMBIOS_IPMI_DEV_INFO_SIG)
    return 0;

  if (dev_info_p[SMBIOS_IPMI_DEV_INFO_TYPE_OFFSET] != type)
    return 0;

  return 1;
}

/* map_physmem
   ARGUMENTS:
   physaddr = physical address to access
   len = length of area to access
   startp = place to store pointer to unmap (caller responsible for unmapping)
   totallen = length of area to unmap
   RETURNS:
   pointer to area of physical memory at physmem */
static u_int8_t*
map_physmem (u_int32_t physaddr, size_t len, void** startp, size_t* totallen)
{
  u_int32_t startaddr;
  u_int32_t pad;
  int mem_fd;

  if (startp == NULL || totallen == NULL)
    return NULL;

  mem_fd = open ("/dev/mem", O_RDONLY);

  if (mem_fd != -1)
    {
      pad = physaddr % sysconf (_SC_PAGESIZE);
      startaddr = physaddr - pad;
      *totallen = len + pad;
      *startp = mmap (NULL, *totallen, PROT_READ, MAP_PRIVATE, mem_fd, startaddr);
      close (mem_fd);

      if (*startp != MAP_FAILED)
	return ((u_int8_t*)(*startp) + pad);
    }
  return NULL;	  
}

/* copy_impi_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   statusp = optional (NULL allowed) pointer to store status
   information: 1 - structure not found, -1 - error, 0 - success
   RETURNS:
   pointer to the device info structure in heap (caller responsible
   for freeing */
static u_int8_t*
copy_impi_dev_info (ipmi_interface_t type, int* statusp)
{
  int status;
  u_int8_t* result = NULL;
  void* map_entry;
  size_t map_entry_len;
  u_int8_t* pmem_entry; 

  status = 1;
  pmem_entry = map_physmem (SMBIOS_AREA_START, SMBIOS_AREA_LEN, &map_entry, &map_entry_len);
  if (pmem_entry != NULL)
    {
      u_int8_t* sigp;
      for (sigp = pmem_entry; sigp - pmem_entry < SMBIOS_AREA_LEN; sigp += SMBIOS_AREA_ALIGN)
	{
	  if (is_ipmi_entry (sigp))
	    {
	      u_int16_t s_table_len;
	      u_int8_t* pmem_table;
	      void* map_table;
	      size_t map_table_len;

	      s_table_len = *(u_int16_t*)(sigp + SMBIOS_ENTRY_TLEN_OFFSET);
	      pmem_table = map_physmem (*(u_int32_t*)(sigp + SMBIOS_ENTRY_PTR_OFFSET), s_table_len,
					&map_table, &map_table_len);
	      if (pmem_table != NULL)
		{
		  u_int8_t* dev_info_p;
		  size_t size;
		  u_int8_t* var_info_p;

		  dev_info_p = pmem_table;
		  size = dev_info_p[SMBIOS_DEV_INFO_LEN_OFFSET];
		  while (dev_info_p - pmem_table < s_table_len)
		    {
		      if (is_ipmi_dev_info (type, dev_info_p))
			{
			  result = ipmi_xmalloc (size);
			  if (result != NULL)
			    {
			      status = 0;
			      memcpy (result, dev_info_p, size);
			    }
			  else
			    {
			      errno = ENOMEM;
			      status = -1;
			    }
			  break;
			}
		      var_info_p = dev_info_p + size;
		      while (var_info_p[0] != 0 || var_info_p[1] != 0)
			var_info_p++;
		      dev_info_p = var_info_p + 2;
		      size = dev_info_p[SMBIOS_DEV_INFO_LEN_OFFSET];
		    }
		  munmap (map_table, map_table_len);
		}
	    }
	  if (status <= 0)
	    break;
	}
      munmap (map_entry, map_entry_len);
    }
  if (statusp != NULL)
    *statusp = status;
  return (status == 0 ? result : NULL);
}

/* get_smbios_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   pinfo = pointer to information structure filled in by this function
   statusp = optional (NULL allowed) pointer to store status
   information: 1 - structure not found, -1 - error, 0 - success
   RETURNS:
   pinfo if successful, NULL otherwise */
ipmi_probe_info_t*
smbios_get_dev_info (ipmi_interface_t type, ipmi_probe_info_t* pinfo, int* statusp)
{
  u_int8_t* bufp;
  u_int8_t version;
  u_int64_t addr;
  u_int64_t strobed;

  bufp = copy_impi_dev_info (type, statusp);
  if (bufp == NULL)
    return NULL;

  pinfo->type = bufp[SMBIOS_IPMI_DEV_INFO_TYPE_OFFSET];
  version = bufp[SMBIOS_IPMI_DEV_INFO_VER_OFFSET];
/*   pinfo->ipmi_major = (version >> 4) & 0xf; */
/*   pinfo->ipmi_minor = version & 0xf; */
/*   pinfo->i2c_slave_addr = bufp[SMBIOS_IPMI_DEV_INFO_I2C_OFFSET]; */
/*   pinfo->nvstor_dev_addr = bufp[SMBIOS_IPMI_DEV_INFO_NVSTOR_OFFSET]; */

  strobed = addr = *(u_int64_t*)(bufp+SMBIOS_IPMI_DEV_INFO_ADDR_OFFSET);

  if (bufp[SMBIOS_DEV_INFO_LEN_OFFSET] > SMBIOS_IPMI_DEV_INFO_MODIFIER_OFFSET)
    {
      u_int8_t modifier;
      u_int8_t lsb;

      modifier = bufp[SMBIOS_IPMI_DEV_INFO_MODIFIER_OFFSET];
      lsb = (modifier >> SMBIOS_LSB_BIT) & 1;
      strobed = (strobed & ~1) | lsb;

/*       pinfo->reg_spacing = (modifier >> SMBIOS_REGSPACING_SHIFT) & SMBIOS_REGSPACING_MASK; */

/*       if (((modifier >> SMBIOS_INTINFO_PRESENT_BIT) & 1) != 0) */
/* 	{ */
/* 	  pinfo->ipmi_int_specified = 1; */
/* 	  pinfo->ipmi_int_polarity = (modifier >> SMBIOS_INTINFO_POLARITY_BIT) & 1; */
/* 	  pinfo->ipmi_int_trigger = (modifier >> SMBIOS_INTINFO_TRIGGER_BIT) & 1; */
/* 	} */
/*       else */
/* 	pinfo->ipmi_int_specified = 0; */
    }
/*   else */
/*     pinfo->ipmi_int_specified = 0; */

  if ((addr & 1) != 0)
    {
      pinfo->bmc_io_mapped = 0;
      pinfo->ipmi_iobase = strobed;
    }
  else
    {
      pinfo->bmc_io_mapped = 1;
      pinfo->ipmi_membase = strobed;
    }

  if (bufp[SMBIOS_DEV_INFO_LEN_OFFSET] > SMBIOS_DEV_INFO_INTNUM_OFFSET)
    pinfo->intr_num = bufp[SMBIOS_DEV_INFO_INTNUM_OFFSET];
  else
    pinfo->intr_num = 0;

  free (bufp);
  return pinfo;
}

#ifdef SMBIOS_MAIN
int
main (int argc, char** argv)
{
  smbios_dev_info_t idi;
  int status;

  if (smbios_get_dev_info (atoi(argv[1]), &idi, &status) == &idi)
    {
      printf ("%s", "Success!\n");
      printf ("interface type = %d\n", idi.type);
      printf ("version = %d.%d\n", idi.version.major, idi.version.minor);
      printf ("i2c slave address = %x\n", idi.i2c_slave_addr);
      printf ("nvstor address = %x\n", idi.nvstor_dev_addr);
      printf ("BMC io memory mapped = %s\n", (idi.bmc_io_mapped ? "true" : "false"));
      if (idi.bmc_io_mapped)
	printf ("BMC memory mapped registers start = %llx\n", idi.base.bmc_membase_addr);
      else
	printf ("BMC IO registers start = %llx\n", idi.base.bmc_iobase_addr);
      printf ("BMC register address spacing = %d\n", 1 << idi.reg_spacing);
      if (idi.intr.intinfo_specified)
	{
	  printf ("BMC interrupt polarity = %s\n", (idi.intr.intinfo_polarity_high ? "high" : "low"));
	  printf ("BMC interrupt trigger = %s\n", (idi.intr.intinfo_trigger_level ? "level" : "edge"));
	}
      printf ("BMC interrupt number = %x\n", idi.intr_num);
      exit (0);
    }
  else
    {
      printf ("%s", "Failure :-(\n");
      exit (1);
    }
}
#endif
