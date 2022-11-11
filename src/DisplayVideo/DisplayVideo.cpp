#include <csignal>
#include <getopt.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>// sleep

const char *PROG_NAME = "DisplayVideo";

struct prog_options
{
};

void
display_usage()
{
	printf("usage: %s videofile [options]\n",PROG_NAME);
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

	printf("openening %s\n", argv[1]);
	cv::VideoCapture vcap(argv[1]);
	if ( ! vcap.isOpened())
	{
		printf("No image data\n");
		return -1;
	}

	const auto OUT_VIDEO_SIZE = cv::Size(1280,720);
	const auto TEXT_FONT = cv::FONT_HERSHEY_DUPLEX;
	const auto TEXT_COLOR = CV_RGB(118, 185, 0);
	double frames_count = vcap.get(cv::CAP_PROP_FRAME_COUNT);
	double fps = vcap.get(cv::CAP_PROP_FPS);
	double frame_time_sec = 1.0 / fps;
	int frame_time_ms = std::round(1000.0 / fps);
	printf("frames_count = %f\n", frames_count);
	printf("fps = %f\n", fps);
	printf("frame_time_ms = %d\n", frame_time_ms);

	cv::Mat frame;
	cv::Mat out_frame;
	char frame_idx_str[1024];
	char frame_time_str[1024];
	double time_offset_sec = 0.0;
	for (size_t frame_idx=0; frame_idx<frames_count; frame_idx++)
	{
		// Capture frame-by-frame
		if (vcap.read(frame))
		{
			cv::resize(frame,out_frame,OUT_VIDEO_SIZE);

			sprintf(frame_idx_str,"frame_idx: %ld",frame_idx);
			cv::putText(
				out_frame, //target image
				frame_idx_str, //text
				cv::Point(10, 30), //top-left position
				TEXT_FONT,// font face
				1.0,// font scale
				TEXT_COLOR, //font color
				1);// thickness

			sprintf(frame_time_str,"time_offset: %0.3fs",time_offset_sec);
			cv::putText(
				out_frame, //target image
				frame_time_str, //text
				cv::Point(10, 30 * 2), //top-left position
				TEXT_FONT,// font face
				1.0,// font scale
				TEXT_COLOR, //font color
				1);// thickness

			// Display the resulting frame
			cv::imshow("Video", out_frame);

			// Press Q on keyboard to exit
			if (cv::waitKey(frame_time_ms) & 0xFF == 'q')
			{
				printf("Quit video playback!\n");
				break;
			}

			time_offset_sec += frame_time_sec;
		}
		else
		{
			// Break the loop
			// i'm seeing GoPro videos do this about 1s into the clip.
			// i wonder if they're encoder some metadata or something.
			printf("frame %ld is bad?\n", frame_idx);
		}
	}

	return 0;
}