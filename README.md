icat - image cat
================
Outputs an image on a 256-color or 24-bit color enabled terminal with UTF-8 locale,
such as gnome-terminal, konsole or rxvt-unicode (urxvt).

Building
--------

Build requirements: icat depends on imlib2.
On Ubuntu, apt-get install libimlib2-dev.

To compile:

	make

Running
-------

Run icat with a local file:

	icat sample.png

or with multiple files:

	icat img1.jpg img2.jpg > newfile.txt

or, if - is used as as file name, it reads from stdin:

	curl -sL https://raw.github.com/atextor/icat/master/sample.png | icat -

The above commands would create the following output (note that you
have to open this README in a console pager such as `less -r' to interpret the
ANSI escape codes that encode the colors):


[0m[38;5;25m▄[0m[38;5;62m▄[38;5;110m[48;5;25m▄[38;5;189m[48;5;25m▄[38;5;7m[48;5;25m▄[38;5;7m[48;5;25m▄[38;5;189m[48;5;25m▄[38;5;110m[48;5;18m▄[0m[38;5;61m▄[0m[38;5;18m▄   [0m
 [0m[38;5;19m▄[38;5;68m[48;5;25m▄[38;5;195m[48;5;68m▄[38;5;195m[48;5;195m▄[38;5;195m[48;5;15m▄[38;5;15m[48;5;15m▄[38;5;15m[48;5;15m▄[38;5;15m[48;5;15m▄[38;5;15m[48;5;15m▄[38;5;195m[48;5;15m▄[38;5;195m[48;5;195m▄[38;5;195m[48;5;68m▄[38;5;68m[48;5;18m▄[0m[38;5;18m▄ [0m
[0m[38;5;19m▄[38;5;32m[48;5;25m▄[38;5;39m[48;5;75m▄[38;5;117m[48;5;153m▄[38;5;153m[48;5;189m▄[38;5;189m[48;5;189m▄[38;5;189m[48;5;195m▄[38;5;195m[48;5;195m▄[38;5;195m[48;5;195m▄[38;5;189m[48;5;195m▄[38;5;189m[48;5;195m▄[38;5;153m[48;5;189m▄[38;5;117m[48;5;153m▄[38;5;39m[48;5;75m▄[38;5;32m[48;5;25m▄[0m[38;5;24m▄[0m
[38;5;25m[48;5;25m▄[38;5;75m[48;5;75m▄[38;5;75m[48;5;39m▄[38;5;75m[48;5;75m▄[38;5;75m[48;5;117m▄[38;5;117m[48;5;153m▄[38;5;117m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;117m[48;5;153m▄[38;5;117m[48;5;153m▄[38;5;75m[48;5;117m▄[38;5;75m[48;5;75m▄[38;5;75m[48;5;39m▄[38;5;75m[48;5;75m▄[38;5;25m[48;5;25m▄[0m
[38;5;25m[48;5;25m▄[38;5;75m[48;5;75m▄[38;5;75m[48;5;75m▄[38;5;117m[48;5;75m▄[38;5;117m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;117m[48;5;117m▄[38;5;117m[48;5;75m▄[38;5;75m[48;5;75m▄[38;5;75m[48;5;75m▄[38;5;25m[48;5;25m▄[0m
[0m[38;5;25m▀[38;5;25m[48;5;74m▄[38;5;117m[48;5;117m▄[38;5;153m[48;5;117m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;117m▄[38;5;117m[48;5;117m▄[38;5;61m[48;5;74m▄[0m[38;5;24m▀[0m
 [0m[38;5;25m▀[38;5;24m[48;5;68m▄[38;5;68m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;153m[48;5;153m▄[38;5;68m[48;5;153m▄[38;5;24m[48;5;68m▄[0m[38;5;18m▀ [0m
   [0m[38;5;18m▀[0m[38;5;61m▀[38;5;18m[48;5;110m▄[38;5;25m[48;5;153m▄[38;5;61m[48;5;153m▄[38;5;61m[48;5;153m▄[38;5;25m[48;5;153m▄[38;5;24m[48;5;110m▄[0m[38;5;61m▀[0m[38;5;18m▀   [0m

For terminals that support 24-bit color (only Konsole and Yakuake are known to the authors at the time of this writing), this can be enabled using:

	icat -m 24bit sample.png


Author
------

icat was written by Andreas Textor <textor.andreas@googlemail.com>.
The sample icon is from the Nuvola icon theme by David Vignoni.


