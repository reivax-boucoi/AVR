from biosppy import storage
from biosppy.signals import ecg

# load raw ECG signal
signal, mdata = storage.load_txt('./ecg_data.txt')

# process it and plot
out = ecg.ecg(signal=signal, sampling_rate=200., show=True, interactive=True)
