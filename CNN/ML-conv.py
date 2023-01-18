import time
import numpy as np
from matplotlib import pyplot as plt
from keras.utils import np_utils
import keras.callbacks as cb
from keras import layers
from keras import models
from keras.layers.core import Dense, Dropout, Activation
from keras import optimizers 
from keras.datasets import mnist
from keras.preprocessing.image import ImageDataGenerator
import sys
import ROOT as r

class LossHistory(cb.Callback):
    def on_train_begin(self, logs={}):
		self.losses = []
    def on_batch_end(self, batch, logs={}):
        batch_loss = logs.get('loss')
        self.losses.append(batch_loss)




##################################################################
##################################################################
########################### Main #################################
##################################################################
##################################################################


train_datagen = ImageDataGenerator(rescale=1./255)
test_datagen = ImageDataGenerator(rescale=1./255)

train_dir = '/home/pablom/Documentos/work/MTP/Analysis/TA-SimpleML-003/pictures/train/'
test_dir = '/home/pablom/Documentos/work/MTP/Analysis/TA-SimpleML-003/pictures/test/'

train_generator = train_datagen.flow_from_directory(train_dir, target_size=(596, 572), batch_size=2, class_mode='categorical')
test_generator = test_datagen.flow_from_directory(test_dir, target_size=(596, 572), batch_size=2, class_mode='categorical')

start_time = time.time()

#Model
model = models.Sequential()
model.add(layers.Conv2D(32, (3, 3), activation='relu', input_shape=(596, 572, 3)))
model.add(Dropout(0.4))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(64, (3, 3), activation='relu'))
model.add(Dropout(0.4))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(128, (3, 3), activation='relu'))
model.add(Dropout(0.4))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(128, (3, 3), activation='relu'))
model.add(Dropout(0.4))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Flatten())
model.add(layers.Dense(512, activation='relu'))
model.add(layers.Dense(9, activation='softmax'))

model.compile(loss='categorical_crossentropy', optimizer=optimizers.RMSprop(lr=1e-4), metrics=['acc'])

history = LossHistory()
print 'Training model...'
story = model.fit_generator(train_generator, steps_per_epoch=72, epochs = 5, validation_data=test_generator, validation_steps = 27)


model.save('weights.h5')




