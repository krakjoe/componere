--TEST--
Value default
--FILE--
<?php
use Componere\Value;

$d = new Value();

var_dump($d->hasDefault());

$d = new Value(1);

var_dump($d->hasDefault());

$d = new Value([]);

var_dump($d->hasDefault());

$d = new Value([1]);
?>
--EXPECTF--
bool(false)
bool(true)
bool(true)

Fatal error: Uncaught InvalidArgumentException: values of type array cannot be declared with default values in %s:16
Stack trace:
#0 %s(16): Componere\Value->__construct(Array)
#1 {main}
  thrown in %s on line 16




