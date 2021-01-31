from subprocess import Popen, PIPE, STDOUT
exe = Popen("../../builddir/0xA2.exe", stdout=PIPE, stdin=PIPE, stderr=STDOUT)
while exe.poll() is None:
    exe.stdin.write(b"go nodes 2")
    out = exe.stdout.readline()
    print(out.decode("utf-8")[:-1])
exe.stdout.close()
exe.stdin.close()