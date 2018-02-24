--TEST--
Recomposition
--FILE--
<?php 
use Componere\Definition;
use Componere\Method;

class A {
	public function thing() {
		return true;
	}
}

$definition = new Definition(A::class);
$definition->addMethod("member", new Method(function(){
	return false;
}));
$definition->register();

$a = new A();

var_dump($a->member());
?>
--EXPECT--
bool(false)
