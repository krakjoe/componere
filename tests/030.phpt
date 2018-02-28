--TEST--
Definition::patch basics
--FILE--
<?php
class A {
	public function method() {
		return true;	
	}
}

$instance = new A();
$patch = new Componere\Patch($instance);
$patch->addMethod("method", new Componere\Method(function(){
	return false;
}));

var_dump($instance->method()); /* bool(true) */

$patch->apply();

var_dump($instance->method()); /* bool(false) */

$patch->revert();

var_dump($instance->method()); /* bool(true) */

$patch->apply();

$patch->addMethod("method", new Componere\Method(function(){
	return 42;
}));

var_dump($instance->method()); /* int(42) */

unset($patch);

var_dump($instance->method()); /* bool(true) */
--EXPECT--
bool(true)
bool(false)
bool(true)
int(42)
bool(true)
