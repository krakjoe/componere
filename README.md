componere
=========
*composing classes at runtime since 2018*

[![Build Status](https://travis-ci.org/krakjoe/componere.svg?branch=master)](https://travis-ci.org/krakjoe/componere)

This extension allows the composition and re-composition of classes at runtime, using dark scary magic, but of the kind you might take to production.

API
===

```
<?php
function compose(string $name, array $functions, string $parent = null, array $properties = []);
```

Explanation
===========

These are for the weak ...
