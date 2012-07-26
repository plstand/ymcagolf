# YMCA golf tester

## Purpose

This test program and corresponding example solution are for the "[It's fun to stay at the Y.M.C.A.](http://codegolf.stackexchange.com/questions/6695/its-fun-to-stay-at-the-y-m-c-a)" challenge on the Programming Puzzles and Code Golf Stack Exchange site. More details are available there.

## Build instructions

Compile with `make`. Optionally, use `make test` to verify proper functioning.

## Usage

Specify a shell command line that runs your program (redirecting standard output to a PTY) as a single argument:

    ./tester ./example

If that argument is omitted, the tester reads standard input (and does not use a PTY):

    ./example | ./tester

The tester and example program include a newline after each letter. Modify the YMCA() macro in ymca_timing.h to change this.

## License

Copyright © 2012 PleaseStand

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
