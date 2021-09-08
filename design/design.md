# Ferrit Design Document

This is an overview of what is planned to be added to the Ferrit language.
This document is not final and anything inside it may change at any point.


## Basic Types

Ferrit has the following primitive types:
 * 8-bit integers: `SByte` and `Byte`
 * 16-bit integers: `Short` and `UShort` 
 * 32-bit integers: `Int` and `UInt`
 * 64-bit integers: `Long` and `ULong`
 * 32-bit floats: `Float` 
 * 64-bit floats: `Double` 
 * booleans: `Bool`
 * characters: `Char`. Note that `Char` is only 8 bits in size and does not necessarily represent a complete unicode character.

Each of these types can be created with literals. 


### Integer Literals
Integer literals are represented with whole number values.

```
let myInt = 42
let myInt = -63 
// underscores can be used as digit separators
let bigInt = 1_000_000_000 
```

A base can be specified by prefixing the literal with `0x` (hexadecimal), `0b` (binary) or `0o` (octal).
Leading zeroes are ignored and do not specify an octal literal.

``` 
let hexInt = 0xA9 
// letters are case-insensitive
let hexInt = 0Xa9 
   
let binaryInt = 0b11001001 
let octalInt = 0o257 
let notOctal = 0257 
```


### Floating-point literals

Float literals are represented with decimal values. 
Note that both the whole-number component and fractional component must be specified.

```
let myDouble = 23.985 
    
//let notADouble = 32. 
//let notADouble = .1463 
//let notADouble = 64.abs()
let aDouble = 39.0
let aDouble = 932.0.abs()
```

Float literals support exponential notation.

```
let oneThousand = 1.0e3
let threeHalves = 15.0e-1
```

### Specifying numeric literal types

Integer and float literals default to the types `Int` and `Double` respectively. To specify a different type, add a suffix to the literal.

| Type     | Suffix          |
|:--------:|:---------------:|
| `SByte`  | `42sb`          | 
| `Byte`   | `42b`           | 
| `Short`  | `42s`           | 
| `UShort` | `42us`          |
| `Int`    | `42`, `42i`     | 
| `UInt`   | `42u`, `42ui`   |
| `Long`   | `42L`           |
| `ULong`  | `42uL`          |
| `Float`  | `42.0f`         |
| `Double` | `42.0`, `42.0d` |

Alternatively, you may explicitly specify the literal's type or  use the `to` operator.

```
let myByte = 0b11001001b 
let myByte: Byte = 0b11001001
let myByte = 0b11001001 to Byte 
// Illegal: Byte is not a subclass or superclass of Int
//let myByte = 2b11001001 as Byte 

// Illegal: integer literals can only be assigned to integer types,
// and floating-point literals can only be assigned to float types
//let myInt: Int = 32.0 
//let myDouble: Double = 259 
// Allowed
let myInt = 32.0 to Int
let myDouble = 259 to Double
```

### Other literals

Literals for non-numeric types also exist:

```
let myBool = true
let yourBool = false
let aChar = 'q'

let aString = "Wow! Strings are pretty great!"
let escapeCodeCollection = "Check out my escape code collection: \t\r\n\0\'\"\\"

// Literal array syntax just calls Array.init[]
let someNumbers = [1, 1, 2, 3, 5, 8, 13, 21]
let someNumbers = Array[1, 1, 2, 3, 5, 8, 13, 21]

let myRange = 1..10
let myExclusiveRange = 1 until 10
```

## Operators
Ferrit supports the following operators:
- Basic operators: `+ - * / %` 
- Logical operators: `! && ||`
- Comparison operators: `== != > < >= <=`
- Bitwise operators: `& | ^ ~ << >> >>>`
- Assignment operators: `= += -= *= /= %= &&= ||= &= |= ^= <<= >>= >>>=`
- Type operators: `is` `!is` `as` `as?` `to` `to?`
- Contains operator: `in` `!in`
- Range operator: `..`
 
### Examples:
```
let mut sum = 5.0 + 10.0 
sum /= 3.0
sum *= 2.6              

let bitPattern: Byte = (0b10101101 & 0b10) << 2 

let isEven = bitPattern % 2 == 0
let isOdd = !isEven
let condition = isOdd || !isEven

// Attempt to dereference a null value, 
// but short circuiting prevents the right side from being executed
let ohNo = false && null!!

let answer = 42

let isInteger = answer is Int // true
let asDouble = answer as Double // panics
let asDouble = answer as? Double // None
let toDouble = answer to? Double // Some(42.0)

let toBoolean = answer to Bool // panics
let toBoolean = answer to? Bool // None

let containsA = 'A' in "Hello, world!" // = false
```


## Variables
Ferrit allows you to declare variables with the `let` keyword.
All variables must be given a value before they are used.

The type is specified with a colon after the variable's name, followed by the type's name. 
If the variable is given an initializer, then the type can be deduced automatically and is not necessary. 

```
// Illegal: read-only variables can not be changed
//let myVal: Int 
//myVal = 10

// Legal
let myVal: Int = 10
let myVal = 10
```

Variables are immutable by default.
To specify that a variable is mutable, declare it with `let mut` instead.

```
// Defines a mutable variable
let mut counter = 1
counter += 1
counter += 10
```

Assignments can be used as expressions in Ferrit. This can be combined with pattern matching like so:

```
let someUnknownType = ...
if (let Some(value) = someUnknownType to? Int) { 
    // I can use  
}
```


## Control Flow

Ferrit supports standard control flow constructs such as `if`/`else` statements and `for` and `while` loops.
The language also supports pattern matching with `when`.

### If/else statements:

These statements work much as they do in other C-like languages. 
However, they have the added benefit of being usable as expressions.

```
let answer: Int = ...
let guess: Int = ...
if (guess > answer) {
    println("Too high!")
} else if (guess < answer) {
    println("Too low!")
} else {
    println("You got it!")
}

// Braces are only required if more than one statement is in the block
if (guess == 69) println("nice")

// Used as an expression:
let message = if (guess > answer) { 
        "Too high!" 
    } else if (guess < answer) { 
        "Too low!" 
    } else { 
        "You got it!"
    }
println(message)
```

### Loops: 
Ferrit supports 3 kinds of loops: `for`, `while` and `do while`.

For loops iterate through every element in an iterator.
Note that Ferrit does not support traditional C-style for loops.

```
// iterate elements
let nums = [1, 2, 3, 4, 5]
for (num in nums) {
    println(num)
}

// iterate indices
for (index in 0 until nums.size) {
    println(nums[index])
}
```

While-loops run until the condition is false.

```
// For-loops are just syntactic sugar for while-loops
let mut _iter = nums to Iterator<Int>
while (_iter.hasNext()) {
    let i = _iter.next()
    println(i)
}

// infinite loop
while (true) {
    println("Loops are pretty great")
}
```

Do-while loops act much the same as while loops,
except that the condition is checked at the end of the loop body instead of before.

```
fun getInput() -> Input
let mut input = Input()
do {
    input = getInput()
} while (!input.isValid)
```

The `break` and `continue` keywords can be used to exit or continue a loop respectively.

```
for (x in 0 until 16) {
    if (x % 2 == 0) continue
    if (x == 8) break
}
```

### Pattern Matching
TODO
```
let x: Int = ... 
let message = when (x) {
    0 -> "X is 0"
    in 1..100 -> "X is small"
    n if n > 100 -> "X is big"
    n if n == n.abs() -> "x == |x|"
    else -> "unknown"
}

let y: Option<Int>
let message = when (y) {
    is Some(n) -> print(n)
}
```

## Functions
Functions are declared by specifying the `fun` keyword and then the function's name, parameters, and return type.
They are then called by writing the function's name and arguments.

```
fun pythagoras(x: Double, y: Double) -> Double {
    return sqrt(x * x + y * y)
}

let c = pythagoras(3.0, 4.0)
```

Functions can return a value via the `return` keyword. If no value is specified, 
or no `return` statement is used, then the function returns `Unit`.

```
fun returnsUnit() {}
fun returnsTen() -> Int { return 10 }
```

If a function is defined with an equals sign after its return type, then its body may be an expression.
The value returned is the last value in the expression. 

```
// Return type ommitted, deduced to Int
fun returnsTen() = 10
// This syntax works with blocks too
fun doSomeMath(n: Int) = {
    if (n % 2 == 0) {
        n / 2
    } else {
        3 * n + 1 
    } 
}
```

Functions may specify default values with an equals sign after the parameter.

```
fun pow(number: Int, exponent: Int = 2) -> Int { ... }
pow(4) // exponent is implicitly 2
pow(4, 3) // exponent is explicitly 3
```

Arguments may be named when you call a function.

```
fun huh(a: Int = 4, b: Int = 93, c: Int = -1) { ... }
huh(a: 1, b: 2, c: 3)
huh(b: 49) 

// This assigns 24 to a local variable named 'a', then passes it in as a parameter to huh
huh(a = 24)
```

If the last argument to a function is a lambda, it may be written outside the argument list.

```
fun doStuff(foo: Int = 39, bar: () -> Unit) { ... }

doStuff(20, { println("i did it") }) 
doStuff(30) { println("i did it") }

doStuff(bar = { println("i did it") })
// parenthesis can be ommitted if no other arguments are passed
doStuff { println("i did it") }
```

You can pass a variable number of arguments by adding `...` to the argument's type.

```
fun myPrint(params: Into<String>...) { 
    // params is actually an Array<Into<String>>
    for (p in params) {
        print(p to String + " ")
    } 
    println()
}
myPrint("arg1", "arg2", "arg3")
myPrint(params = ["this is a single parameter", "not two!"])
myPrint(params = *["this counts as multiple parameters", "thanks to the spread operator *"])
```

### First-Class Functions
Functions in Ferrit are first-class, meaning they are distinct objects in and of themselves.

```
let doSum = 
```