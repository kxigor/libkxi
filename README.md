# libkxi

A header-only C++23 library for compile-time type-list manipulation and a
tuple family built on top of it. The tuples reorder their elements internally
according to a user-supplied predicate while preserving the original
construction and access order, so call sites stay unchanged.

## Modules

### `kxi::meta`

Operations over heterogeneous type lists. A *type list* is any class template
of the form `Shell<Args...>`.

- `Size<L>` / `SizeV<L>` — number of elements.
- `TypeAt<I, L>` / `TypeAtT<I, L>` — element type at index `I`.
- `Count<T, L>` / `CountV<T, L>` — number of occurrences of `T`.
- `IndexOf<T, L>` / `IndexOfV<T, L>` — position of `T` (requires the list to
  hold each type at most once).
- `IsDistinct<L>` / `concepts::Distinct` — all elements unique.
- `LocateIndex<I, Ls...>` — flat-position translation across several lists.
- `CatLists<Ls...>` — list concatenation.
- `Rebind<L, Shell>` — re-host the same elements in a different shell.
- `ShellOf<L>` — type-level handle to the source shell.
- `IsSameShell` / `IsAllSameShells` / `CommonShellOf` — shell comparison.
- `Sort<Predicate, L>` — stable compile-time sort.

`kxi::meta::pack` provides the same operations on bare parameter packs through
`PackHolder<Args...>`.

### `kxi::utility`

Small type-level building blocks.

- `CopyConst<From, To>` / `CopyCV<From, To>` — qualifier propagation.
- `IsIndexSequence<T>` — concept and trait.
- `concepts::PerfectCtorGuard<Self, Args...>` — protects a perfect-forwarding
  constructor from shadowing the copy and move constructors.
- `IndexedType<I, T>` — single-element holder used as a base class brick for
  tuples; provides EBO-friendly storage via `[[no_unique_address]]`.

### `kxi::tuple::flat`

`flat::Tuple<Args...>` is the ground-truth tuple. Element order is the
declaration order; elements are stored as distinct `IndexedType<I, T>` base
subobjects.

```cpp
#include <libkxi/tuple/flat.hpp>

kxi::tuple::flat::Tuple<int, double, std::string> t(42, 3.14, "hi");
int        x = t.Get<0>();        // by index
double     y = t.Get<double>();   // by type (unique types only)
```

### `kxi::tuple::sorted`

`sorted::Of<Predicate>::Tuple<Args...>` reorders its storage by a binary
predicate, but `Get<I>` and `Get<T>` see the user-supplied order. The predicate
is a class template with a `value` member (`Predicate<A, B>::value`).

```cpp
#include <libkxi/tuple/sorted.hpp>

template <typename A, typename B>
struct SizeofLess {
  static constexpr bool value = sizeof(A) < sizeof(B);
};

using namespace kxi::tuple::sorted;

Of<SizeofLess>::Tuple<double, char, int> t(3.14, 'x', 42);
//                                         ^^^^^ stored as (char, int, double)
double a = t.Get<0>();   // 3.14   — original index
char   b = t.Get<1>();   // 'x'
int    c = t.Get<2>();   // 42
```

Free factories carry the predicate explicitly:

```cpp
auto t = sorted::Make<SizeofLess>(1, 2.0, 'x');
auto u = sorted::Tie<SizeofLess>(a, b);
```

### `kxi::tuple::compact`

`compact::Tuple<Args...>` is `sorted::Of<P>::Tuple<Args...>` where `P` is the
built-in alignment-descending predicate. Same observable order as `flat::Tuple`
for `Get<I>`, but the in-memory layout is reordered to minimise padding.

### `kxi::tuple::like`

Generic free-function API that works with any type satisfying the
`like::concepts::TupleLike` shape (has `meta::SizeV` and member `Get<I>`).

- `Get<I>(t)` / `Get<T>(t)` — uniform getter with full value-category and cv
  forwarding.
- `Swap(a, b)` — free-function swap.
- `Cat(t1, t2, ...)` — concatenate tuples of the same shell.
- `SpanGet<I>(t1, t2, ...)` — index across a span of tuples as if they were one
  flat list.
- `Make`, `Tie`, `Forward`, `ForwardInto` — factories parameterised by the
  target shell.

`flat::`, `sorted::` and `compact::` re-export these names so call sites read
naturally inside each namespace.

## Requirements

- CMake 3.25 or newer.
- A C++23-capable compiler. Tested with recent GCC and Clang.
- Ninja (recommended; the presets declare it).
- clang-format and clang-tidy for the format and tidy checks (optional but
  enabled by default in the dev presets).
- gcov, lcov, genhtml for HTML coverage reports (only needed for the coverage
  preset).

## Building

The project ships `CMakePresets.json` with several configurations.

```
cmake --list-presets
```

A typical development cycle uses the AddressSanitizer preset:

```
cmake --preset dev-debug-asan
cmake --build --preset dev-debug-asan
ctest --preset dev-debug-asan
```

Available presets:

| Preset             | Build type | Notes                                   |
|--------------------|------------|-----------------------------------------|
| `dev-debug-asan`   | Debug      | AddressSanitizer                        |
| `dev-debug-tsan`   | Debug      | ThreadSanitizer                         |
| `dev-debug-msan`   | Debug      | MemorySanitizer (requires libc++)       |
| `dev-debug-ubsan`  | Debug      | UndefinedBehaviorSanitizer              |
| `ci-release`       | Release    | `-O3 -DNDEBUG`, no sanitizers           |
| `ci-coverage`      | Debug      | `--coverage` flags for gcov/lcov        |

## Testing

Tests are GoogleTest-based and registered through the `add_gtest` helper in
`cmake/GtestTools.cmake`.

```
ctest --preset dev-debug-asan
ctest --preset dev-debug-asan -R FlatTuple
```

Two extra CTest entries are registered as quality gates:

- `FormatCheck` — runs `clang-format` in check mode.
- `TidyCheck` — runs `clang-tidy` across the public headers.

Both are skipped if the corresponding tool is missing at configure time.

### Coverage

```
cmake --preset ci-coverage
cmake --build --preset ci-coverage --target coverage
```

The coverage target zeroes counters, runs the tests, captures `gcda` data via
LCOV, filters system headers, and emits an HTML report at
`build/ci-coverage/coverage_report/index.html`.

## Project layout

```
include/libkxi/
  meta/         compile-time type-list operations
  utility/      small type-level building blocks
  tuple/
    flat/       declaration-order tuple
    sorted/     tuple reordered by a predicate
    compact/    alignment-descending alias of sorted
    like/       generic free-function API
tests/unit/     gtest unit tests (one target per file)
app/            example executable
cmake/          build, sanitizer, coverage and tooling modules
```

## Adding a test

```cmake
add_gtest(MyTest
  SOURCES my_test.cpp
  LIBRARIES
    project_coverage
    libkxi
)
```

Optional arguments are documented in the header of
`cmake/GtestTools.cmake` (sanitizer linkage, environment variables, fixtures,
serial execution, custom defines, etc.).

## License

MIT. See `LICENSE`.
