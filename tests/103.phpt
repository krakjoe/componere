--TEST--
IS_CONSTANT_AST updates
--FILE--
<?php
class Foo {

	public $object = [
		self::URL => 1,
	];

	static $static = [
		self::URL => 2,
	];

}

$def = new Componere\Definition(Foo::class);
$def->addConstant("URL", new Componere\Value(42));
$def->register();

var_dump(new Foo());
var_dump(Foo::$static);
?>
--EXPECTF--
object(Foo)#%d (%d) {
  ["object"]=>
  array(1) {
    [42]=>
    int(1)
  }
}
array(1) {
  [42]=>
  int(2)
}

