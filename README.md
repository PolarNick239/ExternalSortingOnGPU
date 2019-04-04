# Generating input data

```
./input_generator 1000000000 input.data
```

```
Saving 1000000000 random floats (3814 MB) to file input.data
Done in 9.94176 seconds (383.704 MB/s)
```

# Sorting

```
./external_sort input.data output.data
```

```
Values number: 1000000000 (3814 MB)
Pass #0: sorting part by part in core...
    In core parts number: 60
    Limit for values in core: 16777216 (64 MB)
    IO: 51.1294 MB/s
    Finished in 149.217 s (9% reading + 82% sorting + 10% writing)
Pass #1: merging groups of 16 parts...
    Input parts: 60 with 16777216 values (64 MB) in each
    IO: 629.504 MB/s
    Finished in 12.1199 s
Pass #2: merging groups of 16 parts...
    Input parts: 4 with 268435456 values (1024 MB) in each
    IO: 343.152 MB/s
    Finished in 22.2333 s
Finished in 183.571 s
```
