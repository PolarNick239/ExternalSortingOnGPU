# Generating input data

```
./input_generator 5000000000 input.data
```

```
Saving 5000000000 random floats (19073 MB) to file input.data
Done in 48.74 seconds (391.331 MB/s)
```

# Sorting

```
./external_sort input.data output.data 1
```

```
Using device: AMD Radeon (TM) R9 390 Series (Hawaii)
Values number: 5000000000 (19073 MB)
Pass #0: sorting part by part in core...
    In core parts number: 299
    Limit for values in core: 16777216 (64 MB)
    IO: 252.77 MB/s
    Finished in 150.917 s (49% reading + 19% sorting + 32% writing)
Pass #1: merging groups of 16 parts...
    Input parts: 299 with 16777216 values (64 MB) in each
    IO: 421.337 MB/s
    Finished in 90.539 s
Pass #2: merging groups of 16 parts...
    Input parts: 19 with 268435456 values (1024 MB) in each
    IO: 183.899 MB/s
    Finished in 207.435 s
Pass #3: merging groups of 16 parts...
    Input parts: 2 with 4294967296 values (16384 MB) in each
    IO: 250.762 MB/s
    Finished in 152.125 s
Finished in 601.021 s
```
