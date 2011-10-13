#!/usr/bin/python

# so we don't have to install kvpy.so (or we could 
# have set PYTHONPATH to include its dir beforehand)
import sys
sys.path.append("../build/lib.linux-i686-2.6")

import kvpy
d = {"key":"value","key2":"value2"}
kvpy.kvpy_write("spool","wr",d)
