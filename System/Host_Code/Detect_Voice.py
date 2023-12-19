import numpy as np
import pyaudio
from scipy.signal import periodogram



def detect_voice():

    global global_detect
    global_detect = 0
    CHUNK = 128
    RATE = 44100
    detect = False
    sum = 0

    p               =   pyaudio.PyAudio()

    player = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, output=True, 
    frames_per_buffer=CHUNK)
    stream = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK)


    while(detect == False):
        # Finds power spectral density of audio chunk
        f, Pxx_den = periodogram(np.fromstring(stream.read(CHUNK),dtype=np.int16),RATE)
        # Thresholds 2nd freq bin for detection
        if (Pxx_den[1] > 100):
            sum += 1
        else:
            if (sum > 0):
                sum -= 1
        
        if (sum > 20):
            print('detected')
            detect = True
        
    stream.stop_stream()
    stream.close()
    p.terminate()

    global_detect = 1
    return detect


