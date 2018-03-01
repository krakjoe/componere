--TEST--
Method clone
--FILE--
<?php
use Componere\Method;

$m = new Method(function(){});

var_dump($m, $c = clone $m);
?>
--EXPECT--
object(Componere\Method)#1 (0) {
}
object(Componere\Method)#2 (0) {
}



