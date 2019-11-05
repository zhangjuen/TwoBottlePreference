function Habituation_timer_callback_fcn( ~,~,handles )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
SpoutValvePin = getappdata(0,'SpoutValvePin');

for i = 1:length(SpoutValvePin)
    fwrite(handles.serial_1,SpoutValvePin(i)+200);%give 1 drop of solution
end

end % function end



