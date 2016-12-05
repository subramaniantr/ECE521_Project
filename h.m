
a = csvread('H.csv');
H = a(:,1);
l = numel(H);
y = 1:l;
plot(y,H);
hold on
grid on
set(gca,'LineWidth',2)
set(gca,'fontsize',20)
set(findall(gcf,'Type','line'),'LineWidth',2)

% a = csvread('out1');
% plot(a(:,1),a(:,3))
