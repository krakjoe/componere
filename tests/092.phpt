--TEST--
Componere\cast covariant
--FILE--
<?php
use function Componere\cast;

class Base {
	public $one = 1;
}

class Child extends Base {
	public $two = 2;
}

$base = new Base;

var_dump($base);

var_dump(cast(Child::class, $base));
?>
--EXPECT--
object(Base)#1 (1) {
  ["one"]=>
  int(1)
}
object(Child)#2 (2) {
  ["two"]=>
  int(2)
  ["one"]=>
  int(1)
}

