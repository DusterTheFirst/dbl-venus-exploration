# Driver style guide

"drivers" for various pieces of hardware are separated into various subdirectories
in this directory. Each driver will compiled into static libraries and linked into
the executable file. The reason for this separation is to allow each driver to define
its own global variables and functions without interfering with other driver modules.

In order for the main program to know the functions provided by the driver module,
it must provide a `.hpp` file with the same name as its parent folder. This `.hpp`
file must contain one namespace with the same name as the driver module with
every public facing function of the module being defined in the same namespace.

For example, if you had a driver for the `motor`, your `motor.hpp` file would begin
as follows:

```cpp
// In file: motor.hpp

// The following line is required to tell the compiler to only ever include this
// header once
#pragma once

namespace motor {
    void init();

    // ... more functions provided by the motor module
}
```

It is common practice for each driver module to provide an `init` function which
will initialize all the hardware as well as put the driver code into a known state.

Since the header file only defines the names and types of functions provided by
the module, the functions must be defined elsewhere. It is advised to define them
in a `.cpp` file with the same name as the module, in this example's case, that
would be the file `motor.cpp`.

```cpp
// In file: motor.cpp
void motor::init() {
    // Initialization code
}
```

Since the `init` function was defined in the motor module, you must add `motor::`
before its name in the `.cpp` file so the compiler can know these two functions are
the same
