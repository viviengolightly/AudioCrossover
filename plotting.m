

%High pass filter Vmax values for cut off frequency 1kHz
HPF=[0,104,128,168,192,224,272,340,384,400,432,464,488,500,504,576,576,576,576,600,616,616,616];
%Low pass filter Vmax values for cut off frequency 1kHz
LPF =[0,584,584,584,584,584,584,550,536,528,520,512,480,328,168,160,112,104,104,104,104,104,104];
%Frequencies of the input signal used for measurments.
Frequencies = [0,100,200,300,400,500,600,700,800,850,900,950,1000,1500,2000,3000,4000,5000,6000,7000,8000,9000,10000];
for a = 0 : length(LPF)-1

indB(a+1) = 20*log2(LPF(a+1)/584); %Calculate magnitudes in dB for LPF
indB1(a+1) = 20*log2(HPF(a+1)/616);%Calculate magnitudes in dB for HPF
end

plot(Frequencies, indB) %plot the LPF frequency response

hold

plot(Frequencies, indB1) %plot the HPF frequency response

grid