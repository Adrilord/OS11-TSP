clear;

fin = './input/input_test.txt';
fout = './output/output_PPI_test.txt';
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

n = sscanf(tlines{1}, '%d');
max = sscanf(tlines{2}, '%f %f');
p = zeros(n, 2);
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

starts = zeros(n, 2);
for i=1:n
    l = sscanf(olines{i}, '%f %f');
    starts(i,1)=l(1);
    starts(i,2)=l(2);
end

% plot
hold on;
axis([0 max(1) 0 max(2)]);
scatter(p(:,1),p(:,2),30,'b','filled');
for i=1:n-1
    plot([starts(i,1) starts(i+1,1)],[starts(i,2) starts(i+1,2)],'r');
end
plot([starts(n,1) starts(1,1)],[starts(n,2) starts(1,2)],'r');
hold off;
