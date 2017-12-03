from keras.utils.np_utils import to_categorical
from keras.callbacks import History 
from keras.optimizers import Adam
from keras.models import Sequential
from keras.layers import Dense
import matplotlib.pyplot as plt
import numpy as np

np.random.seed(7)

dataset = np.loadtxt("yeast_modified.csv", delimiter=";", dtype='str')
n = int((dataset.shape)[0] * 0.7) # 70% for training and 30% for test

X_train, X_test = dataset[:n, 0:8].astype(float), dataset[n:, 0:8].astype(float)
Y_train, Y_test = dataset[:n, 8], dataset[n:, 8]

del dataset

# Convert the class's name to the correspondent label
classes = ['CYT', 'MIT', 'ME1', 'ME2', 'ME3', 'EXC', 'NUC']
categorical_labels, classes_dict = to_categorical([0, 1, 2, 3, 4, 5, 6], num_classes=7), {}
for i in range(7):
	classes_dict[classes[i]] = categorical_labels[i]
Y_train = np.array([classes_dict[c] for c in Y_train])
Y_test = np.array([classes_dict[c] for c in Y_test])

# Create model
model = Sequential()
model.add(Dense(8, input_dim=8)) # Input layer
model.add(Dense(10, activation='sigmoid')) # Hidden layer
model.add(Dense(7, activation='softmax')) # Output layer

model.compile(loss='categorical_crossentropy', optimizer=Adam(lr=0.001, decay=0.001), metrics=['categorical_accuracy'])

# Fit the model
history = History()
model.fit(X_train, Y_train, epochs=300, callbacks=[history], validation_split = 0.2, shuffle=True, batch_size=10, verbose=2)

# Evaluate the model
scores = model.evaluate(X_test, Y_test, verbose=0)
print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))

x = history.history['categorical_accuracy']
y = list(range(300))

plt.plot(y, x)
plt.show()

# Calculate predictions
#predictions = model.predict(X_train)
#round predictions
#rounded = np.array([round(x[0]) for x in predictions])
#print(rounded)
