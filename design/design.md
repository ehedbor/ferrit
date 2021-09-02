# ES Design Document

This is an overview of what is planned to be added to the ES language.

## Basic Types

ES has the following primitive types:
 * 8-bit integers: `sbyte` and `byte`
 * 16-bit integers: `short` and `ushort`
 * 32-bit integers: `int` and `uint`
 * 64-bit integers: `long` and `ulong`
 * 32-bit floats: `float`
 * 64-bit floats: `double`
 * booleans: `bool`
 * characters: `char`. Note that `char` is only 8 bits in size and does not necessarily represent a complete unicode character.

Each of these types can be created with literals. Take numeric literals, for example:

    // Integer literals are represented with whole number values
    val myInt = -63 // type is int
    val bigInt = 5000000 // still an int 

    // A base can be specified by prefixing the literal with the base in decimal, then the letter 'b'
    // The base can be an integer within 2 to 36 (inclusive).
    // First the digits 0-9 are exhausted, then the letters A-Z (with A=10, B=11, etc.) 
    // For hexidecimal numbers, the literal can be prefixed with '0x' instead.
    val hexInt0 = 16bA9 // int with value 169 
    val hexInt1 = 0xA9 // same value
    val hexInt2 = 0xa9 // letters are case-insensitive
    
    val binInt = 2b11001001 // int with value 201
    val octInt = 8b257 // int with value 175
    val notOct = 0257 // not octal! value = 257

    // Floating-point literals are represented with decimal values 
    val myDouble = 23.985 // type is double
    
    // The whole number component and fractional component must both be specified
    //val notADouble0 = 32. 
    //val notADouble1 = .1463 
    //val notADouble2 = 64.abs()
    val aDouble0 = 39.0
    val aDouble1 = 932.0.abs()

    // Bases can also be specified for floats
    val neato = 6b0.3 // double with value 0.5

    // Exponential notation is also supported.
    // The exponent must be an integer
    val oneThousand = 1.0e3 // = 1000.0 
    val threeHalves = 15.0e-1 // = 1.5

    // Custom bases can be combined with exponential notation
    val interesting = 6b3.4e4 // = 6b3.4 * 6b10 ^ 6b4 = 3.4 * 6 ^ 4 = 4665.6
    //val invalidHex = 0x3.e4 // illegal, no fractional component
    val validHex = 0x3.ee4 // allowed. = 0x3.E * 0x10^0x4


    // Literals for numeric types other than int or double can be created by manually
    // specifying the type or by converting the literal to the appropriate type:
    val myByte0: byte = 2b11001001 // byte with value 201
    val myByte1 = 2b11001001 to byte // calls int.operator to<byte>() 
    //val myByte10 = 2b11001001 as byte // casting is illegal, as byte is not a subclass of int

    val myLong0: long   = 13289219839
    val myLong1         = 13289219839 to long
    val myFloat0: float = 3298.23
    val myFloat1        = 3298.23 to float

Literals for non-numeric types also exist:

    val myBool = true
    val yourBool = false
    val aChar = 'q'
    // more on these later:
    val aString = "Wait, strings aren't primitive types!"
    val triBool: bool? = null