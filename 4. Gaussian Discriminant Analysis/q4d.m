
function [phi, mu0, mu1, sigma0, sigma1] = GDA()
    x = load('q4x.dat'); y = readYFromFile();
    m = length(y);
    phi = sum(y)/m; n = 2;
    mu0 = zeros(1,n); mu1 = zeros(1,n);
    for i = 1:m
        if y(i) == 0
            mu0 = mu0 + x(i,:);
        else
            mu1 = mu1 + x(i,:);
        end
    end
    c = sum(y); % no of 1's
    mu0 = (1/(m-c)) * mu0';
    mu1 = (1/c) * mu1';
    sigma0 = zeros(n,n);
    sigma1 = zeros(n,n);
    for i=1:m
        curr = x(i,:)'; % i'th data point as a column vector
        if y(i)==0
            sigma0 = sigma0 + (curr - mu0) * (curr - mu0)';
        else
            sigma1 = sigma1 + (curr - mu1) * (curr - mu1)';
        end
    end
    sigma0 = (1/(m-c))*sigma0;
    sigma1 = (1/c)*sigma1;
end

function y = readYFromFile()
    z = textread('q4y.dat','%s');
    for i = 1:length(z)
        y(i,1) = strcmp(z(i,1), 'Canada');
    end
end
