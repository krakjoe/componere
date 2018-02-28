--TEST--
Patch with closure
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

class A {
	public function method() {
		return $this;	
	}
}

$instance = new A();

$a = new Patch($instance);

$closure = $a->getClosure("method");

var_dump($closure());
?>
--EXPECTF--
object(A)#%d (0) {
}

