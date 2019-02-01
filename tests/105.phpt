--TEST--
constants from internals leaking
--FILE--
<?php
class Foo extends DateTime {
	
}

$def = new Componere\Definition(Foo::class);
$def->register();

var_dump(new Foo());
?>
--EXPECTF--
object(Foo)#%d (%d) {
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}


