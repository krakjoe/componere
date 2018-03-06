--TEST--
Componere\cast_by_ref
--FILE--
<?php
use function Componere\cast_by_ref as cast;

class Base {
	public $std;
}

class Child extends Base {
	public $two = [1,2];
}

$base = new Base;

$child = cast(Child::class, $base);

$child->std = new stdClass;

var_dump($base);
?>
--EXPECT--
object(Base)#1 (1) {
  ["std"]=>
  &object(stdClass)#3 (0) {
  }
}





