# FlashingLights
Three LEDs, with one LED on at a time.  With each advancing level, the LEDs blink on and off randomly at a faster rate.  
The objective of the game is to press the button corresponding to the lit LED before it changes.

# Circuit Schematic
![Alt text](https://github.com/Beezlie/FlashingLights/blob/master/Circuit%20Schematics.PNG?raw=true "Title")

# Creating Custom Sounds
### Follow these steps to convert a .wav file to a .txt file that you can copy and paste into the code to add your own sounds.
### The .wav files must be short, since they take up a large amount of memory.

1. Read the file and extract the samples as well as the frequency:
  * [Spls, fs] = wavread('blah.wav');
2. Downsample it, to get it to the frequency you want (11.025 kHz):
  * Spls = downsample(Spls, round(fs/11025));
3. Adjust the samples (originals are fractions between -1 and 1) to range between 0 and 15 (4-bit)
  * Spls = round((Spls+ 1)* 7.5);
4. write it to a file so you can cut and paste it into the sound.c file:
  * file = fopen('blah.txt', 'w');
  * fprintf(file, 'unsigned char blah[] = {');
  * fprintf(file, '%d,', Spls);
  * fprintf(file, '};\n');
