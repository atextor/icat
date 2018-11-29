/*
icat -- Outputs an image on a 256-color or 24 bit color enabled terminal with UTF-8 locale
Andreas Textor <textor.andreas@googlemail.com>

Compile: gcc -Wall -pedantic -std=c99 -D_BSD_SOURCE -o icat icat.c -lImlib2

Copyright (c) 2012 Andreas Textor. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <Imlib2.h>

#define VERSION "0.5"

enum {MODE_NOTHING, MODE_INDEXED, MODE_24_BIT, MODE_BOTH};

static uint32_t colors[] = {
	// Colors 0 to 15: original ANSI colors
	0x000000, 0xcd0000, 0x00cd00, 0xcdcd00, 0x0000ee, 0xcd00cd, 0x00cdcd, 0xe5e5e5,
	0x7f7f7f, 0xff0000, 0x00ff00, 0xffff00, 0x5c5cff, 0xff00ff, 0x00ffff, 0xffffff,
	// Color cube colors
	0x000000, 0x00005f, 0x000087, 0x0000af, 0x0000d7, 0x0000ff, 0x005f00, 0x005f5f,
	0x005f87, 0x005faf, 0x005fd7, 0x005fff, 0x008700, 0x00875f, 0x008787, 0x0087af,
	0x0087d7, 0x0087ff, 0x00af00, 0x00af5f, 0x00af87, 0x00afaf, 0x00afd7, 0x00afff,
	0x00d700, 0x00d75f, 0x00d787, 0x00d7af, 0x00d7d7, 0x00d7ff, 0x00ff00, 0x00ff5f,
	0x00ff87, 0x00ffaf, 0x00ffd7, 0x00ffff, 0x5f0000, 0x5f005f, 0x5f0087, 0x5f00af,
	0x5f00d7, 0x5f00ff, 0x5f5f00, 0x5f5f5f, 0x5f5f87, 0x5f5faf, 0x5f5fd7, 0x5f5fff,
	0x5f8700, 0x5f875f, 0x5f8787, 0x5f87af, 0x5f87d7, 0x5f87ff, 0x5faf00, 0x5faf5f,
	0x5faf87, 0x5fafaf, 0x5fafd7, 0x5fafff, 0x5fd700, 0x5fd75f, 0x5fd787, 0x5fd7af,
	0x5fd7d7, 0x5fd7ff, 0x5fff00, 0x5fff5f, 0x5fff87, 0x5fffaf, 0x5fffd7, 0x5fffff,
	0x870000, 0x87005f, 0x870087, 0x8700af, 0x8700d7, 0x8700ff, 0x875f00, 0x875f5f,
	0x875f87, 0x875faf, 0x875fd7, 0x875fff, 0x878700, 0x87875f, 0x878787, 0x8787af,
	0x8787d7, 0x8787ff, 0x87af00, 0x87af5f, 0x87af87, 0x87afaf, 0x87afd7, 0x87afff,
	0x87d700, 0x87d75f, 0x87d787, 0x87d7af, 0x87d7d7, 0x87d7ff, 0x87ff00, 0x87ff5f,
	0x87ff87, 0x87ffaf, 0x87ffd7, 0x87ffff, 0xaf0000, 0xaf005f, 0xaf0087, 0xaf00af,
	0xaf00d7, 0xaf00ff, 0xaf5f00, 0xaf5f5f, 0xaf5f87, 0xaf5faf, 0xaf5fd7, 0xaf5fff,
	0xaf8700, 0xaf875f, 0xaf8787, 0xaf87af, 0xaf87d7, 0xaf87ff, 0xafaf00, 0xafaf5f,
	0xafaf87, 0xafafaf, 0xafafd7, 0xafafff, 0xafd700, 0xafd75f, 0xafd787, 0xafd7af,
	0xafd7d7, 0xafd7ff, 0xafff00, 0xafff5f, 0xafff87, 0xafffaf, 0xafffd7, 0xafffff,
	0xd70000, 0xd7005f, 0xd70087, 0xd700af, 0xd700d7, 0xd700ff, 0xd75f00, 0xd75f5f,
	0xd75f87, 0xd75faf, 0xd75fd7, 0xd75fff, 0xd78700, 0xd7875f, 0xd78787, 0xd787af,
	0xd787d7, 0xd787ff, 0xd7af00, 0xd7af5f, 0xd7af87, 0xd7afaf, 0xd7afd7, 0xd7afff,
	0xd7d700, 0xd7d75f, 0xd7d787, 0xd7d7af, 0xd7d7d7, 0xd7d7ff, 0xd7ff00, 0xd7ff5f,
	0xd7ff87, 0xd7ffaf, 0xd7ffd7, 0xd7ffff, 0xff0000, 0xff005f, 0xff0087, 0xff00af,
	0xff00d7, 0xff00ff, 0xff5f00, 0xff5f5f, 0xff5f87, 0xff5faf, 0xff5fd7, 0xff5fff,
	0xff8700, 0xff875f, 0xff8787, 0xff87af, 0xff87d7, 0xff87ff, 0xffaf00, 0xffaf5f,
	0xffaf87, 0xffafaf, 0xffafd7, 0xffafff, 0xffd700, 0xffd75f, 0xffd787, 0xffd7af,
	0xffd7d7, 0xffd7ff, 0xffff00, 0xffff5f, 0xffff87, 0xffffaf, 0xffffd7, 0xffffff,
	// >= 233: Grey ramp
	0x000000, 0x121212, 0x1c1c1c, 0x262626, 0x303030, 0x3a3a3a, 0x444444, 0x4e4e4e,
	0x585858, 0x626262, 0x6c6c6c, 0x767676, 0x808080, 0x8a8a8a, 0x949494, 0x9e9e9e,
	0xa8a8a8, 0xb2b2b2, 0xbcbcbc, 0xc6c6c6, 0xd0d0d0, 0xdadada,
};

// Find an xterm color value that matches an ARGB color
uint8_t rgb2xterm(Imlib_Color* pixel) {
	uint8_t c, match = 0;
	uint32_t r, g, b, d, distance;

	distance = 1000000000;
	for(c = 0; c <= 253; c++) {
		r = ((0xff0000 & colors[c]) >> 16) - pixel->red;
		g = ((0x00ff00 & colors[c]) >> 8)  - pixel->green;
		b = (0x0000ff & colors[c]) - pixel->blue;
		d = r * r + g * g + b * b;
		if (d < distance) {
			distance = d;
			match = c;
		}
	}

	return match;
}

void print_usage() {
	printf("icat (" VERSION ") outputs an image on a 256-color or 24-bit color enabled terminal with UTF-8 locale.\n"
			"Usage: icat [-h|--help] [-x value] [-y value] [-w|--width value] [-k|--keep]\n"
			"            [-m|--mode indexed|24bit|both] imagefile [imagefile...]\n"
			"	-h | --help  -- Display this message\n"
			"	-x value     -- Specify the column to print the image in (min. 1)\n"
			"	-y value     -- Specify the row to print the image in (min. 1)\n"
			"	                This is ignored when more than one image is printed.\n"
			"	-w | --width value\n"
			"	                Instead of resizing the image to fit the terminal width,\n"
			"	                use the provided value as the desired width.\n"
			"	-k | --keep  -- Keep image size, i.e. do not automatically resize image to fit\n"
			"	                the terminal width.\n"
			"	-m | --mode indexed|24bit|both\n"
			"	                Use indexed (256-color, the default), 24-bit color, or both.\n"
			"	imagefile    -- The image to print. If the file name is \"-\", the file is\n"
			"	                read from stdin.\n"
			"Big images are automatically resized to your terminal width, unless with the -k option.\n"
			"You can achieve the same effect with convert (from the ImageMagick package):\n"
			"	convert -resize $((COLUMNS - 2))x image.png - | icat -\n"
			"Or read images from another source:\n"
			"	curl -sL http://example.com/image.png | convert -resize $((COLUMNS - 2))x - - | icat -\n");
}

// Find out and return the number of columns in the terminal
int terminal_width() {
	int cols = 80;
#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	cols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	cols = ts.ws_col;
#endif /* TIOCGSIZE */
	return cols;
}

// Resize image, but only if the user explicitly provided the image width,
// or the image wouldn't fit in the terminal.
void resize_image_if_necessary(int *width, int *height, const int user_width) {
	int resized_width;

	if (!user_width) {
		int cols = terminal_width();
		if(cols > *width) return;

		resized_width = cols;
	} else {
		resized_width = user_width;
	}

	float ratio = ((float)resized_width) / *width;
	int resized_height = *height * ratio;

	Imlib_Image resized_image = imlib_create_cropped_scaled_image(0, 0,
			*width, *height, resized_width, resized_height);
	imlib_free_image_and_decache();
	imlib_context_set_image(resized_image);

	*width = resized_width;
	*height = resized_height;
}

// Prints two pixels inside one character, p1 below p2.
// Characters in terminal fonts are usually twice as high
// as they are wide.
void print_pixels(Imlib_Color* p1, Imlib_Color* p2, int mode) {
	// Newer xterms should support 24-bit color with the ESC[38;2;<r>;<g>;<b>m sequence.
	// For backward compatibility, we insert the old ESC[38;5;<color_index>m before it.
	static char* upper = "▀";
	static char* lower = "▄";
	if (p1->alpha == 0 && p2->alpha == 0) {
		// Both pixels are transparent
		printf("\x1b[0m ");
	} else if (p1->alpha == 0 && p2->alpha != 0) {
		// Only lower pixel is transparent
		fputs("\x1b[0m", stdout);
		if(mode & MODE_INDEXED) {
			uint8_t col2 = rgb2xterm(p2);
			printf("\x1b[38;5;%dm", col2);
		}
		if(mode & MODE_24_BIT) {
			printf("\x1b[38;2;%d;%d;%dm", p2->red, p2->green, p2->blue);
		}
		fputs(upper, stdout);
	} else if (p1->alpha != 0 && p2->alpha == 0) {
		// Only upper pixel is transparent
		fputs("\x1b[0m", stdout);
		if(mode & MODE_INDEXED) {
			uint8_t col1 = rgb2xterm(p1);
			printf("\x1b[38;5;%dm", col1);
		}
		if(mode & MODE_24_BIT) {
			printf("\x1b[38;2;%d;%d;%dm", p1->red, p1->green, p1->blue);
		}
		fputs(lower, stdout);
	} else {
		// Both pixels are opaque
		if(mode & MODE_INDEXED) {
			uint8_t col1 = rgb2xterm(p1);
			uint8_t col2 = rgb2xterm(p2);
			printf("\x1b[38;5;%dm\x1b[48;5;%dm", col1, col2);
		}
		if(mode & MODE_24_BIT) {
			printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", p1->red, p1->green, p1->blue, p2->red, p2->green, p2->blue);
		}
		fputs(lower, stdout);
	}
}

int main(int argc, char* argv[]) {
	char *filename;
	static int display_help = 0;
	Imlib_Image image = NULL;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int user_w = 0;
	int c;
	bool keep_size = false;
	int mode = MODE_INDEXED;

	for(;;) {
		static struct option long_options[] = {
			{"help", no_argument,       &display_help, 1},
			{"x",    required_argument, 0, 'x'},
			{"y",    required_argument, 0, 'y'},
			{"width",required_argument, 0, 'w'},
			{"keep", no_argument,       0, 'k'},
			{"mode", required_argument, 0, 'm'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hx:y:w:km:", long_options, NULL);

		if (c == -1)
			break;

		switch (c) {
			case 'x':
				x = atoi(optarg);
				if (x < 0) {
					x = 0;
				}
				break;

			case 'y':
				y = atoi(optarg);
				if (y < 0) {
					y = 0;
				}
				break;

			case 'w':
				user_w = atoi(optarg);
				if(user_w < 1) {
					printf("Image width must be larger than 0\n");
					exit(1);
				}
				break;

			case 'h':
				display_help = 1;
				break;

			case 'k':
				keep_size = true;
				break;

			case 'm':
				if(strcmp(optarg, "indexed") == 0) {
					mode = MODE_INDEXED;
				} else if(strcmp(optarg, "24bit") == 0) {
					mode = MODE_24_BIT;
				} else if(strcmp(optarg, "both") == 0) {
					mode = MODE_BOTH;
				} else {
					printf("Mode must be one of 'indexed', '24bit', or 'both'.\n");
					exit(1);
				}
				break;

			case '?':
				break;

			default:
				display_help = 1;
				break;
		}
	}

	if (display_help == 1 || optind == argc) {
		print_usage();
		exit(0);
	}

	// Ignore y value when more than one picture is printed
	if (argc - optind > 1) {
		y = 0;
	}

	for (int i = optind; i < argc; i++) {
		// Read from stdin and write temp file. Although a temp file
		// is ugly, imlib can not seek in a pipe and therefore not
		// read an image from it.
		char tempfile_name[] = "/tmp/icatXXXXXX";
		if (strcmp(argv[i], "-") == 0) {
			int tempfile;
			if ((tempfile = mkstemp(tempfile_name)) < 0) {
				perror("mkstemp");
				exit(EXIT_FAILURE);
			}
			filename = tempfile_name;
			char buf;
			while (read(0, &buf, 1) > 0) {
				write(tempfile, &buf, 1);
			}
		} else {
			filename = argv[i];
		}

		// Load image
		image = imlib_load_image_immediately(filename);
		if (!image) {
			fprintf(stderr, "Could not load image: %s\n", filename);
			exit(EXIT_FAILURE);
		}

		imlib_context_set_image(image);
		int width = imlib_image_get_width();
		int height = imlib_image_get_height();

		// Unless told to retain image size, check if resize if needed
		if (!keep_size) {
			resize_image_if_necessary(&width, &height, user_w);
		}

		// If an y-value is given, position the cursor in that line (and
		// given or default column)
		if (y > 0) {
			printf("\x1b[%d;%dH", y, x);
		}

		Imlib_Color pixel1;
		Imlib_Color pixel2;
		for (int h = 0; h < height; h += 2) {
			// If an x-offset is given, position the cursor in that column
			if (x > 0) {
				printf("\x1b[%dG", x);
			}

			// Draw a horizontal line. Each console line consists of two
			// pixel lines in order to keep the pixels square (most console
			// fonts have two times the height for the width of each character)
			for (int w = 0; w < width; w++) {
				imlib_image_query_pixel(w, h + 1, &pixel1);
				imlib_image_query_pixel(w, h, &pixel2);
				print_pixels(&pixel1, &pixel2, mode);
			}
			printf("\x1b[0m\n");
		}

		imlib_free_image_and_decache();
	}
	return 0;
}
