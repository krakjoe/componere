--TEST--
Componere\cast incorrect invocation (4)
--FILE--
<?php
use function Componere\cast;

$instance = new stdClass;

cast(DateTime::class, $instance);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: DateTime is not compatible with stdClass in %s:6
Stack trace:
#0 %s(6): Componere\cast('DateTime', Object(stdClass))
#1 {main}
  thrown in %s on line 6

