# Blit

Blit (bit-block transfer) operations on planes of bits. This library
provides efficient bit-level manipulation and raster operations for
graphics operations and bitplane processing.

## Overview

The **blit** library is a C implementation of bitwise raster operations
commonly found in graphics systems. It enables efficient copying,
masking, and logical operations on bitmap data at the bit level. The
library handles complex scenarios like non-byte-aligned bit positions
and arbitrary source-destination alignments through phase alignment
techniques.

## Features

-   **Raster Operations (ROP2)**: Support for all 16 binary raster
    operations, including:
    -   Copy, invert, AND, OR, XOR
    -   Erase, paint, and merge operations
    -   Custom logical combinations of source (S) and destination (D)
        bits
-   **One-Dimensional Region Support**: Define regions with origin,
    extent, and source alignment
-   **Phase Alignment**: Automatic handling of arbitrary bit-level
    alignment between source and destination
-   **Comprehensive API**: Both low-level (`blit_rgn1_rop2`) and
    convenience (`blit_rop2`) interfaces
-   **Well-Documented**: Extensive inline documentation with Doxygen
    comments

## Project Structure

```
blit/
├── CMakeLists.txt           # CMake build configuration
├── README.md                # This file
├── LICENSE                  # MIT License
├── inc/blit/                # Public header files
│   ├── rop2.h               # Raster operations enumeration and API
│   ├── rgn1.h               # One-dimensional region structures
│   ├── scan.h               # Scanline buffer definition
│   └── phase_align.h        # Phase alignment utilities
├── src/blit/                # Implementation files
│   ├── rop2.c               # Raster operations implementation
│   └── phase_align.c        # Phase alignment implementation
└── test/                    # Test suite
    ├── pat.c                # Pattern test (checkerboard)
    └── left_shift_edge.c    # Edge case test (bit shifting)
```

## Core Concepts

### Scanline buffer (`blit_scan`)

A scanline buffer represents a bitmap stored as an array of 8-bit
unsigned integers (`blit_scanline_t` type). Key properties:

-   `store`: Pointer to the bitmap data
-   `width`: Width in bits
-   `height`: Height in rows
-   `stride`: Bytes per row

**Example:**

``` c
BLIT_SCAN_DEFINE(image, 800, 600); // 800×600 bit image
```

### One-dimensional region (`blit_rgn1`)

Defines a region along one axis with:

-   `origin`: Starting position in destination
-   `extent`: Size of the region
-   `origin_source`: Starting position in source

Regions are automatically normalised, clipped, and validated by the
library.

### Raster operations (`blit_rop2`)

All 16 binary operations combining source (S) and destination (D):

-   `blit_rop2_0`: Always 0 (blackness)
-   `blit_rop2_1`: Always 1 (whiteness)
-   `blit_rop2_S`: Copy source
-   `blit_rop2_D`: Keep destination
-   `blit_rop2_DSx`: XOR
-   `blit_rop2_DSa`: AND
-   `blit_rop2_DSo`: OR
-   And 9 more (see [inc/blit/rop2.h](inc/blit/rop2.h))

Common operations have aliases:

``` c
blit_rop2_copy = blit_rop2_S
blit_rop2_invert = blit_rop2_Sn
blit_rop2_xor = blit_rop2_DSx
```

### Phase alignment

Handles efficient bit-level alignment between source and destination:

-   Automatically detects alignment mismatch
-   Applies left-shift, right-shift, or no-shift fetching
-   Maintains carry bits between byte boundaries
-   Essential for non-aligned bitmaps

## API Reference

### Low-Level API: `blit_rgn1_rop2`

``` c
bool blit_rgn1_rop2(struct blit_scan *result,
                    struct blit_rgn1 *x,
                    struct blit_rgn1 *y,
                    const struct blit_scan *source,
                    enum blit_rop2 rop2);
```

**Parameters:**

-   `result`: Destination scanline buffer
-   `x`: X-axis region (with clipping boundaries)
-   `y`: Y-axis region (with clipping boundaries)
-   `source`: Source scanline buffer
-   `rop2`: Raster operation code

**Returns:** `true` if successful, `false` if region is invalid

**Use this when:** You need to inspect or manipulate region structures
(e.g., to get clipped extents)

### Convenience API: `blit_rop2`

``` c
bool blit_rop2(struct blit_scan *result,
               int x, int y, int x_extent, int y_extent,
               const struct blit_scan *source,
               int x_source, int y_source,
               enum blit_rop2 rop2);
```

**Parameters:**

-   `result`: Destination scanline buffer
-   `x`, `y`: Destination region origin
-   `x_extent`, `y_extent`: Region dimensions
-   `source`: Source scanline buffer
-   `x_source`, `y_source`: Source region origin
-   `rop2`: Raster operation code

**Returns:** `true` if successful, `false` if the region is invalid

**Use this when:** You have simple integer coordinates and don't need
region inspection

## Usage Examples

### Copy a 32×32 region

``` c
BLIT_SCAN_DEFINE(dest, 256, 256);
BLIT_SCAN_DEFINE(source, 256, 256);

// Copy source (100,50) to dest (50,50), 32×32 pixels.
blit_rop2(&dest, 50, 50, 32, 32, &source, 100, 50, blit_rop2_copy);
```

### Invert a region

``` c
// Invert a 64×64 area at (0,0) in an image.
blit_rop2(&image, 0, 0, 64, 64, &image, 0, 0, blit_rop2_invert);
```

### XOR pattern overlay

``` c
BLIT_SCAN_DEFINE(pattern, 8, 8);
BLIT_SCAN_DEFINE(canvas, 640, 480);

// XOR the pattern at multiple positions.
for (int y = 0; y < 480; y += 8) {
  for (int x = 0; x < 640; x += 8) {
    blit_rop2(&canvas, x, y, 8, 8, &pattern, 0, 0, blit_rop2_xor);
  }
}
```

### Using region structures (advanced)

``` c
struct blit_rgn1 x_rgn = {.origin = 10, .extent = 50, .origin_source = 20};
struct blit_rgn1 y_rgn = {.origin = 10, .extent = 50, .origin_source = 20};

if (blit_rgn1_rop2(&dest, &x_rgn, &y_rgn, &source, blit_rop2_and)) {
  (void)printf("After clipping: extent=%d\n", x_rgn.extent); // may be reduced
}
```

## Building

### Requirements

-   CMake 3.25 or later
-   C99-compatible compiler
-   (Optional) Doxygen for documentation generation

### Build Commands

``` bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

``` bash
cd build
ctest                    # Run all tests
ctest --verbose          # With detailed output
./test_runner test/pat   # Run specific test
```

## Implementation Details

### Raster operation functions

Each of the 16 ROP2 codes maps to a static function implementing the
bitwise operation. Take one example: the DSx (destination XOR source in
reverse Polish) operation:

``` c
// Example: DSx (XOR) operation.
static blit_scanline_t ropDSx(blit_scanline_t fetch, blit_scanline_t store) {
  return fetch ^ store;
}
```

### Bit block transfer algorithm

The core algorithm works roughly as follows:

1.  **Normalise regions** to ensure non-negative extents
2.  **Slip regions** to move origins into valid ranges
3.  **Clip regions** to destination and source bounds
4.  **Set up phase alignment** for source-to-destination bit alignment
5.  **Process scanlines** applying the ROP2 operation with masking at
    edges

### Masking Strategy

-   Origin mask: Protects bits before the region starts
-   Extent mask: Protects bits after the region end
-   Combined mask for single-byte regions

## License

MIT License - See [LICENSE](LICENSE) file

## Copyright

Copyright © 1996, 1998, 1999, 2002, 2025 Roy Ratcliffe

## References

-   Windows ROP2 codes: [Microsoft
    Documentation](https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations)
-   Bit-block transfer: Classic graphics operation used in X11, Windows
    GDI, and Amiga graphics systems

## Testing

The test suite includes:

### `test_pat` checkerboard testing

Tests pattern replication and verification of raster operations using a
checkerboard pattern.

### `test_left_shift_edge` left-shift edge testing

Tests edge case handling for bit shifting across byte boundaries,
ensuring correct phase alignment in all source-destination offset
combinations.

### Running all tests

Run all tests using the following:

``` bash
cd build
ctest --verbose
```

## Contributing

When contributing:

-   Maintain C99 compliance
-   Add tests for new functionality
-   Update documentation with Doxygen comments
-   Follow existing code style (see source files)
