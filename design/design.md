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
val myInt = 42
val myInt = -63 
// underscores can be used as digit separators
val bigInt = 1_000_000_000 
```

A base can be specified by prefixing the literal with `0x` (hexadecimal), `0b` (binary) or `0o` (octal).
Leading zeroes are ignored and do not specify an octal literal.

``` 
val hexInt = 0xA9 
// letters are case-insensitive
val hexInt = 0Xa9 
   
val binaryInt = 0b11001001 
val octalInt = 0o257 
val notOctal = 0257 
```


### Floating-point literals

Float literals are represented with decimal values. 
Note that both the whole-number component and fractional component must be specified.

```
val myDouble = 23.985 
    
//val notADouble = 32. 
//val notADouble = .1463 
//val notADouble = 64.abs()
val aDouble = 39.0
val aDouble = 932.0.abs()
```

Float literals support exponential notation.

```
val oneThousand = 1.0e3
val threeHalves = 15.0e-1
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
val myByte = 0b11001001b 
val myByte: Byte = 0b11001001
val myByte = 0b11001001 to Byte 
// Illegal: Byte is not a subclass or superclass of Int
//val myByte = 2b11001001 as Byte 

// Illegal: integer literals can only be assigned to integer types,
// and floating-point literals can only be assigned to float types
//val myInt: Int = 32.0 
//val myDouble: Double = 259 
// Allowed
val myInt = 32.0 to Int
val myDouble = 259 to Double
```

### Other literals

Literals for non-numeric types also exist:

```
val myBool = true
val yourBool = false
val aChar = 'q'

val aString = "Wow! Strings are pretty great!"
val escapeCodeCollection = "Check out my escape code collection: \t\r\n\0\'\"\\"

val someNumbers = [1, 1, 2, 3, 5, 8, 13, 21]
val someNumbers = Array[1, 1, 2, 3, 5, 8, 13, 21]

val myRange = 1..10
val myExclusiveRange = 1 until 10
```

## Operators
Ferrit supports the following operators:
- Basic operators: `+ - * / %` 
- Logical operators: `! && ||`
- Comparison operators: `== != > < >= <=`
- Assignment operators: `= += -= *= /= %= &&= ||=`
- Type operators: `is` `!is` `as` `as?` `to` `to?`
- Contains operator: `in` `!in`
- Range operator: `..`
 
### Examples:
```
var sum = 5.0 + 10.0 
sum /= 3.0
sum *= 2.6              

val bitPattern: Byte = (0b10101101 & 0b10) << 2 

val isEven = bitPattern % 2 == 0
val isOdd = !isEven
val condition = isOdd || !isEven

// Attempt to dereference a null value, 
// but short circuiting prevents the right side from being executed
val ohNo = false && null!!

val answer = 42

val isInteger = answer is Int // true
val asDouble = answer as Double // panics
val asDouble = answer as? Double // None
val toDouble = answer to? Double // Some(42.0)

val toBoolean = answer to Bool // panics
val toBoolean = answer to? Bool // None

val containsA = 'A' in "Hello, world!" // = false
```


## Variables
Ferrit allows you to declare variables with the `var` and `val` keywords. 
`var` defines a read-write variable, and `val` defines a read-only variable.

The type is specified with a colon after the variable's name, followed by the type's name. 
If the variable is given an initializer, then the type can be deduced automatically and is not necessary. 

Note: variables must be given a value before they are used.

```
// Illegal: read-only variables can not be changed
//val myVal: Int 
//myVal = 10

// Legal
val myVal: Int = 10
val myVal = 10
```

Variables are immutable by default.
To specify that a variable is mutable, declare it with `var` instead.

```
// Defines a mutable variable
var counter = 1
counter += 1
counter += 10
```

Assignments can be used as expressions in Ferrit.

```
if ((val value: Int? = ..) is Int) { 
    // Smart casting and pattern matching is intended to be supported in the future,
    // but is out of the scope for Ferrit 1.0 
    print(value!!)
}
```


## Control Flow

Ferrit supports standard control flow constructs such as `if`/`else` statements and `for` and `while` loops.
The language also supports pattern matching with `when`.

### If/else statements:

These statements work much as they do in other C-like languages. 
However, they have the added benefit of being usable as expressions.

```
val answer: Int = ...
val guess: Int = ...
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
val message = if (guess > answer) { 
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
val nums = [1, 2, 3, 4, 5]
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
val _iter = nums to Iterator<Int>
while (_iter.hasNext()) {
    val i = _iter.next()
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
var input = Input()
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

## Functions
Functions are declared by specifying the `fun` keyword and then the function's name, parameters, and return type.
They are then called by writing the function's name and arguments.

```
fun pythagoras(x: Double, y: Double) -> Double {
    return sqrt(x * x + y * y)
}

val c = pythagoras(3.0, 4.0)
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

### Native Support

Functions may be declared `native` to provide a C/C++ implementation. 
This feature is not finalized and is likely to change.

```
// in module MyCode

native fun doSomeStuff(a: Int, b: Int)
// C function is:
// ferrit_Unit MyCode_doSomeStuff(ferrit_Int a, ferrit_Int b)

class MyClass {
    native mut fun doMoreStuff(foo: Double?, bar: Int) -> Result<String, String>
}
// C function is:
// ferrit_Result MyCode_MyClass_doMoreStuff(struct MyCode_MyClass *self, ferrit_Optional foo, ferrit_Optional bar)
```

### First-Class Functions

Functions in Ferrit are first-class, meaning they are distinct objects in and of themselves.

```
// both sum and doSum are of type (Int, Int) -> Int
fun sum(a: Int, b: Int) = a + b
val doSum = sum

println(doSum(3, 4)) 
```

Anonymous functions are also supported:

```
fun doStuff(myNums: mut Double[], block: Double.() -> Double) {
    for i in myNums.indices() {
        myNums[i] = num.block()
    }
}

var myNums = [1.0, 3.1415926535, 2.714, 6.022e23]
doStuff(myNums) { n -> n * 4 }
println(myNums)
```

## Modules

Modules allow logical units of code to be grouped together.
They are declared at the top of a source file with the `module` keywords.
Modules can be further grouped into submodules by separating its components with periods.

```
module my.mod.name
```

Module names are correlated with file system paths. For instance, if the above line was written in
`MyCode.fe`, then it should be saved in `"<source_root>/my/mod/name/MyCode.fe"`.

Items in other modules can be referred to by their fully-qualified names. 

```
val f = ferrit.io.File(ferrit.io.getCurrentDirectory())
```

You can remove the need to use the full name with a `using` statement.
You can import individual declarations, multiple declarations at once (with `[]`), or all declarations in a scope (with `*`).

```
using ferrit.io.File
using ferrit.io.getCurrentDirectory
//using ferrit.io.[File, getCurrentDirectory]
//using ferrit.io.*


val f = File(getCurrentDirectory)
```

You don't need to import a fully-qualified path to a declaration. The general rule of thumb is that 
when you use a `using` statement, you can omit everything before the last period.

```
using ferrit.io

val f = io.File(io.getCurrentDirectory())
```

Imports can be aliased to a new name with `as`.

```
using ferrit.io as fio

val io = fio.File(fio.getCurrentDirectory())
```

### Access Modifiers

Access modifiers can be used to restrict declaration visibility. Ferrit has four access modifiers:

* `public` - (default) Visible everywhere
* `module` - Visible everywhere within the same module
* `protected` - (class members only) Visible to the current class and its subclasses
* `private` - Visible only to the current module/class

## Classes

Ferrit supports object-oriented programming. Ferrit 1.0 will support single inheritance, traits, (pure) virtual functions, 
static members and multiple access modifiers.

### Basic Usage
A class can be declared with the `class` keyword.

The following definition specifies a complete class:

```
class MyClass 
```

In this example, `MyClass` extends from `ferrit.Any` and has an automatically-generated default constructor.

Since `ferrit.Any` does not define any fields or methods, and `MyClass` does not declare any of its own,
`MyClass` is completely useless.

```
val instance = MyClass()
// Now what?
```

A more useful class would have fields and methods:

```
class IntBox {
    private var value: Int 
    
    init() : init(0)
    
    init(value: Int) {
        this.value = value
    }
    
    fun getValue() = this.value
    fun setValue(value: Int) = this.value = value
}

val box = IntBox()
println(box.getValue()) // 0
println(box.setValue(10)) // 10
```

This defines a simple data class that wraps an `Int`.

### Polymorphism

To make a class polymorphic, declare it as well as all overridable methods with the `open` modifier.

```
open class Base {
    public val alpha: Int
    module val beta: Int 
    protected val gamma: Int
    private val delta: Int
    
    init(alpha: Int, beta: Int, gamma, Int, delta: Int) {
        println("Base init")
        this.alpha = alpha
        this.beta = beta
        this.gamma = gamma
        this.delta = delta
    }
    
    fun foo() { 
        println("Base foo") 
    }
    
    open fun bar() {
        println("Base bar")
    }
}
```

To extend a class, write `:` and then the superclass' name after the derived class' name.

Overridden members must be declared with `override` (which implies `open`).

```
// Since Derived is not explicitly 'open', it is 'closed' (cannot be extended) 
class Derived : Base { 
    // must explicitly call superclass constructor 
    // when initializing a derived class
    init() : super(10, 20, 30, 40) {
        print("Derived init")
    }
    
    // illegal - foo is 'closed' in Base
    //override fun foo() { ... }
    
    override fun bar() { 
        println("Derived bar")
    }
    
    fun getSum() -> Int = {
        var sum = 0
       
        // ok 
        sum += alpha
        sum += beta
        sum += gamma
        // illegal - delta is private in Base
        //sum += delta
        
        sum 
    }
}

val myBase = Base(1, 2, 3, 4)
myBase.foo() 
myBase.bar()
println()

val myDerived = Derived()
derived.foo()
derived.bar()

var sum = derived.getSum()
// ok
sum += derived.alpha 
sum += derived.beta
// illegal
//sum += derived.gamma
//sum += derived.delta

/*
 * Output:
 * Base init
 * Base foo
 * Base bar
 * 
 * Base init
 * Derived init
 * Base foo
 * Base bar
 * Derived bar
 */
```

### Abstract Classes and Traits

You can use the `abstract` keyword to declare classes and methods that do not have an implementation.

```
abstract class Shape {
    private val x: Double
    private val y: Double
    
    init(x: Double, y: Double) {
        this.x = x
        this.y = y
    }
    
    fun getX() -> Double = x
    fun getY() -> Double = y
    
    abstract fun getPerimeter() -> Double
    abstract fun getArea() -> Double
}
```

This can be implemented like normal:
```
class Rectangle : Shape {
    private val length: Double
    private val width: Double
    
    init(x: Double, y: Double, l: Double, w: Double) : super(x, y) {
        length = l
        width = w
    }
    
    fun getLength() = length 
    fun getWidth() = width

    override fun getPerimeter() = 2.0 * (length + width)
    override fun getArea() = length * width
}


// Illegal - Shape is abstract
//val shape = Shape(3.0, 4.0)
//println("perimeter = " + shape.getPerimeter() to String)

val shape: Shape = Rectangle(3.0, 4.0, 10.3, 3.7)
println("perimeter = " + shape.getPerimeter() to String)
```

Classes can only have one superclass, however, so it might be preferable to use a trait instead.
Traits define abstract members without providing an implementation. A class can implement any number of traits,
but can only extend one class.

Here's what the previous `Shape` class would look like as a trait:

```
trait Shape {
    fun getX() -> Double 
    fun getY() -> Double
    
    fun getPerimeter() -> Double
    fun getArea() -> Double
}
```


### Example - Point
Here's an example of a class that implements the traits `Display`, `Equals` and `Hash`.
```
using ferrit.math.[sqrt, pow]

class Point : Display, Equals, Hash {
    private var x: Int 
    private var y: Int
    
    static val Origin = Point(0, 0)
    
    init(x: Int, y: Int) {
        this.x = x
        this.y = y
    }
    
    fun getDistanceFromOrigin() = {
        sqrt(pow(x, 2) + pow(y, 2))  
    }
    
    fun getX() = this.x 
    fun setX(x: Int) = this.x = x
    
    fun getY() = this.y 
    fun setY() = this.y = y
    
    override fun operator to() = {
        "(" + x to String + ", " + y to String + ")"
    }
    
    override fun operator equals(other: Any) = {
        if (other !is Point) {
            false
        } else {
            val other = other as Point
            x == other.x && y == other.y
        }
    }
    
    override fun getHash() = x ^ 31 + y
}
```

This class creates a simple data holder with two fields, appropriate getter and setter methods, 
two constructors to initialize the fields, a computed property, and a basic implementation of the three traits.
In version 1.0 of the language, this boilerplate will be required.

In the future, additional features may be added to reduce this boilerplate.
A future example might look like this:

```
using ferrit.math.[sqrt, pow]

class Point : default Into<String>, default Equals<Point>, default Hash {
    static val Origin = Point(0, 0)
    
    primary init(var x: Int, var y: Int)
    
    val distanceFromOrigin: Double {  
        get() = sqrt(pow(x, 2) + pow(y, 2)) 
    }
}
```

Or perhaps:

```
using ferrit.math.[sqrt, pow]

data class Point(var x: Int, var y: Int) {
    companion object {
        val Origin = Point(0, 0)
    }
    
    val distanceFromOrigin: Double   
        get() = sqrt(pow(x, 2) + pow(y, 2)) 
}
```

