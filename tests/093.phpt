--TEST--
Componere\cast contravariant
--FILE--
<?php
use function Componere\cast;

class Base {
	public $one = 1;
}

class Child extends Base {
	public $two = 2;
}

$child = new Child;
$child->one++;
$child->two = new stdClass;

var_dump(cast(Base::class, $child));
?>
--EXPECT--
object(Base)#3 (1) {
  ["one"]=>
  int(2)
}



