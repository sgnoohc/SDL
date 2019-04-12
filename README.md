## SDL namespace

The namespace ```SDL``` contains:

    namespace SDL
    {
        class Event;
        class Hit;
        class Module;
        class MiniDoublet;

        namespace Math;

        // Few extra print utility classes in PrintUtil.h (not important.)
    }

### Event

Holds all the pointers to hits, modules, mini-doublets (NOTE: to be done), and more in the instance.  

### Hit

Currently only contains 3-vector. May need to add ```detId```. (Haven't decided.)

### Module

A module is defined as *one* side of the double-modules.
Each side of the double-modules has unique ```detId```.  

Each module has various qualifiers (e.g. layer, subdet, side, etc.).  
The ```Module.h``` explains them.  

### MiniDoublet

Holds two hits.

### Math

It is a namespace that will hold various arithmetic operation functions.
