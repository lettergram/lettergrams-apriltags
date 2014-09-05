CC=gcc -pthread -g -std=gnu99 -Wall -Wno-unused-parameter -Wno-format-zero-length -O4

# For most x86_64 platforms this will be a bit faster.
#
# CC += -msse4.2


TAGTEST_OBJS = apriltag.o tagtest.o image_f32.o image_u8.o image_u32.o unionfind.o zhash.o zarray.o matd.o homography.o graymodel.o tag36h11.o tag36h10.o segment2.o workerpool.o g2d.o

ODIR = obj

all: tagtest


tagtest:	$(TAGTEST_OBJS)
	$(CC) -o $@ $(TAGTEST_OBJS) -lm -lpthread

clean:
	rm -f $(ODIR)/*.o *~ tagtest cachegrind.out* callgrind.out* gmon.out

save-obj:
	mv *.o $(ODIR)
