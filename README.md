Qualitative impressions and application documentation.

As expected, the number of leaf nodes is inversely related to the image
threshold.  The overall compression behaves logarithmically with respect to the
number of leaf nodes; inversely, the number of leaf nodes appears exponential
with respect to the image compression. For images such as the Jupiter image,
even with a threshold of zero the image is still compressed by 40% because there
are so many areas with the same value in the background; however, the Lena image
with a threshold of zero the compression rate is almost 200%, because nearly
every pixel in the image must be represented by a leaf node, and the overhead of
the QuadTree is expensive. The overall relationship between the threshold and
quadrant size is inversely exponential, so consequentially the image quality grows
exponentially lower as the threshold increases. The concept of the QuadTree
and it's respective compression growth rate is reasonable for a variety of
applications.

The application uses dependency injection as design pattern with professional
coding standards and documentation for in-house application. The QuadTree was
developed with rapid prototyping.

The design leverages object oriented programming to create interfaces which are
tested at their seams. The QuadTree is implemented for high efficiency and
applicability to wide range of images. As a result of implementing the increment
and decrement threshold functions in the QuadTree with insert and delete
routines the functions operate with outstanding performance. Other subtle
performance enhancements were made to support a range of various images.

The increment and decrement threshold operate on a "byte" value by incrementing
or decrementing the threshold byte value respectively. The decision to utilize an
integer value as opposed to a floating point threshold with a floating point
increment and decrement sacrificed precision; however, while the integer
threshold lacks in precision it makes up for in applicability. The integer
precision allows for better interpretation from a user ( in-general ) by element
of least surprise. Overall the success of the application is reflected by the
focuses in development which were equally performance and usability.

Given the opportunity to rewrite the QuadTree we would design the QuadTree with
an interface to a node class for testing at the seam. The node class would also
produce a less tightly coupled interface between the QuadTree and it's node
structure making the code more testable, readable, and reusable.
