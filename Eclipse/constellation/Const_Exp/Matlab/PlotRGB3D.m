function f = plotRgb3d(x,y,z, dmin, dmax)

if nargin < 5, dmax = max([x,y,z]); end;
if nargin < 4, dmin = min([x,y,z]); end;

numPoints = length(x);

r = (65472 - x - dmin) ./ (dmax - dmin);
g = (65472 - y - dmin) ./ (dmax - dmin);
b = (65472 - z - dmin) ./ (dmax - dmin);

clf;
for n =1:numPoints,
    plot3(r(n),g(n),b(n),'o','MarkerEdgeColor',[r(n),g(n),b(n)], 'MarkerFaceColor',[r(n),g(n),b(n)]);
    hold on;
end;
grid on; box on; hold off;