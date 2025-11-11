from biosppy import storage
from biosppy.signals import ecg
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import lombscargle

df = pd.read_csv('./serial_log.csv')
# load raw ECG signal

signal = list(df['Data'][4000:32200])

df['Seconds'] = pd.to_timedelta(df['Timestamp']).dt.total_seconds()
plt.ion()
plt.plot(df['Seconds'],df['Data'])

df_filt=df.iloc[4000:32200]
df_filt.to_csv('log_reduced.csv', columns=['Seconds', 'Data'], index=False)

# process it and plot
signal, mdata = storage.load_txt('./ecg_data.txt')
out = ecg.ecg(signal=signal, sampling_rate=182, show=True, interactive=True)

# Define frequency range to analyze
freqs = np.linspace(0.1, 10, 1000)  # Hz
oct
# Compute Lomb–Scargle power
pgram = lombscargle(df['Seconds'], df['Data'], 2 * np.pi * freqs)

# Plot
plt.figure(figsize=(8,4))
plt.plot(freqs, np.sqrt(4 * pgram / len(df)), color='blue')
plt.title("Lomb–Scargle Spectrum (Uneven Sampling)")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude")
plt.grid(True)
plt.show()
