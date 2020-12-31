@echo off
if %1%==-r (
    python builder.py -main test.cc speed pLoops pCompileMax pGenMax -project %2%
)
if %1%==-d (
    python builder.py -main test.cc pCompileMax pGenMax debuggerFast -project %2%
)