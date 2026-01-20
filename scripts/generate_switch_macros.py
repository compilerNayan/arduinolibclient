#!/usr/bin/env python3
"""
Generate C++ preprocessor macros from switch_config.ini
Reads the INI file and outputs compiler flags (-D macros)
"""

import configparser
import sys
import os
from pathlib import Path

def parse_switch_config(ini_path):
    """Parse switch_config.ini and return a list of (switch_id, relay_pin, physical_pin) tuples"""
    config = configparser.ConfigParser()
    
    if not os.path.exists(ini_path):
        print(f"Warning: {ini_path} not found. No switch macros will be generated.", file=sys.stderr)
        return []
    
    config.read(ini_path)
    switches = []
    
    for section_name in config.sections():
        if not section_name.startswith('Switch_'):
            continue
        
        try:
            switch_id = int(section_name.replace('Switch_', ''))
            relay_pin = config.getint(section_name, 'relay_pin')
            physical_pin = config.getint(section_name, 'physical_pin')
            switches.append((switch_id, relay_pin, physical_pin))
        except (ValueError, configparser.NoOptionError) as e:
            print(f"Warning: Skipping invalid section {section_name}: {e}", file=sys.stderr)
            continue
    
    return sorted(switches, key=lambda x: x[0])

def generate_macro_flags(switches):
    """Generate compiler flags (-D macros) from switch configurations"""
    flags = []
    for switch_id, relay_pin, physical_pin in switches:
        flags.append(f"-DSWITCH_{switch_id}_RELAY_PIN_INDEX={relay_pin}")
        flags.append(f"-DSWITCH_{switch_id}_PHYSICAL_DEVICE_PIN_INDEX={physical_pin}")
    return flags

def main():
    # Get the project root directory (where switch_config.ini is located)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    ini_path = project_root / 'switch_config.ini'
    
    switches = parse_switch_config(ini_path)
    flags = generate_macro_flags(switches)
    
    # Output flags separated by spaces (for shell consumption)
    print(' '.join(flags))

if __name__ == '__main__':
    main()

