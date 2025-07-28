# Copyright (C) 2024 Gilles Degottex <gilles.degottex@gmail.com> All Rights Reserved.
#
# You may use, distribute and modify this code under the
# terms of the Apache 2.0 license.
# If you don't have a copy of this license, please visit:
#     https://github.com/gillesdegottex/cpptopdb

import numpy as np
import json

global nb_ctrlc_pressed
nb_ctrlc_pressed = 0

def ctrlc_pressed():
    return nb_ctrlc_pressed

# TODO Doesnt work
def cont():
    global nb_ctrlc_pressed
    nb_ctrlc_pressed = 0
    # Then 'continue' in the debugger


def load(varname, dtype=np.float32):
    globals()[varname] = np.fromfile(f"./cpptopdb/{varname}.bin", dtype=dtype)

def pull():
    print("cpptopdb: Loading data from C++ side...")
    with open(f"./cpptopdb/cpptopdb.json") as f:
        data = json.load(f)
    for var in data["variables"]:
        # TODO Load only if it has been changed
        dtype = np.float32
        if "dtype" in var:
            dtype = np.dtype(var["dtype"])
        load(var["varname"], dtype=dtype)

def list():
    print("cpptopdb: Current variables:")
    pull()
    with open(f"./cpptopdb/cpptopdb.json") as f:
        data = json.load(f)
    for var in data["variables"]:
        varname = var["varname"]
        print(f'cpptopdb:     {varname}: {globals()[varname].shape} [{var["dtype"]}]')
