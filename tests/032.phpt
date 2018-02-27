--TEST--
Definition::patch compatible patches
--FILE--
<?php
interface IFace {
	public function method();
}

class A {
	public function method() {
		return true;
	}
}

function needs(IFace $a) {
	var_dump($a->method());
}

$instance = new A();

var_dump($instance->method());

$a = new Componere\Definition(A::class, [IFace::class]);
$a->addMethod("method", new Componere\Method(function(){
	return false;
}));
$a->patch($instance);

var_dump($instance->method());

$b = new Componere\Definition(B::class, $a);
$b->addMethod("method", new Componere\Method(function(){
	return 42;
}));

$b->patch($instance);

needs($instance);
--EXPECT--
bool(true)
bool(false)
int(42)

