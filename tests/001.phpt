--TEST--
Composition without inheritance
--FILE--
<?php 
use Componere\Definition;
use Componere\Method;

$definition = new Definition(A::class);
$definition->addMethod("member", new Method(function(){
	return true;
}));
$definition->register();

$a = new A();

var_dump($a->member());
?>
--EXPECT--
bool(true)
