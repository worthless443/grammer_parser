Part of writing the compiler :  Parser
---

Parsing only limited to expresions like `(a+b)*(a-b)`, doesn't work nested parens, namely `(2*(a+b))`

### To run 
```
./compile.sh
./parser expr.txt
```
Contents of current expr.txt

```

```

## ToDO 

| Goal | Description | status | 
| ---- | ------------| ------ |
| Parser | parse values and expressions, and check for errors | working on |
| class Parser | bind everything in a class Parser | working on | 
| Evaluator | evalute results | working on |
| IR | Construct itermediate Representation | not working on |
