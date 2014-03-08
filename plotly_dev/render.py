from mako.template import Template
from mako.runtime import Context
import os


destination_parent = '..'
libs = ["wifi", "ethernet", "gsm", "cc3000"]
files = ['./plotly.pre.cpp', './plotly.pre.h']

for fn in files:
    for lib in libs:
        fout = ""
        # Write file to a destination folder
        directory = destination_parent+'/plotly_'+lib.lower()
        if not os.path.exists(directory):
            os.makedirs(directory)
        fout = open(directory+'/'+fn.replace('.pre', '_'+lib.lower()), 'w')

        mytemplate = Template(filename=fn)
        ctx = Context(fout, lib=lib)
        mytemplate.render_context(ctx)
        fout.close()