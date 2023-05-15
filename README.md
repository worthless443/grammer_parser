Part of writing the compiler : The Parser
---

This parser is a standalone program to represent nestedness of parenthesis (`()`), and outputs the internal representation of the input. This is still very lackluster, remember that. It almost most of the times never does what is intended.

## Cloning
```bash
git clone --recursive https://github.com/worthless443/grammer_parser
```
or 

```bash
git clone https://github.com/worthless443/grammer_parser
cd grammer_parser
git submodule update --init
```

### Building and running
```bash
make 
```
make it generate a `./parser` and a static `./libgram.a`, running it 
```bash
./parser -i <file> -j <number of threads>
```
Yes, it is multi-threaded, at default it runs at 16 threads if you have >= 16 threads

### Example
```bash
$ cat expr.txt
((((()))))()(())
```
`expr.txt` comes with the repository, feed it to `./parser`
```bash
./parser -i expr.txt
```
output (only for debugging and logging of internal works)
```bash
10 : 6
Normal:success
Recrusive:success

stats:

closure end adds up to 165172
vector.size() end adds up to 1122
started thread 1
started thread 2
started thread 3
started thread 4
started thread 5
started thread 6
started thread 7
started thread 8
started thread 9
started thread 10
started thread 11
started thread 12
action: 99
action: 99
action: 444
action: 2304
action: 2304
action: 3774
action: 5119
action: 5119
action: 5119
action: 5119
action: 5119
action: 5119
times action 39738
EVAL Val == -1
total size of parse : (parens = 17), (values = 1)parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3) 5( 3( 3) 5) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3) 5( 3( 3) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3) 5( 3( 3
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3) 5( 3
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5( 3
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5) 5
parse parens ( 3( 3( 3( 3( 3) 5) 5
parse parens ( 3( 3( 3( 3( 3) 5
parse parens ( 3( 3( 3( 3( 3
parse parens ( 3( 3( 3( 3
parse parens ( 3( 3( 3
parse parens ( 3( 3
parse parens ( 3
parse parens �+
parse values

exiting with error code: 0[00m
```
## The internal representation for true positives 
As for now, the code is very primitive, and therefore there has not been defined a grammar spec for which is standard and curroct. The code interpretation is for now a black box, and the AST evaluation might false out without having set a concrete set of specs. From what it appears, the correct syntax is having an asymmetrical `)` at the end of each pair of parenthesis.

## ToDO 

| Goal | Description | status | 
| ---- | ------------| ------ |
| Parser | parse values and expressions, and check for errors | working on |
| class Parser | bind everything in a class Parser | working on | 
| Evaluator | evalute results | working on |
| IR | Construct itermediate Representation | not working on |


### Table Generation 

The algorithm is TableGen is obviously wrong, it is. So it should fall into the category of FIXME 
