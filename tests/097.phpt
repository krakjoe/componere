--TEST--
Componere\cast_by_ref handle dynamics non-existent property
--FILE--
<?php
class A {

}

class B extends A {

}

$a = new A();
$a->none = 1;

var_dump(Componere\cast(B::class, $a));
?>
--EXPECT--
object(B)#2 (0) {
}


