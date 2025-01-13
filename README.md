# BZZ compressor

This is made using code from the oldest djvu snapshot
https://sourceforge.net/p/djvu/old-djvu22-git/ci/master/tree/ and
hacking it to increase its limitations on the Burrows-Wheeler buffer
size.


## Usage

Encoding: `bzz -e<bufsize> <infile> <compressedfile>`

Decoding: `bzz -d <compressedfile> <outfile>`

File specifications can be `-` to indicate stdin or stdout.

Block size specification is a number followd by suffix K, M, or G for kilobytes, megabytes, or gigabytes. The default is 2M. 
The maximal size used to be 4M, but has been increased to 1G.

Note that the bzz format encodes the data as a succession of separately encoded blocks.
So for the purposes of measuring entropy, one should have a large enough block size.
There is about a 3-5% overhead because of the entropy coder adaptation costs.
There is also eight bytes of overhead to mark the block boundaries.

Maybe I could save some time by preventing it from writing anything, just returning a file size.
However writing a file is a good way to make sure we can decode it.

