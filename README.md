cs293_autumn2012
================

Course Project for CS 293 - Autumn 2012

Run this command to compile the files

g++ \`pkg-config opencv --cflags\` VideoProcessor.cpp FeatureTracker.cpp Tracker.cpp -o Tracker \`pkg-config opencv --libs\`

The program by default takes the input "sample_video_1.mov" as the input. The tar file gives 4 sample videos. To try with any of the other videos, change the parameter of "processor.setInput ("sample_video_1.mov")" in line 15 of Tracker.cpp to any of the other videos namely sample_video_2.mp4, sample_video_3.mp4, sample_video_4.mp4. To take input from a webcam, change the line to "processor.setInput (0)".

Output is in the terminal and it outputs whether the object has moved left or right. For recorded videos, you can see lines being drawn on the video showing where it is moving. However, for webcam only output in terminal is shown.