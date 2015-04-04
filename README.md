
# CPP Collections




## Tests

The test suite is composed of tests to check that we stop all errors at the compilation stage, not at runtime.
Files places in the `tests/` directory will be compiled against `collections.h`.

Files with `fail_` at the start of the filename will be expected to fail.

Those without will be expected to build.
Buildable files will also be executed.
They should include assertions to insure the library is executing as desired.


Executable with `make`:
```
make test
```



