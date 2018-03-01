--TEST--
Value refcounted destruction
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$v = new Value(
	$s = sprintf("hello world %s", __FILE__));

$e = new Definition(E::class);
$e->addProperty("property", $v);
$e->register();

var_dump(new E());
?>
--EXPECTF--
object(E)#%d (%d) {
  ["property"]=>
  string(%d) "hello world %s"
}


