--TEST--
Composition magic
--FILE--
<?php 
use Componere\Definition;
use Componere\Method;

$definition = new Definition(A::class);
$definition
	->addMethod("__construct", new Method(function(){ echo "ctor\n"; }))
	->addMethod("__destruct", new Method(function(){ echo "dtor\n"; }))
	->addMethod("clone", new Method(function(){ echo "clone\n"; }))
	->addMethod("__get", new Method(function(){ echo "get\n"; }))
	->addMethod("__set", new Method(function(){ echo "set\n"; }))
	->addMethod("__unset", new Method(function(){ echo "unset\n"; }))
	->addMethod("__call", new Method(function(){ echo "call\n"; }))
	->addMethod("__callstatic", new Method(function(){ echo "callstatic\n"; }))
	->addMethod("__toString", new Method(function(){ echo "tostring\n"; return ""; }))
	->addMethod("__debugInfo", new Method(function(){ echo "debuginfo\n"; }))
	->register();

$a = new A();

var_dump($a->get);
$a->set = 10;
unset($a->set);

$a->call();
A::callStatic();

(string) $a;
var_dump($a);
?>
--EXPECTF--
ctor
get
NULL
set
unset
call
callstatic
tostring
debuginfo
object(A)#%d (%d) {
}
dtor
