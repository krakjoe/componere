--TEST--
Allow patching objects of an internal class
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

$instance = new DateTime();

$p = new Patch($instance);
$p->addMethod("__toString", new Method(function(){
	return "ok";
}));
$p->apply();

echo (string) $instance;
?>
--EXPECT--
ok

