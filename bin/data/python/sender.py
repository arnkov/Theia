from pythonosc.udp_client import SimpleUDPClient

ip = "127.0.0.1"
port = 8000

client = SimpleUDPClient(ip, port)
csm = client.send_message
csm("/bLua", [True])
csm("/bReloadLua", [True])
