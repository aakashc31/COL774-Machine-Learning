function plotPoints()
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
end

function y = readYFromFile()
    z = textread('q4y.dat','%s');
    for i = 1:length(z)
        y(i,1) = strcmp(z(i,1), 'Canada');
    end
end
