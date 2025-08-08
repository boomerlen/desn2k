% Matlab script to read the file and confirm it indeed works
% Completely hard-coded here to use the example

fileid = fopen('../assets/HL1_song11.iso');
raw_in = fread(fileid);

y = raw_in(5:end);
y = y - 128;
y = y / 128;

sound(y, 44100);
