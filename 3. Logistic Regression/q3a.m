function theta = logisticRegression(epsilon)
    load q2x.dat; load q2y.dat;
    m = length(q2x);
    x = [ones(m,1), q2x]; y = q2y; n = 3;
    theta = zeros(n,1);
    step = 1;
    while 1
        H = computeHessian(theta, x);
        if(det(H)==0)
            llll = [123 23]
            break
        end
        Del = computeFirstDerivative(theta, x, y);
        theta = theta - (H\Del);
        s(step,1) = computeLikelihood(theta, x, y);
        if(step>1 && (abs(s(step,1) - s(step-1,1)) < epsilon))
            step
            break
        end
        step = step + 1;
    end
end


function Del = computeFirstDerivative(theta, x, y)
    m = length(x); n = 3;
    for i = 1:m
        delta(i,1) = y(i,1) - g(x(i,:) * theta);
    end
    for j = 1:n
        Del(j,1) = sum(delta .* x(:,j));
    end
end

function H = computeHessian(theta, x)
    m = length(x);
    n = 3;
    for i = 1:m
        z = x(i,:) * theta;
        h(i,1) = g(z);
    end
    for j = 1:n
        for k = 1:n
            s = 0;
            for i = 1:m
                s = s + x(i,j) * x(i,k) * h(i) * (1 - h(i));
            end
            H(j,k) = -s;
        end
    end
end

function ret = g(x)
    ret = 1/(1+exp(-x));
end

function s = computeLikelihood(theta, x, y)
    s = 0;
    for i = 1:length(y)
        temp = g(x(i,:)*theta);
        s = s + y(i,1)*log(temp)  + (1 - y(i,1))*log(1 - temp);
    end
end