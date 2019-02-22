"""
FogLAMP filtering fro readings data
"""

__author__ = "Massimiliano Pinto"
__copyright__ = "Copyright (c) 2018 Dianomic Systems"
__license__ = "Apache 2.0"
__version__ = "${VERSION}"

import sys
import json

"""
Filter configuration set by set_filter_config(config)
"""

"""
filter_config, global variable
"""
filter_config = dict()

"""
Set the Filter configuration into filter_config (global variable)

Input data is a dict with 'config' key and JSON string version wit data

JSON string is loaded into a dict, set to global variable filter_config

Return True
"""
def set_filter_config(configuration):
    print configuration
    global filter_config
    filter_config = json.loads(configuration['config'])

    return True

"""
Method for filtering readings data

Input is array of dicts
[
    {'reading': {'power_set1': '5980'}, 'asset_code': 'lab1'},
    {'reading': {'power_set1': '211'}, 'asset_code': 'lab1'}
]

Input data:
   readings: can be modified, dropped etc
Output is array of dict
"""
def readings27(readings):
    # Get list of asset code to filter
    if ('asset_code' in filter_config):
        asset_codes = filter_config['asset_code']
    else:
        asset_codes = [ ]

    for elem in readings:
        if (elem['asset_code'] and elem['asset_code'] in asset_codes):
            reading = elem['reading']
            # Apply some changes: add 5000 to all datapoints value
            for key in reading:
                newVal = reading[key] + 5000
                reading[key] = newVal


                # Apply max_value check
                if ('max_value' in filter_config):
                    maximum_value = int(filter_config['fixed'])
                    if (reading[key] > maximum_value):
                        reading[key] = maximum_value

    return readings

