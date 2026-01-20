#!/usr/bin/env python3
"""
PlatformIO pre-build script to generate device macros from device_config.ini
This script runs before compilation and adds -D flags to build_flags
Currently generates SWITCH_N_* macros, but can be extended for other device types
"""

import configparser
import os
from pathlib import Path

Import("env")

def parse_device_config(ini_path):
    """Parse device_config.ini and return a list of (device_type, device_id, relay_pin, physical_pin) tuples
    Currently parses Switch_N sections, but can be extended for other device types
    """
    config = configparser.ConfigParser()
    
    if not os.path.exists(ini_path):
        print(f"Warning: {ini_path} not found. No device macros will be generated.")
        return []
    
    config.read(ini_path)
    devices = []
    
    # Parse Switch sections (can be extended for other device types like Regulator, etc.)
    for section_name in config.sections():
        if not section_name.startswith('Switch_'):
            continue
        
        try:
            device_id = int(section_name.replace('Switch_', ''))
            relay_pin = config.getint(section_name, 'relay_pin')
            physical_pin = config.getint(section_name, 'physical_pin')
            devices.append(('switch', device_id, relay_pin, physical_pin))
        except (ValueError, configparser.NoOptionError) as e:
            print(f"Warning: Skipping invalid section {section_name}: {e}")
            continue
    
    return sorted(devices, key=lambda x: x[1])  # Sort by device_id

def generate_macro_flags(devices):
    """Generate compiler flags (-D macros) from device configurations"""
    flags = []
    for device_type, device_id, relay_pin, physical_pin in devices:
        if device_type == 'switch':
            # Generate SWITCH_N_* macros
            flags.append(f"-DSWITCH_{device_id}_RELAY_PIN_INDEX={relay_pin}")
            flags.append(f"-DSWITCH_{device_id}_PHYSICAL_DEVICE_PIN_INDEX={physical_pin}")
        # Future: Add other device types here (e.g., regulator, etc.)
    
    return flags

# Get the project root directory (where device_config.ini is located)
project_dir = env.get("PROJECT_DIR")
ini_path = Path(project_dir) / 'device_config.ini'

devices = parse_device_config(ini_path)
flags = generate_macro_flags(devices)

# Add flags to build_flags
if flags:
    env.Append(BUILD_FLAGS=flags)
    print(f"Added {len(flags)} device macro definitions from device_config.ini")
else:
    print("No device macros generated (device_config.ini not found or empty)")
