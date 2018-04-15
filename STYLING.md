# LlanOS Styling Guide

Welcome to the LlanOS styling guide. The LlanOS Operating System will have some very tough styling guidelines to make code style very consistent among
potentially many developers.

Eventually, a linter will be implemented to cleanup any code styling violations, but for now, this document will need to be followed manually.

## Function and Variable Naming

Functions and Variables should be very explicitely named. No abbreviations should be used unless they are commonly understood. For example, you may use `i` or `j` to represent indicies of a list, but if iterating through a matrix, you should use the `row` and `column` terminology.

If a variable or function name needs multiple words, lowercase `snake_case` should be used. For example, a function or variable should be named `int this_is_a_function(...)` or `int this_is_a_variable`.

You should also keep function and variable names concise if at all possible, but do not let concise naming loose the meaning of the variable name. For example, if you have a function that iterates through a list, and it is the only list contained within the function, then you should not name the variable `list_index`. Keep it simple and name the variable `index`. It is understood that `index` will be the index of the list.

## Unions

Please do not use Unions unless it is absolutely necessary as defined by other subsystems or for hardware memory mapping usages.

## Structures and Enumerations Naming and Spaces

C allows Structures and Enumerations to be defined with the same name where the programmer could use the `struct` and `enum` keywords to reference the constructs independently. Do not use this feature. To enforce a unique naming scheme, you should always typedef enumerations and structures with `_t`, prefix the construct with the name of the class it lies within, and typedef before defining the construct. You should also never define 2 variables on the
same line. For example, the following example is not okay.

```
typedef struct {
    int a, b;
} multivalue_t;

int function_name(multvalue_t t);

struct lists {
    int* a;
    int* b;
    int* c;
};

enum lists {
    a = 0, b, c
};

void build_list(struct lists* l, enum lists ltype);
```

There are a few things wrong with the previous example:

* Functions are declared in between constructs which makes them hard to find.
* One structure is exposed as a type while another structure and enumeration is not.
* There is a structure and an enumeration with the same name.
* The `multivalue_t` structure is type-defined anonymously.
* Variables inside `multivalue_t` are defined on the same line.
* Enumeration names inside `lists` are defined on the same line.
* The funciton `build_list` uses the `struct` and `enum` keywords.
* The names of the function parameters are mostly meaningless.

Instead, you should define the previous constructs as:

```
typedef struct multivalue_s multivalue_t;
typedef struct list_container_s list_container_t;
typedef struct list_choices_e list_choices_t;

struct multivalue_s {
    int a;
    int b;
};

struct list_container_s {
    int a;
    int b;
};

enum list_choices_e {
    a = 0,
    b,
    c
};

extern int function_name(multivalue_t value);
extern void build_list(list_container_t* container, list_choices_t choice);
```

The method state above mostly helps with readability and will enforce all available types to be declared at the top of the file, types will be declared elow, while functions will be declared last. One other benefit of this definition method is that you will be able to use other types inside types without having to think about it. A linked list is a good example:

```
typedef struct linked_list_node_s linked_list_node_t;
typedef struct linked_list_s linked_list_t;

struct linked_list_node_s {
    linked_list_node_t* next;
    void* value;
};

struct linked_list_s {
    linked_list_node_t* head;
    uint32_t length;
};
```

## Header file headings

Although it may be controversial, you should use `#pragma once` at the top of any C header file. It is clean, states what it does, does not pollute the header file by including its contents within a `#ifndef ... #define ... #endif`, and most new compilers support the option.

## Functions

Single line declarations should resemble:

```
extern void function_name(int param1, void* param2, uint32_t param3);
```

Multiline declarations should resemble:

```
extern void function_name(
    int param1,
    void* param2,
    uint32_t param3
);
```

Notice the use of `extern` on declarations. This is present implicitely by the compiler, but you should always add it to be more explicit about what is happening.

## Usage of Built-in Types

Although the usage of builtin types can be very convienent, it is looked down upon in the OS world. The `stdint.h`, `stdbool.h`, and `stddef.h` files include a bunch of nice explicit types that should be preferred over builtin types. For example, use `uint32_t` instead of `unsigned` or `unsigned int`. Use `int64_t` instead of `long`. Use `int16_t` instead of `short`. And please use `bool` from `stdbool.h` instead of `int`.

## Includes

When including a header, you should always include what you need in the file that you need it. Including everything that you need in the header `.h` and then including your header into the source `.c` can cause circular dependencies. Instead, if you do not need an `#include` in the header, then do not put it in the header. Place it in the source file.

All includes should also be defined at the top of the file, never within the file.

## Braces

Braces should be placed on the same line as the closing parentheses in all cases (even multiline function definitions). Parameters in a multiline function definition should be indented 2 levels to visually break apart the function body from the defition. Do not add any empty lines after the function name. Empty function definitions should be limited, but are usually understood to be placeholders. Empty function bodys should be placed on the same line as the name. Here are a few examples of proper brace placement.

```
int function_name() { }

int function_name() {
    return 12;
}

int function_name_with_many_params(
        int a,
        int b,
        int c) {
    int a;
    int b;

    return a + b;
}

## Scope

Please do not define anonymous scopes. For example, your code should not contain anything in the form:

```
int function(int a) {
    int b;
    {
        b = 1;
        b += a;
    }

    {
        b += a;
    }
    return b;
}
```

## Comments

Single line comments should not be used to keep a consistent style. All comments should be placed on the top of where they are needed using the multline syntax (even for single line comments). For example, the following should not be done:

```
int function(int a) {
    // do something...
    a += 1;

    a++;  // add one

    return a;
}
```

Instead, the following comment style is preferred:

```
int function(int a) {
    /* do something... */
    a += 1;

    /* add one */
    a++;

    return a;
}

## Static Variables

Do not define static variables inside functions. Functions should never change state. A function should take in a set of parameters and always return the same value for that set of parameters (functional style programming).

## Private Functions

Any function defined inside a source file that is meant to be private should be prefixed by the `static` keyword. This restricts access to the function from anywhere outside the source file. Also, prefix the function name with 2 underscores so that when the function is used within the source file, it is obvious that it is a private function.

## Documentation

Function documentation should be placed at the top of the function declaration in the header file. If there is not function declaration in the header file, then the function is private and should be documented in the source file. Documenting a function includes adding the documentation multiline comment above the function declaration. For example:

```
/**
 * Function short description.
 *
 * Function detailed description.
 *
 * @param param1 parameter docs.
 * @param param2 parameter docs.
 * @return return value docs.
 */
int function(int param1, int param2);
```
