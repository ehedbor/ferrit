# Ferrit Design Document

This is an overview of what is planned to be added to the Ferrit language.
This document is not final and anything inside it may change at any point.


## Overview

Ferrit is an object-oriented, statically-typed, garbage-collected, compiled programming language.
Its syntax is based off C-like languages such as Java and Kotlin.

Programs consist of declarations, statements and expressions organized in an `.fe` file.
There is no restriction on what may appear at top-level.

Here is a "Hello World" program written in Ferrit:

```
println("Hello")
```

For a more complicated example, this program computes the Fibonacci sequence up to 10:

```
fun fib(n: Int) -> Int = {
    if (n <= 0) {
        0
    } else if (n == 1 || n == 2) {
        1
    } else {
        fib(n - 1) + fib(n - 2)
    }
}

for (n in 1...10) {
    println("Fib(" + n + ") = " + fib(n)) 
}
```

Ferrit allows you to organize code across multiple files. Simply specify all source files as parameters
to `ferritc`.

## Basic Types

Ferrit has the following basic types:
 * 8-bit integers: `SByte` and `Byte`
 * 16-bit integers: `Short` and `UShort` 
 * 32-bit integers: `Int` and `UInt`
 * 64-bit integers: `Long` and `ULong`
 * 32-bit floats: `Float` 
 * 64-bit floats: `Double` 
 * booleans: `Bool`
 * characters: `Char`. Note that `Char` is only 8 bits in size and does not necessarily represent a complete unicode character.
 * strings: `String`
 * arrays: `Array<T>`

### Integers
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

### Converting between literals

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

Implicit widening conversions are allowed, but implicit narrowing conversions are permitted.
Conversions between integers/floats must also be made explicit, as conversion between them is
inherently lossy.

```
val myByte: Byte = 10 // illegal, narrowing conversion
val myLong: Long = 10L // allowed, widening conversion 

val myInt: Int = 10.0 // illegal
val myDouble: Double = 10 // illegal
```

### Other literals

Literals for non-numeric types also exist:

```
val myBool = true
val yourBool = false
val aChar = 'q'

val aString = "Wow! Strings are pretty great!"
val escapeCodeCollection = "Check out my escape code collection: \t\r\n\0\'\"\\"

// implcitly calls Array's collection constructor, init[T...]
val someNumbers = [1, 1, 2, 3, 5, 8, 13, 21]
// explicit version of the above
val someNumbers: Array<Int> = Array[1, 1, 2, 3, 5, 8, 13, 21]

val myRange = 1..10
val myExclusiveRange = 1 until 10
```

## Operators
Ferrit supports the following operators:
- Basic operators: `+ - * / %` 
- Logical operators: `! && ||`
- Bitwise operators: `~ & | << >>`
- Comparison operators: `== != > < >= <=`
- Assignment operators: `= += -= *= /= %= &&= ||= &= |= <<= >>=`
- Type operators: `is` `!is` `as` `as?` `to` `to?`
- Contains operator: `in` `!in`
- Range operator: exclusive (`..`) and inclusive (`...`)
 
### Examples:
```
var sum = 5.0 + 10.0 
sum /= 3.0
sum *= 2.6              

val bitPattern: Byte = (0b10101101 & 0b10) << 2 

val isEven = bitPattern % 2 == 0
val isOdd = !isEven
val condition = isOdd || !isEven

val hmmmm = false && true

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
While variables do not need to be initialized immediately, they must be initialized before they are used.

```
// Illegal: read-only variables can not be changed
//val myVal = 10 
//myVal += 4

// Legal: myVar is read-write
var myVar = 10
myVar += 4

// Legal: name shadowing is permitted
val myVal = 10
val myVal = myVal + 4
```

## Control Flow

Ferrit supports standard control flow constructs such as `if`/`else` statements and `for` and `while` loops.

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
for (index in 0..nums.size) {
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
var num: Int? = null
do {
    val input: String = getInput()
    num = input to? Int
} while (num == null)

println
```

The `break` and `continue` keywords can be used to exit or continue a loop respectively.

```
for (n in 0...10) {
    if (n % 2 == 0) continue
    if (n == 8) break
}
```

Labels allow you to break and continue across nested loops:
```
val chunk: Array<Byte> = getChunk()
val DIAMOND_ORE = 56b
for (x in 0..16) @outer:{
    for (z in 0..16) {
        for (y in 0..256) {
            val block = chunk[(x * 16 + z) * 16 + y]
            if (block == DIAMOND_ORE) {
                println("OMG DIAMONDS!!!")
                break@outer
            }
        }
    } 
}
```

## Functions
Functions are declared with the `fun` keyword. They can be called by writing their name and arguments.

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

If a function is defined with an equals sign after its return type, then its body is an expression 
and the return type can be inferred.

```
// Return type ommitted, deduced to Int
fun returnsTen() = 10

// This syntax works with blocks too
// Note that this does NOT return a closure!
fun doSomeMath(n: Int) = {
    println("This is a block!")
    if (n % 2 == 0) {
        n / 2
    } else {
        3 * n + 1 
    } 
}
```

Functions may specify default values for their parameters. The parameter's type is still required.

```
fun pow(number: Int, exponent: Int = 2) -> Int { ... }
pow(4) // exponent is implicitly 2
pow(4, 3) // exponent is explicitly 3
```

Arguments may be named when you call a function. No positional arguments may come after a named argument,
and named arguments need not be in order.

```
fun huh(a: Int = 4, b: Int = 93, c: Int = -1) { ... }
huh(c = 30, a = 10, b = 57)
huh(b = 49) 
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

### First-Class functions

Functions in Ferrit are first-class, meaning they are distinct objects that can be passed around.

```
fun sum(a: Int, b: Int) = a + b

val doSum: (Int, Int) -> Int = sum

println(doSum(3, 4)) 
```

Ferrit provides a concise syntax to create anonymous functions. 

```

```

If the last argument to a function is a lambda, it may be written outside the argument list.
In addition, if there are no other arguments to the function, the parenthesis may be omitted.

```
fun fooBar(foo: Int = 39, bar: (Int) -> Unit) { 
    bar(foo)
}
 
fooBar(20, { f -> println("Foo is " + f) }) 

// Shortened syntax: 
fooBar(30) { f -> println("Foo is " + f) }

// Even shorter:
fooBar { println("i did it") }
```

### Native Support

Functions may be declared `native` to provide a C/C++ implementation. 
You can pass a flag to the compiler to generate header files from Ferrit source files. 
This feature is not finalized and is likely to change.

```
module mycode

native fun doSomeStuff(a: Int, b: Int)

class MyClass {
    native fun doMoreStuff(foo: String, bar: Int) -> Double
}

// This would result in functions called "mycode_doSomeStuff" and "mycode_MyClass_doMoreStuff".
```

## Modules

Modules allow logical units of code to be grouped together.
They are declared at the top of a source file with the `module` keywords.
Modules can be further grouped into submodules by separating its components with periods.

```
module my.mod.name
```

Module names are correlated with file system paths. For instance, if the above line was written in
`MyCode.fe`, then it should be saved in "my/mod/name/MyCode.fe".

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

### Properties, Fields and Constructors

A more useful class would have state and behavior:

```
class Address : Into<String> {
    private var street: String
    private var city: String
    private var state: String?
    private var zip: String
    private var country: String
    
    init(street: String, city: String, state: String?, zip: String, country: String) {
        this.street = street
        this.city = city
        this.state = state
        this.zip = zip
        this.country = country
    }
    
    override operator fun into() -> String {
        return street + "\n" +
            city + (if (state == null) "" else ", " + state + " ") + 
            zip + "\n" +
            country + "\n"
    }
    
    fun getStreet() = this.street
    fun setStreet(street: String) { this.street = street } 
    ... more getters/setters ...
}
```

That's a lot of boilerplate. Thankfully, we can leverage properties and primary constructors to reduce it.

```
// This code is semantically equvalent!
class IntBox : Into<String> {
    primary init(
        var street: String,
        var city: String,
        var state: String?,
        var zip: String,
        var country: String,
    )
    
    override operator fun into() -> String {
        return street + "\n" +
            city + (if (state == null) "" else ", " + state + " ") + 
            zip + "\n" +
            country + "\n"
    }    
}
```

I'm getting tired of writing stuff so here's how you do custom properties:

```
class AHRRGRGGGDSFDFB {
    var customBehavior: Int = 10 private get
    var something: Double 
        module get() = random.next() 
        protected set(value) { panic("lol") }
}
```

### Collection Constructors

Collection constructors are a neat way to make your own collection literals. Here's how you use them:
```
class CharList {
    private var data: String
    
    init[chars: Char...] {
        data = String.fromChars(chars)
    }
    ...
}

val firstList = CharList['T', 'e', 's', 't', '.', '\0']
val secondList: CharList = ['N', 'e', 'a', 't', '\0']
```

If a collection constructor takes a `Pair<T>...`, then you may use a special syntax to specify the pairs.
```
class Map {
    primary init[val data: Pair<String, String>...] 
}
val cookieRatings = Map["Peanut Butter": 10, "Chocolate chip": 8, "Oatmeal Raisin": 7] 
```

### Polymorphism

To make a class polymorphic, declare it and all overridable properties/methods with the `open` modifier.

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

## Generics
We got 'em.

```
class List<T> {
    private val data = Array<T>()
    var size = 0 private set
    var capacity = 0 private set
    
    init[private val data: T...] {
        size = data.size
        capacity = size
    }
    
    operator fun get(i: Int) = data[i]
    operator fun set(i: Int, value: in T) { data[i] = value }  
}
```