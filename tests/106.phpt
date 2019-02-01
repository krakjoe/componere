--TEST--
name and parent must not be the same class
--FILE--
<?php
class Foo {
	
}

new Componere\Definition(Foo::class, Foo::class);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name and parent cannot refer to the same class in %s:6
Stack trace:
#0 %s(6): Componere\Definition->__construct('Foo', 'Foo')
#1 {main}
  thrown in %s.php on line 6


