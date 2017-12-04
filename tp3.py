from keras.utils.np_utils import to_categorical
from keras.optimizers import Adam
from keras.models import Sequential
from keras.layers import Dense
import matplotlib.pyplot as plt
import numpy as np
import sys

np.random.seed(7)

dataset = np.loadtxt(sys.argv[1], delimiter=";", dtype='str')
n = int((dataset.shape)[0] * 0.7) # 70% for training and 30% for test

X_train, X_test = dataset[:n, 0:8].astype(float), dataset[n:, 0:8].astype(float)
Y_train, Y_test = dataset[:n, 8], dataset[n:, 8]

# Select all classes in the dataset and convert the classes's name to the correspondent label
classes = np.unique(dataset[:,-1])
categorical_labels, classes_dict = to_categorical(list(range(classes.shape[0])), num_classes=classes.shape[0]), {}
for i in range(classes.shape[0]):
	classes_dict[classes[i]] = categorical_labels[i]
Y_train = np.array([classes_dict[c] for c in Y_train])
Y_test = np.array([classes_dict[c] for c in Y_test])

# Create model
model = Sequential()
model.add(Dense(10, input_dim=8)) # Input layer
model.add(Dense(10, activation='sigmoid')) # Hidden layer
model.add(Dense(7, activation='softmax')) # Output layer

# Compile model
model.compile(loss='categorical_crossentropy', optimizer=Adam(lr=0.01, decay=0.001), metrics=['categorical_accuracy'])

# Fit the model
model.fit(X_train, Y_train, epochs=200, validation_split = 0.2, shuffle=True, batch_size=10, verbose=2)

# Evaluate the model
scores = model.evaluate(X_test, Y_test, verbose=0)
print("\n%s: %.2f%%" % ("Accuracy using test set", scores[1]*100))