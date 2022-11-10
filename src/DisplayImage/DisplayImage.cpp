#include <csignal>
#include <getopt.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>// sleep

const char *PROG_NAME = "DisplayImage";

struct prog_options
{
};

void
display_usage()
{
	printf("usage: %s imagefile [options]\n",PROG_NAME);
	printf(" -h,--help        : display this menu\n");
}

void
parse_args(
	int argc,
	char *argv[],
	prog_options &opts)
{
	static struct option long_options[] =
	{
		{0, 0, 0, 0}
	};

	while (true)
	{
		int option_index = 0;
		int c = getopt_long(
			argc,
			argv,
			"h",
			long_options,
			&option_index);

		// detect the end of the options
		if (c == -1)
		{
			break;
		}

		switch (c)
		{
			case 0:
				// flag setting
				break;
			case 'h':
			case '?':
			default:
				display_usage();
				exit(0);
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	prog_options opts;
	parse_args(argc,argv,opts);

	cv::Mat image;
	printf("openening %s\n", argv[1]);
	image = cv::imread(argv[1], 1);
	if ( ! image.data)
	{
		printf("No image data\n");
		return -1;
	}
	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Image", image);

	cv::waitKey(0);

	return 0;
}