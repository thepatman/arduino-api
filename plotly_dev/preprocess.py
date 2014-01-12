'''
    A simple, hacky preprocessor 
    that filters out the #ifdef CLIENT_TYPE #endif statements
    for each client type, and writes the result to a folder in the parent folders
'''
import re
import os
client_defines = ['ETHERNET', 'WIFI', 'CC3000', 'GSM']
files = ['plotly.pre.cpp', 'plotly.pre.h']
destination_parent = '..'

for fn in files:
    f = open(fn)
    s = f.read()
    f.close()
    for client in client_defines:
        # Remove the ifdefs around the sections that we wanna keep
        s1 = re.sub(re.compile('#ifdef '+client+'\\n(?P<code>.*?)#endif\\n', re.DOTALL), '\g<code>', s)
        # Remove the rest of the ifdef sections
        s2 = re.sub(re.compile('#ifdef.*?#endif\\n', re.DOTALL), '', s1)
        # Write file to a destination folder
        directory = destination_parent+'/plotly_'+client.lower()
        if not os.path.exists(directory):
            os.makedirs(directory)
        fout = open(directory+'/'+fn.replace('.pre', '_'+client.lower()), 'w')
        fout.write(s2)
        fout.close()
