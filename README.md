# Median Filter

Danila Mishin, B05-922

### Build

```bash
mkdir build
cd build
cmake ...
make
```

### Run
```bash
./MedianFilter --radius 25 --filepath ../resources/cat.bmp --algorithm constant
```
For the example above, we run a median filter with a radius of 25
on the picture of the cat at the relative path `../resources/cat.bmp`,
using an algorithm with asymptotics `O(1)`. The following
algorithms are available:

- `simple` - a naive implementation of the median filter
- `huang` - an algorithm by Huang et al.
- `constant` - algorithm `O(1)`
- `opencv` - implementation of opencv (for tests)

Examples for radii 9 and 25 can be found in
folder `resources`. There you can also find a picture of the cat.

The results of the program work the same for all 
the above described algorithms.

### Benchmark

To compare the speeds I have also written a simple benchmark
in Python, it is located in the folder benchmark. An example
of running it:

``bash
python3 main.py --program-path ../cmake-build-release-system/MedianFilter --image-path ../resources/cat.bmp
```

The `requirements.txt` contains all the required modules for the
benchmark. In the same folder you can find the graphs of the dependencies
of the algorithms performance (msec/megapixel)
on the window radius. For the `simple` algorithm the benchmark was not
was not run for R > 10 due to non-optimality of the algorithm.

### Actual asymptotics

Let an image `h x w` be given, a filter of radius `R`. 
Consider the image as a three-channel image. Then 

- `simple`:
`O(h * w * R * logR)`

- `huang`:
`O(h * w * R)`

- `constant`
`O(h * w)`.

And the last two estimates are amortized because
window initialization may take `O(R^2)`, but further
pass will take `O(R)` and `O(1)` (respectively
for algorithms)
for each step of the pass.

### Optimal composition of algorithms

The graphs show that with ` R > 100` the algorithm
`constant` performs better than `huang`. It makes sense, because
For small R in `huang` we have to go through a smaller
number of values (by `O(R)` values, instead of `256`).

It is clear that the given implementation is far from optimal.
When optimizing algorithms, this bound may change.
But in this situation it is `R = 100`. Up to it we should
use `huang`, after it `constant`.
