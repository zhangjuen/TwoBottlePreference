function LickSaveTimer_callback_fcn( ~,~,handles )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
LickBuffer = getappdata(0,'LickBuffer');
if LickBuffer(1,1)>=1    
    dlmwrite(getappdata(0,'LickFileName'),LickBuffer(2:LickBuffer(1,1)+1,:),'-append');    
    setappdata(0,'LickBuffer',zeros(501,2));
end
end % function end



