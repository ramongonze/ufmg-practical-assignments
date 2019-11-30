import socket
import threading
import game

class Server:
	def __init__(self, port, nClients, dataSize, ip='0.0.0.0'):
		"""
			
			@Parameters:
				port: Port used to listen.
				nClients: Maximum number of simultaneous connections.
				ip: address to listen. It has to be '0.0.0.0' if the server is listening all addresses.
				dataSize: Maximum number of bytes the server receives in each message.
		"""

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # SOCK_STREAM because it is a TCP connection
		self.ip = ip
		self.port = port
		self.dataSize = dataSize
		self.players = [] # List to keep players' connection
		self.matchStarted = False
		sock.bind((ip, port))
		sock.listen(nClients)

	def getMessage(connection, address):
		"""
			A handler used by a new thread created due to a new connection between server and client.

			@Parameters:
				connection: client's connection object
				address: client's address
				dataSize: Maximo number of bytes the server receives in each message
		"""

		while True:
			data = connection.recv(self.dataSize)
			
			if not data:
				self.players.remove(connection)
				connection.close()
				break

	while True:
		connection, address = sock.accept()
		
		# After making a new connection, we create a new thread
		cThread = threading.Thread(target=getMessage, args=(connection, address))
		cThread.daemon = True # Program becomes able to exit even if many thread are being executed
		cThread.start()
		self.players.append(connection) # Add the new player's connection to the list of players

		if len(self.players) == 2:

			if not matchStarted:
				# Start a new match

			else:
				

def main()
	port = 77777
	nPlayers = 2
	dataSize = 1024

	# Create a new server
	Server S(port, nPlayers, dataSize)

main()