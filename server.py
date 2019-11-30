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
		self.sock.bind((ip, port))
		self.sock.listen(nClients)

	def getMessage(self, connection, address):
		"""
			A handler used by a new thread created due to a new connection between server and client.

			@Parameters:
				connection: client's connection object
				address: client's address
				dataSize: Maximo number of bytes the server receives in each message
		"""

		while True:
			data = connection.recv(self.dataSize)
			print('[On server]: ' + str(data))
			for p in self.players:
				p.send(bytes(data))
			if not data:
				self.players.remove(connection)
				connection.close()
				break

	def run(self):
		while True:
			connection, address = self.sock.accept()
			# connection.send(bytes('olaaa'))
			# After making a new connection, we create a new thread
			cThread = threading.Thread(target=self.getMessage, args=(connection, address))
			cThread.daemon = True # Program becomes able to exit even if many thread are being executed
			cThread.start()
			self.players.append(connection) # Add the new player's connection to the list of players

			# if len(self.players) == 2:

			# 	if not matchStarted:
			# 		# Start a new match

			# 	else:


def main():
	port = 65000
	nPlayers = 2
	dataSize = 1024

	# Create a new server
	S = Server(port, nPlayers, dataSize)
	S.run()

main()