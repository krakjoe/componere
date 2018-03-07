--TEST--
Componere\cast incorrect invocation (2)
--FILE--
<?php
use function Componere\cast;

$instance = new stdClass;

interface IFace {}

cast(IFace::class, $instance);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot cast to interface IFace in %s:8
Stack trace:
#0 %s(8): Componere\cast('IFace', Object(stdClass))
#1 {main}
  thrown in %s on line 8
