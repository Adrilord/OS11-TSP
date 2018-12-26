clear;

fin = './input/input_test.txt';
fout = './output/output_PPI_test.txt';
%'./output/output_PPI_test.txt'
%'./output/output_PLI_test.txt'
%'./output/output_MI_test.txt'

% read input
f=fopen(fin);
tline = fgetl(f);
tlines = cell(0,1);
while ischar(tline)
    tlines{end+1,1} = tline;
    tline = fgetl(f);
end
fclose(f);

n = sscanf(tlines{1}, '%d'); %number of points
max = sscanf(tlines{2}, '%f %f'); %maximum X and Y coordinates
p = zeros(n, 2); %points
for i=1:n
    p(i,:) = sscanf(tlines{i+2}, '%f %f');
end;

% read output
f=fopen(fout);
oline = fgetl(f);
olines = cell(0,1);
while ischar(oline)
    olines{end+1,1} = oline;
    oline = fgetl(f);
end
fclose(f);

starts = zeros(n, 2); %circuit connections
number = zeros(n, 1); %number of when the node was added
for i=1:n
    l = sscanf(olines{i}, '%f %f %d');
    starts(i,1)=l(1);
    starts(i,2)=l(2);
    number(i)=l(3);
end
total_length = sscanf(olines{n+1}, '%f');

% plot
figure; ##close all;
hold on;
axis([0 max(1) 0 max(2)]);
scatter(p(:,1),p(:,2),30,'b','filled');
title(["Total length : " , num2str(total_length)]);
for i=1:n-1
    text(starts(i,1)+0.1,starts(i,2)+0.1, num2str(number(i)));
    plot([starts(i,1) starts(i+1,1)],[starts(i,2) starts(i+1,2)],'r');
end
text(starts(n,1)+0.1,starts(n,2)+0.1, num2str(number(n)));
plot([starts(n,1) starts(1,1)],[starts(n,2) starts(1,2)],'r');

hold off;
