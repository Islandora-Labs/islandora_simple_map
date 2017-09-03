<?php
/**
 * @file
 * Simple Map Test Case
 */

class IslandoraSimpleMapTestCase extends IslandoraWebTestCase {
  /**
   * Location of test resources.
   *
   * @var string
   */
  protected $resourcePath;

  /**
   * Existing Use Google Maps Javascript API setting.
   *
   * @var bool
   */
  protected $jsApiState;

  /**
   * Array of objects created in the test, that should be removed.
   *
   * @var array
   */
  protected $testObjects = array();

  /**
   * {@inheritdoc}
   *
   * @see \IslandoraWebTestCase::setUp()
   */
  public function setUp() {
    $args = func_get_args();
    $args = (isset($args[0]) && is_array($args[0])) ? $args[0] : $args;
    $args[] = 'islandora_simple_map';
    parent::setUp($args);
    $this->jsApiState = variable_get('islandora_simple_map_use_gmaps_api', FALSE);
    $this->resourcePath = realpath(dirname(__FILE__) . "/../resources");
    $this->purgeTestObjects();
  }

  /**
   * Free any objects/resources created for this test.
   *
   * @see IslandoraUnitTestCase::tearDown()
   */
  public function tearDown() {
    $this->purgeTestObjects();
    variable_set('islandora_simple_map_use_gmaps_api', $this->jsApiState);
    parent::tearDown();
  }

  /**
   * Purge any objects created by the test's in this class.
   */
  protected function purgeTestObjects() {
    foreach ($this->testObjects as $object) {
      try {
        $object = $this->admin->repository->getObject($object);
        $object->label = "Don't Block";
        $this->admin->repository->purgeObject($object->id);
      }
      catch (Exception $e) {
        // Meh... Either it didn't exist or the purge failed.
      }
    }
  }

  /**
   * Enable/disable the javascript api setting for some tests.
   *
   * @param bool $val
   *   Value for the use javascript api variable.
   */
  protected function setJavascriptApi($val = FALSE) {
    variable_set('islandora_simple_map_use_gmaps_api', $val);
  }
}