Apriltags
====

Apriltags are a fiducial marker placed in the scene by a user, enabling a computer vision (or really any system) to use it as a point of reference. Apriltags are designed to be used to identify robots, and have encoded in each tag (which looks similar to a QR code). 

This encoded information can be used to not only identify the robot, but the tag itself can also be used to identify ones relative position. It's possible to determine the position based a method called [homography](http://en.wikipedia.org/wiki/Homography_%28computer_vision%29), or pose estimation. 

Our implementation takes most of the code from the origional [Apriltags website](http://april.eecs.umich.edu/wiki/index.php/AprilTags), however we make it run in real time. We also have expanded it to work with something we call chromatags, an improved version of apriltags in development.

### To Build:  
1. `lettergrams-apriltags $ make`  
2. `mkdir build && cd build`  
3. `cmake .. && make`  

####Invoke single image

$ ./apriltags_demo [options] input_image.pnm

Options: 
	 -d: Debug, displays image at every step

####To invoke realtime tracking

$ cd build

$ ./apriltags

Boxes should apear around the tags, along with fps, score, etc.