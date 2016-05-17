function plotPointsAndLine(mu0, mu1, sigma0, sigma1)
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
    
    xx = [40 : 10 : 180];
    %yy = zeros(length(xx));
    for i = 1:length(xx)
        yval = computeYVal(mu0, mu1, sigma0, sigma1, xx(i));
        yval = double(yval);
        yy(i) = yval(2);
    end
    yy;
    plot(xx,yy,'green');
end


function yval = computeYVal(mu0, mu1, sigma0, sigma1, xval)
    sinv0 = inv(sigma0);
    sinv1 = inv(sigma1);
    syms f;
    x = [xval ; f];
    eqn = ((x-mu0)' * sinv0 * (x - mu0)) - ((x-mu1)' * sinv1 * (x-mu1)) == 0;
    yval = solve(eqn,f);
end

function y = readYFromFile()
    z = textread('q4y.dat','%s');
    for i = 1:length(z)
        y(i,1) = strcmp(z(i,1), 'Canada');
    end
end