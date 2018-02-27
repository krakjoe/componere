--TEST--
getClosures
--FILE--
<?php
use Componere\Definition;

class A {
	public function foo() {

	}
}

$a = new Definition(A::class);

var_dump($a->getClosures());
?>
--EXPECTF--
array(1) {
  ["foo"]=>
  object(Closure)#%d (0) {
  }
}

