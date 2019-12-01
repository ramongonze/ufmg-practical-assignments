import socket
import threading
import game

class Match():
	def __init__(self):
		self.currPlayer = None # Keeps the number of the current player who must play.
		self.G = None 

	def startMatch(self):
		self.G = game.TikTakToe()

	def getStrBoard(self):
		"""
			Returns the board in a string. Example:
			Original table:
				|a|b|c|
				|d|e|f|
				|g|h|i|
			
			Return: "a,b,c,d,e,f,g,h"
		"""
		board = ''
		for i in range(3):
			for j in range(3):
				board = board + str(self.G.board[i][j]) + ','
		del board[-1]

		return board


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
		self.player1 = None
		self.player2 = None
		self.matchStarted = False

		self.sock.bind((ip, port))
		self.sock.listen(nClients)
		self.match = Match()

	def getMessage(self, connection, address, playerID):
		"""
			A handler used by a new thread created due to a new connection between server and client.

			@Parameters:
				connection: client's connection object
				address: client's address
				playerID: Player's number
		"""

		while True:
			# Decide which player the server will listen
			if self.match.currPlayer == playerID:
				data = connection.recv(self.dataSize)

				if not data:
					connection.close()
					break

				x,y = int(data.split(',')[0]), int(data.split(',')[1]) # Get the play
				self.match.G.board[x, y] = self.match.currPlayer # Fill the board with the play

				# Check if there is a winner or a draw happened
				result = self.match.winner()
				if result == 0:
					currBoard = self.getStrBoard()
					if self.match.currPlayer == 1:
						self.match.currPlayer = 2
						self.player1.send(bytes(currBoard + "|" + "It's the adversary's turn"))
						self.player2.send(bytes(currBoard + "|" + "It's your turn"))
					else:
						self.match.currPlayer = 1
						self.player1.send(bytes(currBoard + "|" + "It's your turn"))
						self.player2.send(bytes(currBoard + "|" + "It's the adversary's turn"))
					continue
				
				if result == 1:
					self.player1.send(bytes('|You win!'))
					self.player2.send(bytes('|You loose!'))
				elif result == 2:
					self.player1.send(bytes('|You loose!'))
					self.player2.send(bytes('|You win!'))
				else:
					self.player1.send(bytes('|Draw!'))
					self.player2.send(bytes('|Draw!'))

				self.match.destroy()

	def run(self):
		while True:
			connection, address = self.sock.accept()

			# After making a new connection, we create a new thread
			cThread = threading.Thread(target=self.getMessage, args=(connection, address, 1 if self.player1 == None else 2))
			cThread.daemon = True # Program becomes able to exit even if many thread are being executed
			cThread.start()
			
			# Add the new player's connection to the list of players
			if self.player1 == None:
				self.player1 = connection
			else:
				self.player2 = connection
				# Start a new match
				self.match.startMatch()

				# The first player starts playing
				self.match.currPlayer = 1
				initialBoard = getStrBoard()
				self.player1.send(bytes(initialBoard + "|" + "It's your turn"))
				self.player2.send(bytes(initialBoard + "|" + "It's the adversary's turn"))
		
		

def main():
	port = 65000
	nPlayers = 2
	dataSize = 1024

	# Create a new server
	S = Server(port, nPlayers, dataSize)
	S.run()

main()