--TEST--
Componere\cast by reference
--INI--
error_reporting=E_ALL &~ E_NOTICE
--FILE--
<?php
use function Componere\cast;

class Base {
	public $std;
}

class Child extends Base {
	public $two = [1,2];
}

$base = new Base;

$child = &cast(Child::class, $base);

var_dump($base);
$child->std = new stdClass;
var_dump($base);

$base = new Base;

$child = cast(Child::class, $base);

var_dump($base);
$child->std = new stdClass;
var_dump($base);
?>
--EXPECT--
object(Base)#1 (1) {
  ["std"]=>
  &NULL
}
object(Base)#1 (1) {
  ["std"]=>
  &object(stdClass)#3 (0) {
  }
}
object(Base)#4 (1) {
  ["std"]=>
  NULL
}
object(Base)#4 (1) {
  ["std"]=>
  NULL
}




