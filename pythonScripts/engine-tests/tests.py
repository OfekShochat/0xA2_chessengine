from subprocess import Popen, PIPE, STDOUT
exe = Popen("../../builddir/0xA2.exe", stdout=PIPE, stdin=PIPE, stderr=STDOUT)
while exe.poll() is None:
    out = exe.stdout.readline()
exe.stdout.close()
exe.stdin.close()