function plotFigure(theta)
    x = load('q2x.dat'); y = load('q2y.dat');
    m = length(y);
    figure;
    hold on;
    for i = 1:m
        if y(i,1)==0
            scatter(x(i,1), x(i,2), '*', 'red');
        else
            scatter(x(i,1), x(i,2), '*', 'blue');
        end
    end
    m = -theta(2)/theta(3);
    c = -theta(1)/theta(3);
    hline = refline(m,c);
    hline.Color = 'g';
end
    