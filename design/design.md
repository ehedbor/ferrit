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
 * 8-bit integers: `Byte`
 * 16-bit integers: `Short`
 * 32-bit integers: `Int`
 * 64-bit integers: `Long`
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
| `Byte`   | `42b`           | 
| `Short`  | `42s`           | 
| `Int`    | `42`, `42i`     | 
| `Long`   | `42L`           |
| `Float`  | `42.0f`         |
| `Double` | `42.0`, `42.0d` |

Alternatively, you may explicitly specify the literal's type or use a conversion function.

```
val myByte = 0b11001001b 
val myByte: Byte = 0b11001001
val myByte = 0b11001001.toByte() 
// Illegal: Byte is not a subclass or superclass of Int
//val myByte = 2b11001001 as Byte 

// Illegal: integer literals can only be assigned to integer types,
// and floating-point literals can only be assigned to float types
//val myInt: Int = 32.0 
//val myDouble: Double = 259 
// Allowed
val myInt = 32.0.toInt()
val myDouble = 259.toDouble()
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
val myInclusiveRange = 1...10 
```

## Operators
Ferrit supports the following operators:
- Basic operators: `+ - * / %` 
- Logical operators: `! && ||`
- Bitwise operators: `~ & | << >>`
- Comparison operators: `== != > < >= <=`
- Assignment operators: `= += -= *= /= %= &&= ||= &= |= <<= >>=`
- Type operators: `is` `!is` `as` `as?` 
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
// Note that this does NOT return a lambda!
fun doSomeMath(n: Int) = {
    println("This is a block!")
    if (n % 2 == 0) {
        n / 2
    } else {
        3 * n + 1 
    } 
}
```

### Named Arguments

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

### Vararg Functions

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

Ferrit provides a concise syntax to create lambda functions (anonymous functions). 

```
// type is (Double, Double, Double) -> Double
val slopeFormula = { x: Double, y: Double, b: Double -> 
    (y - b) / x 
}

```

When passed to a function, the lambda's argument types can be omitted.  

If the lambda is the last argument then it may be written outside the argument 
list. In addition, if there are no other arguments to the function, the 
parentheses may be omitted.

```
fun fooBar(foo: Int = 39, bar: (Int) -> Unit) { 
    bar(foo)
}
 
fooBar(20, { f -> println("Foo is " + f) }) 

// Shortened syntax: 
fooBar(30) { f -> println("Foo is " + f) }

// Even shorter:
fooBar { f -> println("Foo is " + f) }
```

### Extensions

Ferrit allows you to mimic extension functions with the `::` operator.
Using it simply passes the value on the left as the first argument 
to the function on the right.

```
fun printMe(obj: Any) { println(obj) }

10::printMe() // printMe(10)
"Hello, extensions!"::printMe() // printMe("Hello, extensions!")

```

### Operator Overloading

Ferrit allows you to provide custom implementations for operators 
with the `operator` modifier. Operators may either be methods or
free functions.

```
class MyClass {
    primary init(var value: Int)
    
    operator fun increment() = { 
        println("This is a custom operator!")
        value++
        this
    }
}

operator fun decrement(instance: MyClass) = {
    println("Also a custom operator"!)
    instance.value--
    instance
}

val instance = MyClass()
instance++     // calls instance.increment()
instance--     // calls instance::decrement() AKA decrement(instance)
```

Here's a list of all overloadable operators:

#### Unary operators
| Expression | Translated to    |
|------------|------------------|
| `+a`       | `a.unaryPlus()`  |
| `-a`       | `a.unaryMinus()` |
| `!a`       | `a.not()`        |
| `~a`       | `a.bitNot()`     |

#### Arithmetic operators
| Expression | Translated to     |
|------------|-------------------|
| `a + b`    | `a.plus(b)`       |
| `a - b`    | `a.minus(b)`      |
| `a * b`    | `a.times(b)`      |
| `a / b`    | `a.divide(b)`     |
| `a % b`    | `a.modulo(b)`     |

#### Bitwise operators
| Expression | Translated to     |
|------------|-------------------|
| `a << b`   | `a.shiftLeft(b)`  |
| `a >> b`   | `a.shiftRight(b)` |
| `a & b`    | `a.bitAnd(b)`     |
| `a | b`    | `a.bitOr(b)`      |

#### Compound assignment operators
| Expression  | Translated to           |
|-------------|-------------------------|
| `a += b`    | `a.plusAssign(b)`       |
| `a -= b`    | `a.minusAssign(b)`      |
| `a *= b`    | `a.timesAssign(b)`      |
| `a /= b`    | `a.divideAssign(b)`     |
| `a %= b`    | `a.moduloAssign(b)`     |
| `a <<= b`   | `a.shiftLeftAssign(b)`  |
| `a >>= b`   | `a.shiftRightAssign(b)` |
| `a &= b`    | `a.bitAndAssign(b)`     |
| `a |= b`    | `a.bitOrAssign(b)`      |

Implementing a binary arithmetic/bitwise operator will also create
a default implementation for the compound assignment operators.

If both a binary and assignment operator are valid,
the assignment will be preferred.

#### Index operators
| Expression       | Translated to       |
|------------------|---------------------|
| `a[i]`           | `a.get(i)`          |
| `a[i, j, j]`     | `a.get(i, j, k)`    |
| `a[i] = b`       | `a.set(i, b)`       |
| `a[i, j, k] = b` | `a.set(i, j, k, b)` |

#### Comparison operators
| Expression | Translated to         |
|------------|-----------------------|
| `a > b`    | `a.compareTo(b) > 0`  |
| `a >= b`   | `a.compareTo(b) >= 0` |
| `a < b`    | `a.compareTo(b) < 0`  |
| `a <= b`   | `a.compareTo(b) <= 0` |
| `a == b`   | `a.equals(b)`         |
| `a != b`   | `!a.equals(b)`        |

#### Other operators
| Expression | Translated to     |
|------------|-------------------|
| `a in b`   | `a.contains(b)`   |
| `a !in b`  | `!a.contains(b)`  |
| `a..b`     | `a.rangeTo(b)`    |
| `a...b`    | `a.rangeToInclusive(b)` |

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
class Address {
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
    
    override fun toString() = {
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
    
    override fun toString() {
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
    var customBehavior: Int { private get } = 10
    var something: Double {
        module get() = random.next() 
        protected set(value) { panic("lol") }
    }
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

Collection constructors can be combined with the `to` function to make map-like literals.
```
class CookieRatings {
    primary init[val data: Pair<String, Int>...] 
}
val cookieRatings = CookieRatings["Peanut Butter"::to(10), "Chocolate Chip"::to(8), "Oatmeal Raisin"::to(7)] 
```

### Polymorphism

To make a class polymorphic, declare it and all overridable properties/methods with the `open` modifier.

```
open class Base {
    primary init(
        public val alpha: Int, 
        module val beta: Int, 
        protected val gamma: Int, 
        private val delta: Int
    ) {
        println("Base init")
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

### Abstract Classes 

You can use the `abstract` keyword to declare classes and methods that do not have an implementation:

```
abstract class Shape {
    primary init(var x: Double, var y: Double)
    
    abstract val perimeter: Double
    abstract val area: Double
}
```

This can be implemented like normal:
```
class Rectangle : Shape {
    primary init(
        x: Double, 
        y: Double, 
        var length: Double, 
        var width: Double,
    ) : super(x, y)
    
    override val perimeter: Double 
        get() = 2.0 * (length + width)
    override val area: Double
        get() = length * width
}

// Illegal - Shape is abstract
//val shape = Shape(3.0, 4.0)
//println("perimeter = " + shape.perimeter)

val shape: Shape = Rectangle(3.0, 4.0, 10.3, 3.7)
println("perimeter = " + shape.perimeter)
```

### Traits

Abstract classes have a lot of uses, but they aren't without their limitations. 
Classes can only have one superclass, so you can't use them in situations where 
you want more than one "contract". In this situation, it might be preferable 
to use a trait. 

Traits define methods and properties without providing an implementation. 
Their advantage is that classes can implement any number of traits.

Here's what the previous example would look like with traits:

```
trait Shape {
    var x: Double 
    var y: Double
    var perimeter: Double
    var area: Double
}

class Rectangle : Shape {
    primary init(
        override var x: Double, 
        override var y: Double, 
        var length: Double, 
        var width: Double,
    )
    
    override val perimeter: Double
        get() = 2.0 * (length + width) 
    override val area: Double
        get() = length * width
}
```

## Objects

An object is a class with a single instance (a singleton). It is declared with 
the `object` keyword in much the same way as a class.

```
object Logger {
    fun info(msg: String) {
        println("[INFO]: " + msg)
    }
}

val x = 1 + 2
Logger.info("added one to two. result: " + x)
```

Like classes, objects can inherit from one class and any number of traits. 
They may also define a no-argument constructor (but no other constructors).
Objects cannot be `open` or `abstract`.

Objects may also be declared as expressions. Aside from not having a name,
anonymous objects work exactly like normal objects. 

```
val messagePrinter = object /* : Any */ {
    var message = "Hello, world!"

    fun display() {
        println(message)
    }
}
messagePrinter.display()
```

### Companion Objects

All classes have a special object instance attached to them called a companion.
These allow you to mimic static properties and methods in other languages.
The advantage of objects is that they can implement traits and classes and 
can be passed to functions.

```
class MyClass {
    private primary init()
 
    companion object {
        fun create() = MyClass()  
    }
}

val instance = MyClass.create()
val instance = MyClass.Companion.create()
```

It can be a hassle to try and fit all class-specific behavior into the 
companion object declaration (for instance, if you want private api functions
to be separate from public functions). To alleviate this, class-level properties
and  methods can be annotated with the `friend` modifier. This treats them as if
they were declared inside the companion instead.

```
class MyClass {
    friend val Constant = 17
    var myProperty = 23
 
    primary init() { ... }

    friend fun publicApi() { ... }
    fun someInstanceSpecificBehavior() { ... }
    
    private friend fun privateApi() { ... }
    private fun somePrivateStuff() { ... }
}

// Equivalent to:
class MyClass {
    companion object {
        val Constant = 17
        fun publicApi() { ... }
        private fun privateApi() { ... }
    }
    
    var myProperty = 23
    primary init() { ... }
    fun someInstanceSpecificBehavior() { ... }
    private fun somePrivateStuff() { ... }
}
```

## Generics

Classes and functions can have type parameters.

```
class Box<T> {
    primary init(var value: T)
}

val box: Box<Int> = Box<Int>(1)
```

Just like with normal type specifiers, generic specifiers can be omitted if the type can be inferred:

```
val box = Box(1)
```

### Variance

You can specify "producers" (covariance) and "consumers" (contravariance) of a
type parameter with the `out` and `in` keywords.

These keywords can appear at the declaration...

```
trait Comparable<in T> {
    operator fun compareTo(other: T) -> Int
}

val a: Comparable<Number> = 7.0
val b: Comparable<Int> = 3

a.compareTo(b) // OK
```

or at the use site:

```
class Array<T> { ... }

fun copy(from: Array<out Any>, into: Array<Any>) { ... }
```

If you know nothing about a type, you can specify that too with `*`.

```
val someList: List<*>
```


### Constraints

You can constrain the set of types with an upper bound:

```
fun <T : Comparable<T>> sort(list: List<T>) { ... }
```

If you don't specify an upper bound, it is assumed to be `Any`.


### Example - Point
Here's an example of a generic class that implements the traits `Into<String>`, `Equate<Vector2<N>>` and `Hash`.

```
using ferrit.math.[sqrt, pow]

class Vector2<N : Number> : Into<String>, Equate<Vector2<N>>, Hash {
    primary init(val x: N, val y: N)
    
    val magnitude get() = sqrt(pow(x, 2) + pow(y, 2))  
    
    override operator fun to() -> String = {
        "(" + x to String + ", " + y to String + ")"
    }
    
    override operator fun equals(other: Vector2<N>) = {
        x == other.x && y == other.y
    }
    
    override val hashCode = x ^ 31 + y
}
```