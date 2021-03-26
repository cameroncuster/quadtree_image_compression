Qualatative impressions and application documentation.

As expected the number of leaf nodes is inversely releated to the image threshold.

Application uses dependency injection as design pattern with professional coding
standards and documentation for in-house application. The QuadTree was developed
with rapid prototyping and test-driven-development.

The design leverages object oriented programming to create interfaces which are
tested at their seams. The quadTree is implemented for high effeciency and
applicability to wide range of images. As a result of implementing the increment 
and decrement threshold functions in the QuadTree with insert and delete
routines the functions operated with outstanding performance. Other subtle
performance enhancements were made to support a range of different images.

The increment and decrement threshold operate on a "byte" value by incrementing
or decrementing the theshold byte value respectively. The decision to utilize an
integer value as opposed to a floating point threshold with a floating point
increment and decrement sacrificed precision; however, while the integer
threshold lacks in precision it makes up for in applicability. The integer
precision allows for better interpretation from a user ( in-general ) by element
of least suprise. Overall the application focuses were equally performance and
usability.

Log of bug-fixes:
* double drawing lines
* serpinskis triangle
* communication
