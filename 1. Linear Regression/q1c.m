function [theta, error, allTheta] = batchGradientDescent(x,y,eta, epsilon)
    m = length(x); n = 2; % this is intended to be the number of attributes
    theta = zeros(n,1); % initialize the params as zero vector
    step = 1;
    theta1 = (0.1:0.1:10)'; theta2 = (0.1:0.1:10)';
    for i = 1:length(theta1)
        for j = 1:length(theta2)
            temp = [theta1(i,1) ; theta2(j,1)];
            Z(j,i) = meansquare(y,temp,x)/m;
        end
    end
    figure;
    mesh(theta1, theta2, Z);
    hold on;
    while 1
        for i = 1:m
            delta(i,1) = y(i,1) - x(i,:)*theta;
        end
        % delta is a vector which stores the difference in y(i) and h(x(i))
        for j = 1:n
            temp = 0;
            for i = 1:m
                temp = temp + delta(i,1)*x(i,j);
            end
            theta(j,1) = theta(j,1) + (eta/m)*temp;
        end
        allTheta(step,:) = theta;
        error(step,1) = meansquare(y,theta,x)/m;
        scatter3(theta(1), theta(2), error(step,1),'*','black');
        pause(0.2);
        if (step > 1) && (abs(error(step,1) - error(step-1,1)) < epsilon)
            step
            break
        end
        step = step + 1;
    end
end
            
        
    