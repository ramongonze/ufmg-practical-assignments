import game
import socket, select, string, sys


def connect(host, port):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.settimeout(2)
	
	# connect to remote host
	try :
		sock.connect((host, port))
	except :
		print('Unable to connect')
		sys.exit()
	
	print('Connected to remote host')
	return sock

def main():
	if(len(sys.argv) < 3) :
		print('Usage : python client.py hostname port')
		exit()
	
	host = sys.argv[1]
	port = int(sys.argv[2])

	sock = connect(host, port)
	game.run()


if __name__ == "__main__":
	def main()