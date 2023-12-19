import numpy as np
import scipy.io.wavfile as wav
import pyaudio
from scipy.signal import butter, lfilter, periodogram
import matplotlib.pyplot as plt



def butter_lowpass_filter(data, cutoff, fs, order):
    b, a = butter(order, cutoff, fs=fs, btype='low',analog=False)
    y = lfilter(b, a, data)
    return y



CHUNK = 128
order = 6
cutoff = 500
RATE = 44100



#RATE, x = wav.read("./Speech_Recognition/noise_far.wav")

p               =   pyaudio.PyAudio()

player = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, output=True, 
frames_per_buffer=CHUNK)
stream = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK)

time_detects = []

while(True):
    f, Pxx_den = periodogram(np.fromstring(stream.read(CHUNK),dtype=np.int16),RATE)
    if (Pxx_den[1] > 100):
        print('you are talking')
    else:
        print('speak up lil boi')
    #y = butter_lowpass_filter(x[i*CHUNK:(i+1)*CHUNK-1,0], cutoff, RATE, 6)
    '''f, Pxx_den = periodogram(x[i*CHUNK:(i+1)*CHUNK-1,0],RATE)
    if (Pxx_den[1] > 100):
        time_detects.append(i*CHUNK/RATE)
    if (i == int(RATE/CHUNK*5)):
        f, Pxx_den = periodogram(x[i*CHUNK:(i+1)*CHUNK-1,0],RATE)
        #print(Pxx_den)
        plt.semilogy(f, Pxx_den)
        plt.ylim([1e-7, 1e4])
        plt.xlabel('frequency [Hz]')
        plt.ylabel('PSD [V**2/Hz]')
        #plt.show()'''

    #player.write(np.ascontiguousarray(y,dtype=np.int16),CHUNK) 

stream.stop_stream()
stream.close()
p.terminate()
#print(time_detects)

