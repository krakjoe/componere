--TEST--
Componere\cast_by_ref handle dynamics no refs
--FILE--
<?php
class A {

}

class B extends A {
	public $aref;
}

class C extends B {}

$a = new A();

$a->aref = "one";

$b = Componere\cast_by_ref(B::class, $a);

var_dump($a, $b, $c = Componere\cast_by_ref(C::class, $b));

$c->aref = "two";

var_dump($a, $b, $c);

$a->aref = "three";

var_dump($a, $b, $c);
?>
--EXPECT--
object(A)#1 (1) {
  ["aref"]=>
  &string(3) "one"
}
object(B)#2 (1) {
  ["aref"]=>
  &string(3) "one"
}
object(C)#3 (1) {
  ["aref"]=>
  &string(3) "one"
}
object(A)#1 (1) {
  ["aref"]=>
  &string(3) "two"
}
object(B)#2 (1) {
  ["aref"]=>
  &string(3) "two"
}
object(C)#3 (1) {
  ["aref"]=>
  &string(3) "two"
}
object(A)#1 (1) {
  ["aref"]=>
  &string(5) "three"
}
object(B)#2 (1) {
  ["aref"]=>
  &string(5) "three"
}
object(C)#3 (1) {
  ["aref"]=>
  &string(5) "three"
}

