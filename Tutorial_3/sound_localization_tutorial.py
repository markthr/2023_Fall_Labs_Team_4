import numpy as np
import scipy.io as io
import matplotlib.pyplot as plt

files = ['M1.wav', 'M2.wav', 'M3.wav']
waveforms = [None]*3

for i, file in enumerate(files):
    waveforms[i] = io.wavfile.read(files[i])

def rms(arr):
    return np.sqrt(np.average(np.square(data, dtype='int64')))

# parts 1, 2
for i, (_, data) in enumerate(waveforms):
    print(f'RMS={rms(data)} for waveform[file={files[i]}]')

# part 3
for i, (_, data) in enumerate(waveforms):
    print(f'length={len(data)} for waveform[file={files[i]}]')

# only called once, created to organize code
def stride_pad_shift(arr):
    len_arr = len(arr)
    # pad each end of the array with zeros the length of the array
    a = np.pad(arr, (len_arr, len_arr))
    n = a.strides[0]
    # select 2*len, len length rows each shifted to get all the possible 0 padded shifts
    return np.lib.stride_tricks.as_strided(a[-len_arr:], (2*len_arr,len_arr), (-n,n))


sig_len = len(waveforms[0][1])
strides = stride_pad_shift(waveforms[0][1]/sig_len)
rf = np.matmul(strides, waveforms[1][1])
max_cor = np.argmax(rf)

shift = max_cor-sig_len

if(shift < 0):
    print(f'wav1 is delayed from wav2 by {-shift/waveforms[0][0]} seconds ({-shift} samples)')
else:
    print(f'wav2 is delayed from wav1 by {shift/waveforms[0][0]} seconds ({shift} samples)')

# negative delay means a delay of the first
plt.figure()
plt.plot(np.matmul(strides, waveforms[1][1]))
plt.show()

