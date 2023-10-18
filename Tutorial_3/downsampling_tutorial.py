import numpy as np
import scipy.io as io
import matplotlib.pyplot as plt

# part 2 and 3
(fs, data) = io.wavfile.read('human_voice.wav')
f_downsample = 8000
ratio = fs/f_downsample
print(f"Downsamping signal from fs={fs} to f_downsample={f_downsample}, ratio=1/{ratio}")

plt.figure()
plt.subplot(1, 2, 1)
plt.plot(np.arange(0, len(data))/fs, data)

# parts 4, 5, 6
data_downsample = data[::int(ratio)]
plt.subplot(1, 2, 2)
plt.plot(np.arange(0, len(data_downsample))/f_downsample, data_downsample)
plt.show(block=False)

print(f"Downsample sgnal[samples={len(data_downsample)}] from original signal[samples={len(data)}]")

# part 7
t_start = 0.5
n_start = (int(fs*t_start)//6)*6
segment_len = 240
plt.figure()
plt.subplot(1, 2, 1)
t_segment = np.arange(n_start, n_start + segment_len)/fs+0.5
plt.plot(t_segment, data[n_start:(n_start+segment_len)])
n_start_ds = n_start//int(ratio)
segment_len_ds = segment_len//int(ratio)
plt.subplot(1, 2, 2)
t_segment_ds = np.arange(n_start_ds, n_start_ds + segment_len_ds)/f_downsample+0.5
plt.plot(t_segment_ds, data_downsample[n_start_ds:(n_start_ds+segment_len_ds)])
plt.show(block=True)