# thinkfc
Custom ThinkPad fan control software written in C.
-----------------------------------------

thinkfc is provided without any warranty.

This software directly controls system cooling and should only be used by users who understand the risks involved.

I am not responsible for any damage to hardware, software, data, or other losses resulting from the use of this software.

Use at your own risk.

thinkfc reads the temperatures from /proc/acpi/ibm/thermal, finds the maximum temperature of the system and regulates the fan by editing the /proc/acpi/ibm/fan file. 
Requires thinkpad_acpi kernel module to be loaded.

# Usage
- Install the deb package from releases or compile from source.
- The default settings have quite aggressive fan speed/temperature ratio.
- You can create a config file: /etc/thinkfc.conf, consisted of 8 integer temperature values, from lower to higher, one for each level of fan speed, from 1 to full-speed.
