import socket
import threading
import game
import numpy as np

def createThread(func, arguments):
	cThread = threading.Thread(target=func, args=arguments)
	cThread.daemon = True # Program becomes able to exit even if many thread are being executed
	cThread.start()

class Match():
	def __init__(self):
		self.currPlayer = None # Keeps the number of the current player who must play.
		self.G = None # TicTacToe object

	def startMatch(self):
		self.G = game.TicTacToe()

	def winner(self):
		"""
		Check if there is a draw, winner, or none of them.

		Return:
			0 -- No win or draw
			1 -- If player 1 has won
			2 -- If player 2 has won
			3 -- If there was a draw
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
		self.resetMatch = 0 # It keeps the number of players who answered 'game.PLAY_AGAIN'
		self.matchNumber = 1 # Used to swap which player starts playing

		self.sock.bind((ip, port))
		self.sock.listen(nClients)
		self.match = Match()

	def manageMatch(self):
		while True:
			if self.resetMatch == 2:
				del self.match
				self.match = Match() # New match

				# Start a new match
				self.match.startMatch()

				# Decides which player is going to play at first
				if self.matchNumber%2 == 1:
					self.match.currPlayer = 2
					self.player2.send(bytes('%d00'%(game.YOU_ARE_P1), encoding='utf-8'))
					self.player1.send(bytes('%d00'%(game.YOU_ARE_P2), encoding='utf-8'))
				else:
					self.match.currPlayer = 1
					self.player1.send(bytes('%d00'%(game.YOU_ARE_P1), encoding='utf-8'))
					self.player2.send(bytes('%d00'%(game.YOU_ARE_P2), encoding='utf-8'))
				
				self.matchNumber += 1
				self.resetMatch = 0

				aux = self.player1
				self.player1 = self.player2
				self.player2 = self.player1

	def sendResult(self, data, result, playerID):
		if result == 0:
			if self.match.currPlayer == 1:
				self.match.currPlayer = 2
				#(1,2,3) 1 message, 2,3 coord
				self.player1.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
				self.player2.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
			else:
				self.match.currPlayer = 1
				self.player1.send(bytes('%d%s%d'%(playerID, data, 1), encoding='utf-8'))
				self.player2.send(bytes('%d%s%d'%(playerID, data, 0), encoding='utf-8'))
			
			return None
		
		if result == 1:
			self.player1.send(bytes("%d00"%(game.P1_WON), encoding='utf-8'))
			self.player2.send(bytes("%d00"%(game.P1_WON), encoding='utf-8'))
		elif result == 2:
			self.player1.send(bytes("%d00"%(game.P2_WON), encoding='utf-8'))
			self.player2.send(bytes("%d00"%(game.P2_WON), encoding='utf-8'))
		else:
			self.player1.send(bytes("%d00"%(game.DRAW), encoding='utf-8'))
			self.player2.send(bytes("%d00"%(game.DRAW), encoding='utf-8'))

	def manageClient(self, connection, playerID):
		"""
			A handler used by a new thread created due to a new connection between server and client.

			@Parameters:
				connection: client's connection object
				playerID: Player's number
		"""
		
		while True:
			if playerID == 1:
				connection = self.player1
			else:
				connection = self.player2

			# Decide which player the server will listen to
			data = connection.recv(self.dataSize).decode('utf-8')
			
			print('Server received from Player[%d]: |'%(playerID) + str(data) + '|')
			
			if len(data) > 1 and int(data[0]) == game.PLAY_AGAIN:
				self.resetMatch += 1 # Increase the number of players who accepted playing again

				if(playerID == 1):
					self.player2.send(bytes('%d00'%(game.PLAY_AGAIN), encoding='utf-8'))
				else:
					self.player1.send(bytes('%d00'%(game.PLAY_AGAIN), encoding='utf-8'))
			
			elif len(data) > 1 and int(data[0]) == game.EXIT_GAME:
				if(playerID == 1):
					self.player2.send(bytes('%d00'%(game.EXIT_GAME), encoding='utf-8'))
				else:
					self.player1.send(bytes('%d00'%(game.EXIT_GAME), encoding='utf-8'))

				connection.close()

			elif self.match.currPlayer == playerID:
				if not data:
					connection.close()
					break

				x,y = int(data[0]), int(data[1]) # Get the play
				self.match.G.board[x][y] = self.match.currPlayer # Fill the board with the play
				
				# Check if there is a winner or a draw
				result = self.match.winner()
				self.sendResult(data, result, playerID)

	def run(self):
		while True:
			connection, _ = self.sock.accept()

			# After making a new connection, we create a new thread
			
			
			# Add the new player's connection to the list of players
			if self.player1 == None:
				self.player1 = connection
				createThread(self.manageClient, (connection, 1))
				print('Player 1 connected.')
			else:
				print('Player 2 connected.')
				self.player2 = connection
				createThread(self.manageClient, (connection, 2))

				# Start a new match
				self.match.startMatch()
				print('Match started!')

				# The first player starts playing
				self.match.currPlayer = 1
				self.resetMatch = 0
				
				cThread = threading.Thread(target=self.manageMatch)
				cThread.daemon = True # Program becomes able to exit even if many thread are being executed
				cThread.start()

				self.player1.send(bytes('%d00'%(game.YOU_ARE_P1), encoding='utf-8'))
				self.player2.send(bytes('%d00'%(game.YOU_ARE_P2), encoding='utf-8'))		

def main():
	port = 65001
	nPlayers = 2
	dataSize = 2048

	# Create a new server
	S = Server(port, nPlayers, dataSize)
	S.run()

main()