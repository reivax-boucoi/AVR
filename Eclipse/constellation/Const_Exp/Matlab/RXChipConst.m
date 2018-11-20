% The Signal Path (http://www.TheSignalPath.com)
% Shahriar Shahramian, June 2012
% 3D Constellation Experiment

%% Initialize & Open Port

clear all; clc;
MyCOM = serial('COM4', 'Baudrate', 57600);
fopen(MyCOM);
MyCOM.Timeout = inf;
fprintf (MyCOM, '%c', 's'); fscanf (MyCOM, '\n\r');

%% Plot Constellation

i = 1;
fprintf (MyCOM, '%c', 'r'); fscanf (MyCOM, '\n\r');
while (1)
    TempData = fscanf (MyCOM, '%s\n\r');
    ReadR(i) = hex2dec (TempData (2:5));
    ReadG(i) = hex2dec (TempData (7:10));
    ReadB(i) = hex2dec (TempData (12:15));
    Data (i) = hex2dec (TempData (17:18));
    fprintf ('%c', Data (i));
    clf; PlotRGB3D (ReadR, ReadG, ReadB, 0, 65472); drawnow;
    i = i + 1;
end

%% View Thresholds

clc;
Desc = [3840 11776 19712 27584 35424 43360 51264 59040 65472];
plot (ones (1,i), ReadR, 'ro'); hold on;
plot (2 * ones (1,i), ReadG, 'go'); hold on;
plot (3 * ones (1,i), ReadB, 'bo'); hold on;
plot (ones (1,1), Desc, 'ko'); hold on;
plot (2 * ones (1,1), Desc, 'ko'); hold on;
plot (3 * ones (1,1), Desc, 'ko'); grid on;
DescH = dec2hex (Desc,4);

%% Close Port

fclose(MyCOM);
clear all; clc;