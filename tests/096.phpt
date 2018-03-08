--TEST--
Componere\cast_by_ref sub type handle dynamics already ref
--FILE--
<?php
class A {

}

class B extends A {
	public $aref;
}

class C extends B {}

$a = new A();

$aref = "one";

$a->aref = &$aref;

$b = Componere\cast_by_ref(B::class, $a);

var_dump($a, $b, $c = Componere\cast_by_ref(C::class, $b));

$aref = "two";

var_dump($a, $b, $c);

$aref = "three";

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

