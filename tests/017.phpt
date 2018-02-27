--TEST--
Definition redeclare class implementing interfaces
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

interface IFace {
	public function thing();
}

class A implements IFace {
	public function thing() {
		return true;
	}
}

$componere = new Definition(A::class);
$componere->addMethod("thing", new Method(function(){
	return false;
}));
$componere->register();

$a = new A();

var_dump($a instanceof IFace);

var_dump($a->thing());
?>
--EXPECT--
bool(true)
bool(false)
