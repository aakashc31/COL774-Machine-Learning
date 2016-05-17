% This function simply computes the mean squares sum , J(theta)
function j = meansquare(y, theta, x)
    j = 0; m = length(y);
    for i = (1:m)
        temp = y(i) - theta'*x(i,:)';
        temp = 0.5 * temp * temp;
        j = j + temp;
    end
end