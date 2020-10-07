<?php

/**
 * @generate-function-entries
 * @generate-legacy-arginfo
 */

namespace Componere\Abstract;

abstract class Definition {
	public function addMethod(string $name, \Componere\Method $method):static {}

	public function addTrait(string $trait):static {}

	public function addInterface(string $interface):static {}

	public function getReflector():\ReflectionClass {}
}


namespace Componere;

abstract class Definition extends Abstract\Definition {

	/**
	 * @param mixed $arguments
	 */
	public function __construct(...$arguments) {}

	public function addProperty(string $name, Value $value):static {}

	public function addConstant(string $name, Value $value):static {}

	public function setConstant(string $name, Value $value):static {}

	public function getClosure(string $name):\Closure {}

	public function getClosures():array {}

	/**
	 * @return void
	 */
	public function register() {}

	public function isRegistered():bool {}
}

