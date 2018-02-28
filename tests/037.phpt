--TEST--
Patch with closures
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

foreach ($a->getClosures() as $name => $closure) {
	var_dump($name, $closure());
}
?>
--EXPECTF--
string(6) "method"
object(A)#%d (0) {
}

