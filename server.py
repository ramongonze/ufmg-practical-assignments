import socket
import threading
import game
import numpy as np

class Match():
	def __init__(self):
		self.currPlayer = None # Keeps the number of the current player who must play.
		self.G = None 

	def startMatch(self):
		self.G = game.TikTakToe()


	def winner(self):
		"""
		Check if there is a draw, winner, or none of them.

		Return:
			0 -- No win or draw
			1 -- If player 1 has won
			2 -- If player 2 has won
			3 -- If there is a draw
		"""

		for player in [1,2]:
			won = np.full((self.G.board_size), player)

			# Check diagonals
			if(np.array_equal(np.diag(self.G.board), won)): return player
			if(np.array_equal(np.diag(np.fliplr(self.G.board)), won)): return player

			# Check lines and columns
			for i in range(self.G.board_size):
				if(np.array_equal(self.G.board[i], won)): return player
				if(np.array_equal(self.G.board[:,i], won)): return player

		# Draw
		if(not(0 in self.G.board)): return 3

		# No win or draw
		return 0	

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
		print(playerID)
		print('aaaaaaaaaaaaaaaaa')
		print('self.match.currPlayer'+str(self.match.currPlayer))
		while True:
			# Decide which player the server will listen
			if self.match.currPlayer == playerID:
				data = connection.recv(self.dataSize).decode('utf-8')
				if not data:
					connection.close()
					break
				print('player '+str(self.match.currPlayer))
				x,y = int(data.split(',')[0]), int(data.split(',')[1]) # Get the play
				self.match.G.board[x, y] = self.match.currPlayer # Fill the board with the play
				print(x,y)
				# Check if there is a winner or a draw happened
				result = self.match.winner()
				if result == 0:
					#currBoard = self.match.getStrBoard()
					if self.match.currPlayer == 1:
						self.match.currPlayer = 2
						#(1,2,3,4) 1 player 2,3 coord  4 is my turn?
						self.player1.send(bytes(str(playerID)+','+data+','+str(0), encoding='utf-8'))
						self.player2.send(bytes(str(playerID)+','+data+','+str(1), encoding='utf-8'))
					else:
						self.match.currPlayer = 1
						self.player1.send(bytes(str(playerID)+','+data+','+str(1), encoding='utf-8'))
						self.player2.send(bytes(str(playerID)+','+data+','+str(0), encoding='utf-8'))
					continue
				
				if result == 1:
					self.player1.send(bytes("7,0,0,1", encoding='utf-8'))
					self.player2.send(bytes("6,0,0,1", encoding='utf-8'))
				elif result == 2:
					self.player1.send(bytes("6,0,0,2", encoding='utf-8'))
					self.player2.send(bytes("7,0,0,2", encoding='utf-8'))
				else:
					self.player1.send(bytes("8,0,0,0", encoding='utf-8'))
					self.player2.send(bytes("8,0,0,0", encoding='utf-8'))

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

				self.player1.send(bytes("4,0,0,0", encoding='utf-8'))
				self.player2.send(bytes("5,0,0,0", encoding='utf-8'))
				#self.player1.send(bytes(initialBoard + "|" + "It's your turn", encoding='utf-8'))
				#self.player2.send(bytes(initialBoard + "|" + "It's the adversary's turn", encoding='utf-8'))
		
		

def main():
	port = 65002
	nPlayers = 2
	dataSize = 2048

	# Create a new server
	S = Server(port, nPlayers, dataSize)
	S.run()

main()