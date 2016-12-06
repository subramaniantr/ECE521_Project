
a = csvread('out.csv');
plot(a(:,1),a(:,3))
hold on
grid on
set(gca,'LineWidth',2)
set(gca,'fontsize',20)
set(findall(gcf,'Type','line'),'LineWidth',2)
a = csvread('out.csv');
plot(a(:,1),a(:,4))
hold on
grid on
set(gca,'LineWidth',2)
set(gca,'fontsize',20)
set(findall(gcf,'Type','line'),'LineWidth',2)
% a = csvread('out1');
% plot(a(:,1),a(:,3))
