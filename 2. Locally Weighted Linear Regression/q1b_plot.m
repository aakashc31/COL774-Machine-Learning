function plotLearnedParams(x,y,theta)
    yLearned = x*theta;
    plot(x(:,2),y ,'xr', x(:,2) , yLearned,'--.b')
    %legend('Observed', 'Learned')
    xlabel('X Axis')
    ylabel('Y Axis')
    title('Plot')
    grid on
end