<?php

/**
 * @generate-function-entries
 * @generate-legacy-arginfo
 */

namespace Componere;

abstract class Patch extends Abstract\Definition {

	/**
	 * @param mixed $arguments
	 */
	public function __construct(...$arguments) {}

	/**
	 * @return void
	 */
	public function apply() {}

	/**
	 * @return void
	 */
	public function revert() {}

	public function getClosure(string $name):\Closure {}

	public function getClosures():array {}

	public function isApplied():bool {}

	/**
	 * @param mixed $object
	 * @return void
	 */
	public function derive($object) {}
}

