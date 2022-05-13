Import("env", "projenv")

import socket
from time import sleep

address = ("localhost", 6969)

def before_upload(source, target, env):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    status = sock.connect_ex(address)
    if status == 0:
        sock.sendall(b"detach")
        sock.close()
        sleep(1)


def after_upload(source, target, env):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    status = sock.connect_ex(address)
    if status == 0:
        sock.sendall(b"attach")
        sock.close()
        sleep(1)

env.AddPreAction("upload", before_upload)
env.AddPostAction("upload", after_upload)