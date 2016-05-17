function plotPointsAndLine(mu0, mu1, sigma)
    x = load('q4x.dat'); y = readYFromFile();
    m = length(y);
    figure;
    hold on;
    for i = 1:m
        if y(i,1)==1
            scatter(x(i,1), x(i,2), '*', 'red'); % Canadian flag
        else
            scatter(x(i,1), x(i,2), '*', 'blue'); % US stars
        end
    end
    xx = [40 180];
    y1 = computeYVal(mu0, mu1, sigma, xx(1));
    y2 = computeYVal(mu0, mu1, sigma, xx(2));
    [slope,c] = findSlopeIntercept(xx(1), xx(2), y1, y2);
    slope = double(slope)
    c = double(c)
    hline = refline(slope,c);
    hline.Color = 'g';
end

function [m,c] = findSlopeIntercept(x1, x2, y1, y2)
    m = (y1-y2)/(x1-x2);
    c = y1 - m*x1;
end

function yval = computeYVal(mu0, mu1, sigma, xval)
    sinv = inv(sigma);
    syms f;
    x = [xval ; f];
    eqn = ((x-mu0)' * sinv * (x - mu0)) - ((x-mu1)' * sinv * (x-mu1)) == 0;
    yval = solve(eqn,f);
end

function y = readYFromFile()
    z = textread('q4y.dat','%s');
    for i = 1:length(z)
        y(i,1) = strcmp(z(i,1), 'Canada');
    end
end