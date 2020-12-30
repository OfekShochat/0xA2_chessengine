@echo off
if %1%==-r (
    python D:\0xA2\builder.py -main test.cc speed pLoops pCompileMax pGenMax -project %2%
)
if %1%==-d (
    python D:\0xA2\builder.py -main test.cc pCompileMax pGenMax debuggerFast -project %2%
)