clear all
close all

max_bpm=200;
min_bpm = 20;
Orderfilt = 3;    % filter order
window_duration = 10;
nfft = 2^14;

% Load CSV file
data = csvread('log_full.csv', 1, 0);  % skip header row
%data = csvread('log_reduced.csv', 1, 0);  % skip header row
data_StartIdx=1;
data_StopIdx=min(length(data(:, 1)),inf);
t = data(data_StartIdx:data_StopIdx, 1) - min(data(:, 1));   % time column
x = data(data_StartIdx:data_StopIdx, 2)-mean(data(:, 2));   % data/signal column
N = length(t);
t_uniform = linspace(min(t), max(t), N);
x_uniform = interp1(t, x, t_uniform, 'linear');

dt_actual = diff(t)*1e3;
dt_uniform = mean(dt_actual)*1e-3;%t_uniform(2) - t_uniform(1);
Fs = 1 / dt_uniform;

% --- Compute FFT ---
X = fft(x_uniform);
f = (0:N-1) * (Fs / N);
half_N = floor(N / 2);
f = f(1:half_N);
X_mag = abs(X(1:half_N)) * 2 / N;

% --- Filter ---
Fc_max = max_bpm/60;           % cutoff frequency in Hz
Fc_min = min_bpm/60;           % cutoff frequency in Hz
Wn = [Fc_min Fc_max] / (Fs/2); % normalized frequencies
[b, a] = butter(Orderfilt, Wn, 'bandpass');
[H, f_resp] = freqz(b, a, length(f), Fs);
H_dB = 20*log10(abs(H));
x_filtered = filtfilt(b, a, x_uniform);

% --- Filtered FFT ---
X_filt = fft(x_filtered);
X_filt_mag = abs(X_filt(1:half_N)) * 2 / N;

% --- Spectrogram ---
window = hamming(round(Fs*window_duration));  % 2-second window
noverlap = round(length(window)/2);
[S,F,T] = specgram(x_filtered, nfft, Fs, window, noverlap);
S_dB = 20*log10(abs(S));
[~, idx_Fmax] = max(S_dB, [], 1);   % find index of max power per column
BPM_t = F(idx_Fmax)*60;

figure;
set(gcf, 'Units', 'pixels', 'Position', [0, 0, 1280, 1024]);
% ===== Subplot 1: Time Domain =====
subplot(2,2,1);
ax1 = gca;
p1=plot(ax1,t, x, '-', 'LineWidth', 1.5);
hold on;
p4=plot(ax1,t_uniform, x_uniform, '-', 'LineWidth', 1.5);
hold on;
p2=plot(ax1,t_uniform, x_filtered, '-', 'LineWidth', 1.5);
xlim([min(t),max(t)])
xlabel('Time (s)');
ylabel(ax1,'Signal amplitude (a.u.)');
title('Time Domain Signal and Sampling Intervals');
grid on;
hold on;
ax2 = axes('Position', get(ax1, 'Position'),'YAxisLocation', 'right','Color', 'none','XColor', 'k');
hold(ax2, 'on');
p3=plot(ax2,t(2:end), dt_actual, '--', 'LineWidth', 1);

legend([p1, p4, p2, p3], {'Input signal', 'Resampled signal','Filtered signal', '\DeltaFc (ms)'});
ylabel(ax2,'\Deltat(ms)');
set(ax2, 'XLim', get(ax1, 'XLim'));
%linkaxes([ax1 ax2], 'x');


% ===== Subplot 2: Frequency Domain =====
subplot(2,2,4);
p1=semilogy(f, X_mag, 'LineWidth', 1.5);
hold on;
p2=semilogy(f, X_filt_mag, 'LineWidth', 1.5);
xlim([min(f), max(f)]);
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('Frequency Domain Analysis (Pre & Post filtering)');
grid on;
legend([p1, p2], {'Input signal FFT', 'Filtered signal FFT'});

% ===== Subplot 2: Filter =====
subplot(2,2,2);
idx = (f>0.5*Fc_min & f <= 2*Fc_max);
plot(f(idx), H_dB(idx), 'LineWidth', 1.5);
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
xlim([0.5*Fc_min 2*Fc_max]);
title('Filter Frequency Response');
grid on;

% --- Plot spectrogram ---
ax3 = subplot(2,2,3);
imagesc(T, F*60, S_dB);
axis xy;  % y-axis increasing upwards
xlabel('Time (s)');
ylabel('Frequency (BPM)');
title('Spectrogram of filtered signal');
hcb=colorbar("SouthOutside");
xlabel(hcb, 'Frequency band power (dB)');
caxis([max(S_dB(:))-30 max(S_dB(:))]);  % dynamic range 60 dB
ylim([min_bpm max_bpm/1.5]);
hold on;
p3=plot(T, BPM_t, 'r+-', 'MarkerSize', 6, 'LineWidth', 1.2);
linkaxes([ax1, ax2, ax3], 'x');
legend([p3], {'BPM (dominant F)'});
