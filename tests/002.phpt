--TEST--
Composition with inheritance
--FILE--
<?php 
use Componere\Definition;
use Componere\Method;

class A {
	public function thing() {
		return true;
	}
}

$definition = new Definition(B::class, A::class);
$definition->addMethod("member", new Method(function(){
	return parent::thing();
}));
$definition->register();

$b = new B();

var_dump($b->member());
?>
--EXPECT--
bool(true)
