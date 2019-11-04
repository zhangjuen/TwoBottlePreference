function LickReadTimer_callback_fcn( ~,~,handles )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
inputNum = handles.serial_1.BytesAvailable;
if inputNum>=1  % should be 'inputNum', not handles.serial_1.BytesAvailable, otherwise, in some case if the exact time has inputs, inputNum will be 0, but BytesAvailable not 0
    LickBuffer = getappdata(0,'LickBuffer');
    LickBuffer(1,1) = LickBuffer(1,1)+1;
    TimeStamp = str2double(fgets(handles.serial_1));
    ChannelNote = str2double(fgets(handles.serial_1));
    LickBuffer(LickBuffer(1,1)+1,:) = [TimeStamp ChannelNote];
    setappdata(0,'LickBuffer',LickBuffer);
%     disp([inputNum,TimeStamp,ChannelNote])
      
end % serial avalable end
end % function end



