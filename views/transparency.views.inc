<?php
/**
 * Implements hook_views_plugins().
 */
function transparency_views_plugins() {  
  //xdebug_break();
  $path = drupal_get_path('module', 'transparency');
  $plugins = array(
    'style' => array(
      'transparency' => array(
        'title' => t('Transparency views'),
        'help' => t('Combine json views into a client-side view.'),
        'help topic' => 'style-transparency',
        'handler' => 'views_plugin_style_transparency',
        'path' => $path . '/views',
        'theme' => 'views_view_transparency',
        'theme path' => $path . '/views',
        'uses fields' => TRUE,
        'uses options' => TRUE, 
        'uses grouping' => FALSE,
        'uses row plugin' => FALSE,
        'type' => 'normal',
        'even empty' => FALSE,
      )
    )
  );
  return $plugins;
}