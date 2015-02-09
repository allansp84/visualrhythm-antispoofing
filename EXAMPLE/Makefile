# Author: Allan Pinto
# Descrition: Testing the computation of the visual rhythms

all: compile vertical horizontal zigzag

vertical:
	mkdir -p output/visualrhythm/vertical
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase1.avi -output_image output/visualrhythm/vertical/testcase1.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase2.avi -output_image output/visualrhythm/vertical/testcase2.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase3.avi -output_image output/visualrhythm/vertical/testcase3.png

horizontal:
	mkdir -p output/visualrhythm/horizontal
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 1 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase1.avi -output_image output/visualrhythm/horizontal/testcase1.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 1 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase2.avi -output_image output/visualrhythm/horizontal/testcase2.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 1 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase3.avi -output_image output/visualrhythm/horizontal/testcase3.png

zigzag:
	mkdir -p output/visualrhythm/zigzag
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 2 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase1.avi -output_image output/visualrhythm/zigzag/testcase1.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 2 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase2.avi -output_image output/visualrhythm/zigzag/testcase2.png
	../Release/VisualRhythmAntiSpoofing -visual_rhythm_type 2 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase3.avi -output_image output/visualrhythm/zigzag/testcase3.png

compile: clean
	make -C ../Release

clean:
	make -C ../Release clean
	rm -rf output/*
