==============================================
Half-Life function hooking/overriding on macOS
==============================================

.. image:: https://raw.githubusercontent.com/Matherunner/hl-mac-hook/master/demo/crossfire.jpg

This is a proof-of-concept of function hooking/overriding in the macOS version
of Half-Life. This demonstrates that hooking-based mods like BunnymodXT or
hl-capture can be made to work on the Mac.

Building
========

Make sure that the ``mach_override`` submodule has been cloned.

::

   $ cd mach_override
   $ make -j
   $ cd ..
   $ make -j

Running
=======

::

   $ cd <Half-Life directory>
   $ STEAM_DYLD_INSERT_LIBRARIES=<absolute path to libmachook.dylib> ./hl.sh

The ``STEAM_DYLD_INSERT_LIBRARIES`` is a variable recognised by ``hl.sh`` to be
stored into ``DYLD_INSERT_LIBRARIES``. An alternative, of course, is to modify
``hl.sh`` itself or to export ``DYLD_INSERT_LIBRARIES``.

Mechanism
=========

The library first hooks into ``dlopen`` by function interposition, namely using
the ``DYLD_INSERT_LIBRARIES`` method and generating a ``__interpose`` section in
the ``__DATA`` segment using `this macro`_. Using ``DYLD_INSERT_LIBRARIES``
alone would not work in recent Macs due to the `SIP`_.

.. _this macro: https://opensource.apple.com/source/dyld/dyld-239.3/include/mach-o/dyld-interposing.h
.. _SIP: https://en.wikipedia.org/wiki/System_Integrity_Protection

This method, however, would not work for private extern symbols which is most of
the functions. Look at this following partial output:

::

   $ nm -a hl.dylib | grep '[tT].*PM_'
   000bc260 t _PM_Accelerate
   000bb720 t _PM_AddCorrectGravity
   000be980 t _PM_AddGravity
   000bb430 t _PM_AddToTouched
   000bcb30 t _PM_AirAccelerate
   000bcf60 t _PM_AirMove
   000bfda0 t _PM_CalcRoll
   000bd350 t _PM_CatagorizePosition
   000bae80 t _PM_CatagorizeTextureType
   000bfa60 t _PM_CheckFalling
   000bfeb0 t _PM_CheckParamters
   000bd730 t _PM_CheckStuck
   ...

Indeed, the lowercase ``t`` beside the symbols indicate that they are local to
the library. These functions also cannot be looked up by ``dlsym``. Fortunately,
as the symbols are still present in the library, we can look up the offset of
any function by walking through the symbol table pointed by the ``LC_SYMTAB``
segment command and checking the corresponding name string stored in the string
table. However, as the symbol table is not stored permanently into memory, we
must open the ``.dylib`` file itself to obtain the function offsets. We obtain
the path to the ``.dylib`` from the hooked ``dlopen``.

The base address of any loaded library must be obtained by calling
``task_info``, which enumerates all loaded dynamic libraries. We can eventually
obtain an array of ``dyld_image_info`` which contain the names and base
addresses, among other information. The relevant code is modified from `this
gist`_. Upon knowing the base address we can compute the actual addresses of the
functions we are interested in. And to override the functions, we use
`mach_override`_.

.. _this gist: https://gist.github.com/P1kachu/e6b14e92454a87b3f9c66b3163656d09
.. _mach_override: https://github.com/rentzsch/mach_override
