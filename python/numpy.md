# NumPy

Homepage: <https://numpy.org>

<!-- MarkdownTOC -->

- [Exclude outliers from array](#exclude-outliers-from-array)

<!-- /MarkdownTOC -->

## Exclude outliers from array

``` py
import numpy

# outliers here are `1`, `1e15` and `10`
lst = [4, 4, 4, 4, 3, 5, 6, 2, 4, 4, 1, 1e15, 4, 4, 3, 10]
nmparr = numpy.array(lst)

def outliersMask(
    originalArray: numpy.array,  # without NaNs/INFs
    sigma: int = 3
):
    med = numpy.median(originalArray)
    sig = 1.48 * numpy.median(numpy.abs(originalArray - med))

    rez = [x for x in originalArray if (x > med - sigma * sig)]
    rez = [x for x in rez if (x < med + sigma * sig)]

    return numpy.array(rez)

mskDeviated = outliersMask(nmparr)
print(mskDeviated)
# [4. 4. 4. 4. 3. 5. 6. 2. 4. 4. 4. 4. 3.]
```
