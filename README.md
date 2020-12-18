A simple peak + smooth external.

Installation:

```
make install pd_include=/path/to/pd/src objectsdir=/path/to/pd-externals
```

on Bela:
```
make install
```
is sufficient (defaults are set appropriately)

on Mac, e.g.:

```
make pd_include=/Applications/Pd-0.51-0.app/Contents/Resources/src/ objectsdir=~/Documents/Pd/externals/
```
(you may have to adjust the `pd_include` path to match the version of Pd you have installed).
