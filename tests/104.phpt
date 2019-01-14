--TEST--
relinking classes working after unsetting definition
--FILE--
<?php
class Foo {
	const TEST = 1;

	protected $options = [
		FOO::TEST => 2
	];

	public function returnOptions() : array {
		return $this->options;
	}
}

$def = new Componere\Definition(Foo::class);
$def->addMethod("returnOptions", new Componere\Method(function(){
	return [$this->options, $this->options, $this->options];
}));
$def->register();

$foo = new Foo();
var_dump($foo->returnOptions());

unset($def);

$def = new Componere\Definition(Foo::class);
$def->register();

$foo = new Foo();
var_dump($foo->returnOptions());
?>
--EXPECT--
array(3) {
  [0]=>
  array(1) {
    [1]=>
    int(2)
  }
  [1]=>
  array(1) {
    [1]=>
    int(2)
  }
  [2]=>
  array(1) {
    [1]=>
    int(2)
  }
}
array(1) {
  [1]=>
  int(2)
}

