function SessionEndTimer_callback_fcn( ~,~,handles )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
TimeNow = toc(getappdata(0,'SessionStartTic'))/60;
TestTime = getappdata(0,'TestTime');
if TimeNow>=TestTime
    pushbutton_stop_Callback(hObject, eventdata, handles);
else
    str = ['Time Left:',TestTime-floor(TimeNow)];
    disp(str);
end
end % function end



