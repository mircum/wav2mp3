# wav2mp3
This small project was created as a cross-platform (Windows, Linux, MacOS) coding playground. 
The outcome is a simple, multi-threaded wav to mp3 converter. It is only able to convert RIFF WAVE files with PCM audio format.
For wav to mp3 conversion, it is used lame library 3.99.5. More details here: http://lame.sourceforge.net/download.php

## Running
The repo contains already compiled versions for Linux, Windows and MacOS. These are available in bin.
To execute, simply call the right binary for your platform like this:
./wav2mp3 /path/to/wave/files
wav2mp3.exe c:\path\to\wave\files

Some sample wave files can be found here: https://drive.google.com/drive/folders/1P61Joo2x1xIFnML9WVl_GZhqL0H1sBAv?usp=sharing


## Compiling yourself

## References
* Thread Pool: https://stackoverflow.com/questions/26516683/reusing-thread-in-loop-c
* Dirent for Windows: https://web.archive.org/web/20171123102131/https://github.com/tronkko/dirent
* Compiling lame with msvc: https://wehuberconsultingllc.com/wordpress/2007/12/30/building-the-lame-mp3-encoder-using-visual-studio-8-express/
* Using lame: https://stackoverflow.com/questions/2495420/is-there-any-lame-c-wrapper-simplifier-working-on-linux-mac-and-win-from-pure/2496831#2496831
* Wave file format: http://soundfile.sapp.org/doc/WaveFormat/
* Parsing a wave file: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
* Multithreading: https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/
