function varargout = TwoBottlePreference(varargin)
% TWOBOTTLEPREFERENCE MATLAB code for TwoBottlePreference.fig
%      TWOBOTTLEPREFERENCE, by itself, creates a new TWOBOTTLEPREFERENCE or raises the existing
%      singleton*.
%
%      H = TWOBOTTLEPREFERENCE returns the handle to a new TWOBOTTLEPREFERENCE or the handle to
%      the existing singleton*.
%
%      TWOBOTTLEPREFERENCE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TWOBOTTLEPREFERENCE.M with the given input arguments.
%
%      TWOBOTTLEPREFERENCE('Property','Value',...) creates a new TWOBOTTLEPREFERENCE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before TwoBottlePreference_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to TwoBottlePreference_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help TwoBottlePreference

% Last Modified by GUIDE v2.5 05-Nov-2019 11:38:11

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @TwoBottlePreference_OpeningFcn, ...
                   'gui_OutputFcn',  @TwoBottlePreference_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before TwoBottlePreference is made visible.
function TwoBottlePreference_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to TwoBottlePreference (see VARARGIN)
setappdata(0,'TouchSensorResetCommand',100);
setappdata(0,'SessionStartCommand',101);
setappdata(0,'SessionStopCommand',102);
setappdata(0,'SpoutValvePin',22:33);
%%%%lick data file notes
%column 1: time stamp
%column 2: specification. 0~11: lick channel lick onset. 100~111: lick
%channel lick offset. -1: session start. -2: session pause. -3: session
%resume.  -4: session stop

% Choose default command line output for TwoBottlePreference
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes TwoBottlePreference wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = TwoBottlePreference_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function SessionEndTimer_callback_fcn( hObject,eventdata,handles )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
TimeNow = toc(getappdata(0,'SessionStartTic'))/60;
TestTime = getappdata(0,'TestTime');
if TimeNow>=TestTime
    pushbutton_stop_Callback(hObject, eventdata, handles);
else
    str = ['Time Left: ',num2str(TestTime-floor(TimeNow)),' min'];
    disp(str);
end


% --- Executes on button press in pushbutton_start.
function pushbutton_start_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%%%%%pre-set
disp('start');
ts = timerfind;
if ~isempty(ts)
    stop(timerfind);
    delete(timerfind);
end
flushinput(handles.serial_1);

setappdata(0,'LickBuffer',zeros(501,2));
TestTime = str2double(get(handles.edit_TestTime,'string'));
setappdata(0,'TestTime',TestTime*60);
%%%%%%%%%timer definition
LickReadTimer = timer('TimerFcn',{@LickReadTimer_callback_fcn,handles},'Period',0.02,'ExecutionMode','fixedRate');
% setappdata(0,'LickReadTimer',LickReadTimer);
LickSaveTimer = timer('TimerFcn',{@LickSaveTimer_callback_fcn,handles},'Period',10,'ExecutionMode','fixedRate');
% setappdata(0,'LickSaveTimer',LickSaveTimer);
SessionEndTimer = timer('TimerFcn',{@SessionEndTimer_callback_fcn,handles},'Period',60,'ExecutionMode','fixedRate');
% setappdata(0,'SessionEndTimer',SessionEndTimer);
%%%%set data folder
DataFolderPath = getappdata(0,'DataFolderPath');
if isempty(DataFolderPath)
    DataFolderPath = 'G:\data';
end
DataFolderPath = uigetdir(DataFolderPath,'Select the folder for data');
if ~DataFolderPath
    return;
end
set(handles.pushbutton_stop,'Visible','on');
set(handles.pushbutton_start,'Visible','off');
fileTime = datestr(datetime());
fileTime(fileTime==':') = '_';
fileTime(fileTime==' ') = '_';
LickFileName = [DataFolderPath,'\',fileTime, '.dat'];
setappdata(0,'LickFileName',LickFileName);
setappdata(0,'DataFolderPath',DataFolderPath);
dlmwrite(LickFileName,[now,-1],'-append');%record start time stamp, use datetime(x,'ConvertFrom','datenum') to convert
fwrite(handles.serial_1,getappdata(0,'SessionStartCommand'));%start lick detect command to arduino
start(LickReadTimer);
start(LickSaveTimer);
if get(handles.radiobutton_TestTime,'value')
    start(SessionEndTimer);
    setappdata(0,'SessionStartTic',tic);
end
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% --- Executes on button press in pushbutton_stop.
function pushbutton_stop_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.pushbutton_stop,'Visible','off');
set(handles.pushbutton_start,'Visible','on');
dlmwrite(getappdata(0,'LickFileName'),[now,-4],'-append');%record start time stamp
ts = timerfind;
if ~isempty(ts)
stop(timerfind); 
delete(timerfind);
end
try
    fwrite(handles.serial_1,getappdata(0,'SessionStopCommand'));%arduino session stop command
    fwrite(handles.serial_1,0);%arduino command
catch
end
%%%save non saved data, if there is
LickBuffer = getappdata(0,'LickBuffer');
if LickBuffer(1,1)>=1    
    dlmwrite(getappdata(0,'LickFileName'),LickBuffer(2:LickBuffer(1,1)+1,:),'-append');    
    setappdata(0,'LickBuffer',zeros(501,2));
end
disp('Well Done! Session Stopped');


% --- Executes on button press in radiobutton_ConnectArduinoCom.
function radiobutton_ConnectArduinoCom_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_ConnectArduinoCom (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_ConnectArduinoCom
if get(hObject,'value')    
    com = ['COM',get(handles.edit_com,'string')];
    try
        delete(instrfind('Port',com));
        handles.serial_1 = serial(com);
        set(handles.serial_1,'BaudRate',9600);
        fopen(handles.serial_1);
        str = [com,':Connected!'];
        disp(str);
    catch
        msgbox('Check Arduino');
        set(hObject,'value',0)
        return;
    end
else
    try
    com = ['COM',get(handles.edit_com,'string')];
    fclose(handles.serial_1);
    delete(handles.serial_1);
    str = [com,':Disconnected!'];
    disp(str);
    catch
    end
end


handles.output = hObject;

% Update handles structure
guidata(hObject, handles);


function edit_com_Callback(hObject, eventdata, handles)
% hObject    handle to edit_com (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_com as text
%        str2double(get(hObject,'String')) returns contents of edit_com as a double


% --- Executes during object creation, after setting all properties.
function edit_com_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_com (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton_pause.
function pushbutton_pause_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_pause (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.output = hObject;
% Update handles structure
guidata(hObject, handles);
str = get(hObject,'String');
%%%% stop
if strcmp(str,'Pause')
    fwrite(handles.serial_1,102);
    dlmwrite(getappdata(0,'LickFileName'),[now,-2],'-append');
    set(hObject,'String','Resume','BackgroundColor',[0.9 0.2 0.2]);
end
if strcmp(str,'Resume')
    fwrite(handles.serial_1,101);
    dlmwrite(getappdata(0,'LickFileName'),[now,-3],'-append');
    set(hObject,'String','Pause','BackgroundColor',[0.94 0.94 0.94]);
end


% --- Executes on button press in pushbutton_plot.
function pushbutton_plot_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DataFolderPath = getappdata(0,'DataFolderPath');%if photometry together with HeadFixed three ports
str_fileopen = [DataFolderPath,'\*.dat'];
[filename,pathname,index] = uigetfile(str_fileopen,'Select lickData');
if ~index
    return;
end
setappdata(0,'DataFolderPath',pathname);
display(filename);
str = [pathname,filename];
LickData = importdata(str);
Lick_channelRaw = cell(12,2);
for i = 0:11
    Lick_channelRaw{i+1,1} = LickData(LickData(:,2)==i,:);
    Lick_channelRaw{i+1,2} = LickData(LickData(:,2)==100+i,:);    
end
assignin('base','Lick_channelRaw',Lick_channelRaw);
figure;
for i = 1:2:11
    subplot(2,3,ceil(i/2));
    lickNow = Lick_channelRaw{i,1};
    if ~isempty(lickNow)
    stairs(lickNow(:,1)/60000,1:size(lickNow,1),'-k');hold on;% minutes
    end    
    lickNow = Lick_channelRaw{i+1,1};
    if ~isempty(lickNow)
    stairs(lickNow(:,1)/60000,1:size(lickNow,1),'-r');hold off;

    end
end
assignin('base','lickNow',lickNow);


% --- Executes on button press in radiobutton_TestTime.
function radiobutton_TestTime_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_TestTime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_TestTime



function edit_TestTime_Callback(hObject, eventdata, handles)
% hObject    handle to edit_TestTime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_TestTime as text
%        str2double(get(hObject,'String')) returns contents of edit_TestTime as a double


% --- Executes during object creation, after setting all properties.
function edit_TestTime_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_TestTime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_command_Callback(hObject, eventdata, handles)
% hObject    handle to edit_command (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_command as text
%        str2double(get(hObject,'String')) returns contents of edit_command as a double
command = get(hObject,'string');
set(handles.edit_command,'string',[]);
command = str2num(command);
fwrite(handles.serial_1,command);
str = ['Command is: ',num2str(command)];
disp(str);

% --- Executes during object creation, after setting all properties.
function edit_command_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_command (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton_sendCommand.
function pushbutton_sendCommand_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_sendCommand (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
edit_command_Callback(hObject, eventdata, handles)


% --- Executes during object creation, after setting all properties.
function pushbutton_pause_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pushbutton_pause (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in pushbutton_habituation.
function pushbutton_habituation_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_habituation (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.output = hObject;
% Update handles structure
guidata(hObject, handles);
str = get(hObject,'String');
%%%% stop
if strcmp(str,'Hab')
    Habituation_timer = timer('TimerFcn',{@Habituation_timer_callback_fcn,handles},'Period',300,'ExecutionMode','fixedRate','tag','Habituation_timer');
    start(Habituation_timer);
    set(hObject,'String','StHab','BackgroundColor',[0.9 0.2 0.2]);
end
if strcmp(str,'StHab')
    ts = timerfind('tag','Habituation_timer');
    if ~isempty(ts)
    stop(ts);delete(ts);
    end
    set(hObject,'String','Hab','BackgroundColor',[0.94 0.94 0.94]);
end
