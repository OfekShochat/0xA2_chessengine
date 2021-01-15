from distributed import LocalCluster, Client
import os
from glob import glob

def functionality(config):
    # config is a dict, and there will be:
    # : engines : which is a list of engine dicts
    # : engine dict : dict, there will be the url to download the source code, flags for compiling it, network file if exists (if not value='disabled')
    # :             : compile bool, if true compile the source code, if not, execute it as an executable (already built)
    if not ".\\cutechess" in glob(".\\**"):
        import zipfile
        with zipfile.ZipFile(".\\cutechess.zip", 'r') as zip_ref:
            os.mkdir(".\\cutechess")
            zip_ref.extractall(".\\cutechess")
    if not os.path.isdir('engines' ): os.mkdir('engines')

    os.system((""".\\cutechess\\cutechess-cli.exe -engine name=lc0 cmd=C:\\Users\\User\\Downloads\\lc0-v0.26.3-windows-gpu-nvidia-cuda\\lc0.exe"""
    + """ -engine name=stockfish cmd=C:\\Users\\User\\Downloads\\stockfish_12_win_x64_bmi2\\stockfish_20090216_x64_bmi2.exe -each proto=uci tc={} """
    + """-rounds 1 -pgnout out.pgn min fi -bookmode disk -openings file=book.txt format=epd""").format(config["tc"]))
    return open("out.pgn").read()

def serve():
    client = Client('127.0.0.1:8786')

    futures = []
    for i in range(1):
        fi = client.submit(functionality, {"tc":"100/190+1"}, key=i)
serve()