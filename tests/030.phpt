--TEST--
Definition::patch basics
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class A {
	public function foo() {
		return false;
	}
}

$instance = new A();

var_dump($instance->foo());

$a = new Definition(A::class);
$a->addMethod("foo", new Method(function(){
	return true;
}));

$a->patch($instance);

var_dump($instance->foo());
?>
--EXPECT--
bool(false)
bool(true)
