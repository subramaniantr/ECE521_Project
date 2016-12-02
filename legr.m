o=5        %order
k=1+o      %number of parameters
t=1:k+1    %current + k previous points

L=ones(k,1)%coefficients
for i=1:k  %go through all o+1 excluding current
for j=1:k  %go through all o+1 points
if (i~=j)  % BUT points where i and j don't cancel
L(i)=L(i).*(t(k+1)-t(k+1-j))./(t(k+1-i)-t(k+1-j));  %(curr - j)/(i-j)
t(k+1)
t(k+1-j)
t(k+1-i)
end
end
end
L