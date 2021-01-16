from distributed import LocalCluster, Client
from distributed.security import Security
import tldextract
import os
from glob import glob
import platform
import hashlib
import requests
from urllib.parse import urlparse
import zipfile

def addExtension(name, distinguishe=False):
    adder = ["-linux", "-windows"][IS_WINDOWS] if distinguishe else ""
    return name + adder + ["", ".exe"][IS_WINDOWS]

def unzip(name):
    with zipfile.ZipFile("./{}".format(name), 'r') as zip_ref:
        os.mkdir("./{}".format(os.path.splitext(name)[0]))
        zip_ref.extractall("./{}".format(os.path.splitext(name)[0]))

def download(url):
    r = requests.get(url, allow_redirects=True)
    a = urlparse(url)
    open(os.path.basename(a.path), 'wb').write(r.content)

def download_and_unzip(url):
    download(url)
    a = urlparse(url)
    unzip(os.path.basename(a.path))

def comp(engine):
    compiler = None
    if os.system("cl") == 0: compiler = "cl"
    elif os.system("g++") == 0: compiler = "g++"

    os.system("{} ./engines/{}".format(compiler, engine))

def functionality(config):
    # config is a dict, and there will be:
    # : engines : which is a list of engine dicts
    # : engine dict : dict, there will be the url to download the source code, flags for compiling it, network file if exists (if not value='disabled')
    # :             : compile bool, if true compile the source code, if not, execute it as an executable (already built)

    global IS_WINDOWS
    IS_WINDOWS = platform.system() == "Windows"

    cutepath = addExtension("{}/cutechess/cutechess".format(os.getcwd()), True)

    if not os.path.isdir("cutechess"): unzip("cutechess.zip")
    if not os.path.isdir('engines' ): os.mkdir('engines')

    os.system(("""{} -engine name=lc0 cmd=C:\\Users\\User\\Downloads\\lc0-v0.26.3-windows-gpu-nvidia-cuda\\lc0.exe"""
    + """ -engine name=stockfish cmd=C:\\Users\\User\\Downloads\\stockfish_12_win_x64_bmi2\\stockfish_20090216_x64_bmi2.exe -each proto=uci tc={} """
    + """-rounds 1 -pgnout out.pgn min fi -bookmode disk -openings file=book.txt format=epd""").format(cutepath, config["tc"]))
    return open("out.pgn").read()

def serve():
    sec = Security(tls_ca_file='cluster_ca.pem',
                  tls_client_cert='cli_cert.pem',
                  tls_client_key='cli_key.pem',
            require_encryption=True)
    client = Client('127.0.0.1:8786') #, security=sec
    # {"engines":[{"name":"engine1", "compile":"true", "url":"https://github.com/OfekShochat/0xA2_chessengine"}, {"name":"engine2", "compile":"false", "url":"https://github.com/poop.exe"}]}
    for i in range(1):
        fi = client.submit(functionality, {"tc":"100/190+1"}, key=i)
        
    print(fi.result())
serve()