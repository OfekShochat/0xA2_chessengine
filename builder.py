
"""
0xA2_chess-engine
Copyright (C) 2020-2021  Ofek Shochat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from os.path import relpath
from glob import glob
import os
from sys import argv

dirs = glob(".\\{}\\*".format(argv[argv.index("-project")+1]))
g_dirs = []  

def get_children(path):
    ds = []
    for i in glob(path + "\\*"):
        if i.count(".") < 2:
            dd = get_children(i)
            for d in dd:
                ds.append(d)
        else:
            ds.append(i)
        
    return ds
# get all file dirs (all tree)

for i in dirs:
    if i.count(".") < 2:
        dd = get_children(i)
        for i in dd:
            g_dirs.append(i)   
    else:
        g_dirs.append(i)
class f:
    def __init__(self, text, name):
        self.text = text
        self.once = False
        self.name = name
        self.deps = 0
        self.dds = []

# new plan: iterating over the includes. searching for the include in g_dirs (local include) and then doing `deps += 1` 
# and doing that for every thing in that file for every file. not efficiant but works
 
def get_includes(path, curr):
    try:
        for i in open(path, encoding="utf-8").readlines():
            if i[0:11].__contains__("#include \""):
                curr.deps += 1
                curr.dds.append(i[11:-1])
                for d in g_dirs:
                    if d.split('\\')[-1] == i[11:-1]:
                        get_includes(d, curr)
    except UnicodeDecodeError:
        for i in open(path, encoding="ISO-8859-1").readlines():
            if i[0:11].__contains__("#include \""):
                curr.deps += 1
                curr.dds.append(i[11:-1])
                for d in g_dirs:
                    if d.split('\\')[-1] == i[11:-1]:
                        get_includes(d, curr)

fs = []
for t in g_dirs:
    
    curr = f(hash(t), t)
    get_includes(t, curr)
    fs.append(curr)
ss = sorted(fs, key=lambda i: i.deps)
for d in range(len(ss)):
    
    try:
        if ss[d].deps == ss[d+1].deps:
            filename, file_extension = os.path.splitext(ss[d+1].name)
            if filename + ".h" in ss[d].dds:
                ss[d+1], ss[d] = ss[d], ss[d+1]
        if ss[d].name == argv[argv.index("-main")+1]:
            ss.insert(len(ss),ss[d])
            del ss[d]

    except IndexError:
        break


files = ""
for i in ss:
    filename, file_extension = os.path.splitext(i.name)
    if file_extension == ".cc" or file_extension == ".cpp":
        files += i.name + " "
print(files)
compilers = ["g++", "cl"]
comp = input("compiler to use " + str(compilers) + ": ")
if comp == "cl":
    options = {"pCompileHigh":"/MP8", "pCompileMid":"/MP:4", "pLoops":"/Qpar", "pGenMax":"/CGTHREADS:8", "pGenMid":"/CGTHREADS4", "debugerFast":"/DEBUG[FASTLINK]", "speed":"/O2"}
    active = ""
    for i in argv[2:]:
        try:
            active += options[i] + " "
        except KeyError: pass
    
os.system("{} /std:c++17 /Fo.\\build\\objs\\ /Fe.\\build\\main.exe  {}/I.\src {}".format(comp, active, files))