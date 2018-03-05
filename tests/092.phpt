--TEST--
Componere\cast incorrect invocation (5)
--FILE--
<?php
use function Componere\cast;

$instance = function() {
	yield;
};

cast(Generator::class, $instance());
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: Generator does not support cloning in %s:8
Stack trace:
#0 %s(8): Componere\cast('Generator', Object(Generator))
#1 {main}
  thrown in %s on line 8

